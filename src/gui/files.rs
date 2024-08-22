use crate::data::File;
use crate::gui::{Tab, TabState, Theme};
use iced::widget::Row;
use iced::{Command, Element, Renderer};
use iced_aw::TabLabel;

#[derive(Clone, Debug)]
pub enum Message {}

pub struct DisplayStrings {
    pub titel: &'static str,
}

pub struct Files {
    files_state: [File; 3],
    display_strings: DisplayStrings,
}

impl Files {
    pub fn new(display_strings: DisplayStrings) -> Self {
        Self {
            files_state: Default::default(),
            display_strings,
        }
    }
}

impl Tab for Files {
    type InMessage = Message;

    fn title(&self) -> String {
        self.display_strings.titel.into()
    }

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.title())
    }

    fn update(&mut self, message: Self::InMessage) -> Command<super::Message> {
        Command::none()
    }

    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        Row::new().into()
    }
}

impl TabState for Files {
    type State = [File; 3];

    fn get_state(&self) -> Self::State {
        self.files_state.clone()
    }

    fn set_state(&mut self, state: Self::State) {
        self.files_state = state;
    }
}
