use crate::data;
use crate::data::cheats::CheatsEnabled;
use crate::gui::{ElementState, Tab, Theme};
use iced::widget::{checkbox, column, container, text};
use iced::{Element, Length, Renderer, Task};
use iced_aw::TabLabel;

#[derive(Debug, Clone)]
pub enum Message {
    CheatsToggled(bool),
}

pub struct Cheats {
    cheats_state: data::Cheats,
    display_strings: DisplayStrings,
}

pub struct DisplayStrings {
    pub title: &'static str,
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

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.display_strings.title.into())
    }

    fn update(&mut self, message: Self::InMessage) -> Task<super::Message> {
        match message {
            Message::CheatsToggled(state) => {
                self.cheats_state.cheats_enabled = CheatsEnabled::try_from(state as u8)
                    .expect("bool should always be convertible to CheatsEnabled");
                Task::none()
            }
        }
    }

    fn view(&self, _show_tooltips: bool) -> Element<'_, super::Message, Theme, Renderer> {
        container(
            column![
                checkbox(
                    self.display_strings.checkbox,
                    self.cheats_state.cheats_enabled == CheatsEnabled::Enabled,
                )
                .on_toggle(|state| super::Message::Cheats(Message::CheatsToggled(state))),
                text(self.display_strings.description)
            ]
            .spacing(16),
        )
        .center(Length::Fill)
        .into()
    }
}

impl ElementState for Cheats {
    type State = data::BfsSettings;

    fn get_state(&self) -> Self::State {
        Self::State {
            cheats: self.cheats_state,
        }
    }

    fn set_state(&mut self, state: Self::State) {
        self.cheats_state = state.cheats;
    }
}
