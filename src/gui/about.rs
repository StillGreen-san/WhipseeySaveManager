use crate::gui::{Tab, Theme};
use iced::widget::{Button, Column, Row, Space, Text};
use iced::{Command, Element, Renderer};
use iced_aw::TabLabel;

#[derive(Debug, Clone)]
pub enum Message {
    Link(&'static str),
}

#[derive(Default)]
pub struct About;

impl About {
    // TODO hyperlink
    fn library(
        &self,
        label: &'static str,
        link: &'static str,
    ) -> Element<'_, super::Message, Theme, Renderer> {
        Row::new()
            .push(Text::new(label))
            .push(Button::new(link).on_press(super::Message::About(Message::Link(link))))
            .into()
    }
}

impl Tab for About {
    type InMessage = Message;

    fn title(&self) -> String {
        "About".into()
    }

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.title())
    }

    fn update(&mut self, message: Self::InMessage) -> Command<super::Message> {
        match message {
            Message::Link(link) => {
                opener::open(link).unwrap(); // TODO error handling
            }
        }
        Command::none()
    }

    // TODO dynamic loading, layout
    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        Column::new()
            .push(Text::new("this program uses the following libraries:"))
            .push(Space::with_height(16.0))
            .push(self.library("iced [MIT]", "https://github.com/iced-rs/iced"))
            .push(self.library("iced_aw [MIT]", "https://github.com/iced-rs/iced_aw"))
            .push(self.library(
                "steamlocate [MIT]",
                "https://github.com/WilliamVenner/steamlocate-rs",
            ))
            .push(self.library("rust-ini [MIT]", "https://github.com/zonyitoo/rust-ini"))
            .push(self.library("opener [MIT]", "https://github.com/Seeker14491/opener"))
            .push(self.library("rfd [MIT]", "https://github.com/PolyMeilex/rfd"))
            .push(self.library("num_enum [MIT]", "https://github.com/illicitonion/num_enum"))
            .push(self.library("thiserror [MIT]", "https://github.com/dtolnay/thiserror"))
            .push(self.library(
                "enumflags2 [MIT]",
                "https://github.com/meithecatte/enumflags2",
            ))
            .push(self.library("tokio [MIT]", "https://github.com/tokio-rs/tokio"))
            .push(self.library("strum [MIT]", "https://github.com/Peternator7/strum"))
            .into()
    }
}
