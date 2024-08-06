use crate::data;
use crate::data::cheats::CheatsEnabled;
use crate::gui;
use crate::gui::{Tab, Theme};
use iced::{Command, Element, Renderer};
use iced_aw::TabLabel;

#[derive(Debug)]
pub enum Message {
    CheatsEnabled(CheatsEnabled),
}

pub struct Cheats {
    cheats_state: data::Cheats,
}

impl Tab for Cheats {
    type InMessage = gui::about::Message;

    fn title(&self) -> String {
        "Cheats".into()
    }

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.title())
    }

    fn update(&mut self, message: Self::InMessage) -> Command<super::Message> {
        todo!()
    }

    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        todo!()
    }
}
