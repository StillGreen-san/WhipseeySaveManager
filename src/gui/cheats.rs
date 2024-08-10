use crate::data;
use crate::data::cheats::CheatsEnabled;
use crate::gui::{Tab, Theme};
use iced::widget::{Checkbox, Column, Space, Text};
use iced::{Command, Element, Renderer};
use iced_aw::TabLabel;

#[derive(Debug, Clone)]
pub enum Message {
    CheatsToggled(bool),
    Cheats(data::Cheats),
}

pub struct Cheats {
    cheats_state: data::Cheats,
    display_strings: DisplayStrings,
}

pub struct DisplayStrings {
    pub checkbox: &'static str,
    pub description: &'static str,
}

impl Cheats {
    pub fn new(display_strings: DisplayStrings) -> Self {
        Self {
            cheats_state: Default::default(),
            display_strings,
        }
    }
}

impl Tab for Cheats {
    type InMessage = Message;

    fn title(&self) -> String {
        "Cheats".into()
    }

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.title())
    }

    fn update(&mut self, message: Self::InMessage) -> Command<super::Message> {
        match message {
            Message::CheatsToggled(state) => {
                self.cheats_state.cheats_enabled = CheatsEnabled::try_from(state as u8).unwrap();
                Command::none()
            }
            Message::Cheats(cheats) => {
                self.cheats_state = cheats;
                Command::none()
            }
        }
    }

    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        Column::new()
            .push(
                Checkbox::new(
                    self.display_strings.checkbox,
                    self.cheats_state.cheats_enabled == CheatsEnabled::Enabled,
                )
                .on_toggle(|state| super::Message::Cheats(Message::CheatsToggled(state))),
            )
            .push(Space::with_height(16.0))
            .push(Text::new(self.display_strings.description))
            .into()
    }
}
