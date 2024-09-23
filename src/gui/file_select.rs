use std::future::ready;
use std::path::{Path, PathBuf};

use crate::gui::{theme, with_tooltip, ElementState};
use crate::util;
use iced::widget::tooltip::Position;
use iced::widget::{button, row, text, text_input};
use iced::{Element, Renderer, Task};
use rfd::AsyncFileDialog;

pub struct FileSelect {
    path: String,
    display_strings: DisplayStrings,
    id: super::FileSelectId,
}

pub struct DisplayStrings {
    pub placeholder: &'static str,
    pub open_label: &'static str,
    pub open_tooltip: &'static str,
    pub save_label: &'static str,
    pub save_tooltip: &'static str,
    pub reload_label: &'static str,
    pub reload_tooltip: &'static str,
    pub dialog_title: &'static str,
    pub dialog_file_name: &'static str,
    pub dialog_filter_file: &'static str,
    pub dialog_filter_ext: &'static str,
    pub dialog_filter_all: &'static str,
    pub modal_description: &'static str,
}

impl FileSelect {
    pub fn new(id: super::FileSelectId, display_strings: DisplayStrings) -> Self {
        Self {
            path: Default::default(),
            display_strings,
            id,
        }
    }

    pub fn set_id(&mut self, id: super::FileSelectId) {
        self.id = id;
    }

    pub fn does_path_exist(&self) -> bool {
        Path::exists(self.path.as_ref())
    }

    /// wraps message in super::Message with `self.id`
    fn pack_message(&self, message: Message) -> super::Message {
        message.pack(self.id)
    }

    async fn run_file_dialog(dialog: AsyncFileDialog) -> Message {
        Message::Selected(dialog.pick_file().await.map(|path_buf| path_buf.into()))
    }

    fn build_file_dialog(&self) -> AsyncFileDialog {
        AsyncFileDialog::new()
            .set_title(self.display_strings.dialog_title)
            .set_directory(util::trim_to_existing_dir(self.path.as_ref()))
            .set_file_name(self.display_strings.dialog_file_name)
            .add_filter(
                self.display_strings.dialog_filter_file,
                &[self.display_strings.dialog_filter_ext],
            )
            .add_filter(self.display_strings.dialog_filter_all, &["*"])
    }

    pub fn update(&mut self, message: Message) -> Task<super::Message> {
        match message {
            Message::PathChanged(str_path) => {
                self.path = util::string_trim(str_path);
                Task::none()
            }
            Message::Open => {
                let id = self.id;
                let dialog = self.build_file_dialog();
                let modal_strings = (
                    self.display_strings.dialog_title,
                    self.display_strings.modal_description,
                );
                Task::batch([
                    Task::perform(ready(()), move |()| {
                        super::Message::OpenModal(modal_strings)
                    }),
                    Task::perform(Self::run_file_dialog(dialog), move |msg| msg.pack(id)),
                ])
            }
            Message::Selected(opt_path) => match opt_path {
                None => Task::perform(ready(()), |()| super::Message::CloseModal),
                Some(path) => {
                    let lossy = path.to_string_lossy();
                    match lossy.contains(char::REPLACEMENT_CHARACTER) {
                        true => Task::perform(
                            ready((
                                util::Error::Io("cannot represent path as String".into()),
                                "selecting new path".into(),
                            )),
                            super::Message::Error,
                        ),
                        false => {
                            self.path = lossy.to_string();
                            Task::batch([
                                Task::perform(ready(()), |()| super::Message::CloseModal),
                                Task::perform(ready(self.id), super::Message::Load),
                            ])
                        }
                    }
                }
            },
            Message::Save => Task::perform(ready(self.id), super::Message::Save),
            Message::Reload => Task::perform(ready(self.id), super::Message::Load),
        }
    }

    pub fn view(&self) -> Element<'_, super::Message, super::Theme, Renderer> {
        row![
            text_input(self.display_strings.placeholder, &self.path)
                .on_input(|string| self.pack_message(Message::PathChanged(string)))
                .style(theme::text_input),
            with_tooltip(
                button(text(self.display_strings.open_label))
                    .on_press(self.pack_message(Message::Open)),
                self.display_strings.open_tooltip,
                Position::Bottom,
            ),
            with_tooltip(
                button(text(self.display_strings.save_label))
                    .on_press(self.pack_message(Message::Save)),
                self.display_strings.save_tooltip,
                Position::Bottom,
            ),
            with_tooltip(
                button(text(self.display_strings.reload_label)).on_press_maybe(
                    self.does_path_exist()
                        .then(|| self.pack_message(Message::Reload))
                ),
                self.display_strings.reload_tooltip,
                Position::Bottom,
            )
        ]
        .spacing(4)
        .into()
    }
}

impl ElementState for FileSelect {
    type State = String;

    fn get_state(&self) -> Self::State {
        self.path.clone()
    }

    fn set_state(&mut self, state: Self::State) {
        self.path = state;
    }
}

#[derive(Debug, Clone)]
pub enum Message {
    PathChanged(String),
    Open,
    Selected(Option<PathBuf>),
    Save,
    Reload,
}

impl Message {
    /// wraps message in super::Message with `self.id`
    fn pack(self, id: super::FileSelectId) -> super::Message {
        super::Message::FileSelect(id, self)
    }
}
