use crate::gui::Theme;
use iced::widget::{Button, Row, Text, TextInput};
use iced::{Command, Element, Renderer};
use rfd::{AsyncFileDialog, FileHandle};
use std::future::Future;
use std::path::PathBuf;

pub struct File {
    path: PathBuf,
    display_strings: DisplayStrings,
    state: State,
    id: super::FileId,
}

pub struct DisplayStrings {
    pub placeholder: &'static str,
    pub open: &'static str,
    pub save: &'static str,
    pub reload: &'static str,
    pub dialog_title: &'static str,
    pub dialog_filter_save: &'static str,
    pub dialog_filter_all: &'static str,
}

impl File {
    pub fn new(id: super::FileId, display_strings: DisplayStrings) -> Self {
        Self {
            path: Default::default(),
            display_strings,
            state: Default::default(),
            id,
        }
    }

    fn pack_message(&self, message: Message) -> super::Message {
        message.pack(self.id)
    }

    async fn run_file_dialog(dialog: impl Future<Output = Option<FileHandle>> + Sized) -> Message {
        Message::Opened(dialog.await.map(|path_buf| path_buf.into()))
    }

    fn build_file_dialog(
        display_strings: &DisplayStrings,
    ) -> impl Future<Output = Option<FileHandle>> + Sized {
        AsyncFileDialog::new()
            .set_title(display_strings.dialog_title)
            .add_filter(display_strings.dialog_filter_save, &["sav"])
            .add_filter(display_strings.dialog_filter_all, &["*"])
            .pick_file()
    }

    pub fn update(&mut self, message: Message) -> Command<super::Message> {
        match message {
            Message::PathChanged(path) => todo!(),
            Message::Open => {
                let id = self.id;
                let dialog = Self::build_file_dialog(&self.display_strings);
                Command::perform(Self::run_file_dialog(dialog), move |msg| msg.pack(id))
            }
            Message::Opened(_) => todo!(),
            Message::Save => todo!(),
            Message::Reload => todo!(),
        }
    }

    pub fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
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
        super::Message::File(id, self)
    }
}

#[derive(Default)]
enum State {
    #[default]
    NoPath,
    ValidPath,
    FilesystemError,
    ApplicationError,
}
