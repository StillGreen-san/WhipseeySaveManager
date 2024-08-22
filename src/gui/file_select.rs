use std::future::ready;
use std::path::PathBuf;

use iced::widget::{Button, Row, Text, TextInput};
use iced::{Command, Element, Renderer};
use rfd::AsyncFileDialog;

pub struct FileSelect {
    path: PathBuf,
    display_strings: DisplayStrings,
    id: super::FileId,
}

pub struct DisplayStrings {
    pub placeholder: &'static str,
    pub open: &'static str,
    pub save: &'static str,
    pub reload: &'static str,
    pub dialog_title: &'static str,
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

    fn pack_message(&self, message: Message) -> super::Message {
        message.pack(self.id)
    }

    async fn run_file_dialog(dialog: AsyncFileDialog) -> Message {
        Message::Opened(dialog.pick_file().await.map(|path_buf| path_buf.into()))
    }

    fn build_file_dialog(display_strings: &DisplayStrings) -> AsyncFileDialog {
        AsyncFileDialog::new()
            .set_title(display_strings.dialog_title)
            .add_filter(
                display_strings.dialog_filter_file,
                &[display_strings.dialog_filter_ext],
            )
            .add_filter(display_strings.dialog_filter_all, &["*"])
    }

    pub fn update(&mut self, message: Message) -> Command<super::Message> {
        match message {
            Message::PathChanged(str_path) => {
                self.path = PathBuf::from(str_path);
                Command::none()
            }
            Message::Open => {
                let id = self.id;
                let dialog = Self::build_file_dialog(&self.display_strings);
                Command::perform(Self::run_file_dialog(dialog), move |msg| msg.pack(id))
            }
            Message::Opened(opt_path) => match opt_path {
                None => Command::none(),
                Some(path) => {
                    let id = self.id;
                    self.path = path;
                    Command::perform(ready(self.path.clone()), move |path| {
                        super::Message::Load(id, path)
                    })
                }
            },
            Message::Save => {
                let id = self.id;
                Command::perform(ready(self.path.clone()), move |path| {
                    super::Message::Save(id, path)
                })
            }
            Message::Reload => {
                let id = self.id;
                Command::perform(ready(self.path.clone()), move |path| {
                    super::Message::Load(id, path)
                })
            }
        }
    }

    pub fn view(&self) -> Element<'_, super::Message, super::Theme, Renderer> {
        Row::new()
            .push(
                TextInput::new(
                    self.display_strings.placeholder,
                    self.path.to_str().unwrap(),
                )
                .on_input(|string| self.pack_message(Message::PathChanged(string))),
            )
            .push(
                Button::new(Text::new(self.display_strings.open))
                    .on_press(self.pack_message(Message::Open)),
            )
            .push(
                Button::new(Text::new(self.display_strings.save))
                    .on_press(self.pack_message(Message::Save)),
            )
            .push(
                Button::new(Text::new(self.display_strings.reload))
                    .on_press(self.pack_message(Message::Reload)),
            )
            .into()
    }
}

#[derive(Debug, Clone)]
pub enum Message {
    PathChanged(String),
    Open,
    Opened(Option<PathBuf>),
    Save,
    Reload,
}

impl Message {
    fn pack(self, id: super::FileId) -> super::Message {
        super::Message::FileSelect(id, self)
    }
}
