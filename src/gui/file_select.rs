use std::future::ready;
use std::path::PathBuf;

use crate::gui::{with_tooltip, ElementState};
use crate::util;
use iced::widget::tooltip::Position;
use iced::widget::{button, row, text, text_input};
use iced::{Command, Element, Renderer};
use rfd::AsyncFileDialog;

pub struct FileSelect {
    path: PathBuf,
    display_strings: DisplayStrings,
    id: super::FileId,
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
}

impl FileSelect {
    pub fn new(id: super::FileId, display_strings: DisplayStrings) -> Self {
        Self {
            path: Default::default(),
            display_strings,
            id,
        }
    }

    pub fn set_id(&mut self, id: super::FileId) {
        self.id = id;
    }

    fn pack_message(&self, message: Message) -> super::Message {
        message.pack(self.id)
    }

    async fn run_file_dialog(dialog: AsyncFileDialog) -> Message {
        Message::Selected(dialog.pick_file().await.map(|path_buf| path_buf.into()))
    }

    fn build_file_dialog(&self) -> AsyncFileDialog {
        AsyncFileDialog::new()
            .set_title(self.display_strings.dialog_title)
            .set_directory(util::trim_to_existing_dir(&self.path))
            .set_file_name(self.display_strings.dialog_file_name)
            .add_filter(
                self.display_strings.dialog_filter_file,
                &[self.display_strings.dialog_filter_ext],
            )
            .add_filter(self.display_strings.dialog_filter_all, &["*"])
    }

    pub fn update(&mut self, message: Message) -> Command<super::Message> {
        match message {
            Message::PathChanged(str_path) => {
                self.path = PathBuf::from(str_path);
                Command::none()
            }
            Message::Open => {
                let id = self.id;
                let dialog = self.build_file_dialog(); // TODO lock ui?
                Command::perform(Self::run_file_dialog(dialog), move |msg| msg.pack(id))
            }
            Message::Selected(opt_path) => match opt_path {
                None => Command::none(),
                Some(path) => {
                    self.path = path;
                    Command::perform(ready(self.id), |id| super::Message::Load(id))
                }
            },
            Message::Save => Command::perform(ready(self.id), |id| super::Message::Save(id)),
            Message::Reload => Command::perform(ready(self.id), |id| super::Message::Load(id)),
        }
    }

    pub fn view(&self) -> Element<'_, super::Message, super::Theme, Renderer> {
        row![
            text_input(
                self.display_strings.placeholder,
                self.path.to_str().unwrap(),
            )
            .on_input(|string| self.pack_message(Message::PathChanged(string))),
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
                button(text(self.display_strings.reload_label))
                    .on_press(self.pack_message(Message::Reload)),
                self.display_strings.reload_tooltip,
                Position::Bottom,
            )
        ]
        .spacing(4)
        .into()
    }
}

impl ElementState for FileSelect {
    type State = PathBuf;

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
    fn pack(self, id: super::FileId) -> super::Message {
        super::Message::FileSelect(id, self)
    }
}
