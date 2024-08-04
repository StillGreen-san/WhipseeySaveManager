use crate::gui::Theme;
use iced::widget::{Button, Row, Text, TextInput};
use iced::{Command, Element, Renderer};
use std::path::PathBuf;

pub struct File {
    path: PathBuf,
    state: State,
    placeholder: &'static str,
    id: super::FileId,
}

impl File {
    pub fn new(id: super::FileId, placeholder: &'static str) -> Self {
        Self {
            path: Default::default(),
            state: Default::default(),
            placeholder,
            id,
        }
    }

    fn pack_message(&self, message: Message) -> super::Message {
        super::Message::File(self.id, message)
    }

    pub fn update(&mut self, message: Message) -> Command<super::Message> {
        match message {
            Message::PathChanged(path) => todo!(),
            Message::Open => todo!(),
            Message::Save => todo!(),
            Message::Reload => todo!(),
        }
    }

    pub fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        Row::new()
            .push(
                TextInput::new(self.placeholder, self.path.to_str().unwrap())
                    .on_input(|string| self.pack_message(Message::PathChanged(string))),
            )
            .push(Button::new(Text::new("...")).on_press(self.pack_message(Message::Open)))
            .push(Button::new(Text::new("save")).on_press(self.pack_message(Message::Save)))
            .push(Button::new(Text::new("reload")).on_press(self.pack_message(Message::Reload)))
            .into()
    }
}

#[derive(Debug, Clone)]
pub enum Message {
    PathChanged(String),
    Open,
    Save,
    Reload,
}

#[derive(Default)]
enum State {
    #[default]
    NoPath,
    ValidPath,
    FilesystemError,
    ApplicationError,
}
