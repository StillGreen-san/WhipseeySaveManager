use crate::gui::{Tab, Theme};
use iced::alignment::Horizontal;
use iced::widget::{button, column, row, scrollable, text, Space};
use iced::{Alignment, Command, Element, Length, Renderer};
use iced_aw::TabLabel;
use std::future::ready;

#[derive(Debug, Clone)]
pub enum Message {
    Link(&'static str),
}

pub struct DisplayStrings {
    pub title: &'static str,
    pub description: &'static str,
}

pub struct About {
    display_strings: DisplayStrings,
}

impl About {
    pub fn new(display_strings: DisplayStrings) -> Self {
        Self { display_strings }
    }
}

/// build a library entry
fn library(
    label: &'static str,
    link: &'static str,
) -> Element<'static, super::Message, Theme, Renderer> {
    row![
        Space::with_width(Length::FillPortion(15)),
        text(label)
            .width(Length::FillPortion(20))
            .horizontal_alignment(Horizontal::Right),
        button(text(link).horizontal_alignment(Horizontal::Left))
            .on_press(super::Message::About(Message::Link(link)))
            .style(iced::theme::Button::Text)
            .width(Length::FillPortion(50))
            .padding(0),
        Space::with_width(Length::FillPortion(15)),
    ]
    .spacing(4)
    .align_items(Alignment::Center)
    .width(Length::Fill)
    .into()
}

impl Tab for About {
    type InMessage = Message;

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.display_strings.title.into())
    }

    fn update(&mut self, message: Self::InMessage) -> Command<super::Message> {
        match message {
            Message::Link(link) => match opener::open(link) {
                Ok(()) => Command::none(),
                Err(error) => Command::perform(
                    ready((error.into(), "trying to open a link".into())),
                    super::Message::Error,
                ),
            },
        }
    }

    // TODO dynamic loading
    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        let libraries = column![
            library("iced [MIT]", "https://github.com/iced-rs/iced"),
            library("iced_aw [MIT]", "https://github.com/iced-rs/iced_aw"),
            library(
                "steamlocate [MIT]",
                "https://github.com/WilliamVenner/steamlocate-rs"
            ),
            library("rust-ini [MIT]", "https://github.com/zonyitoo/rust-ini"),
            library("opener [MIT]", "https://github.com/Seeker14491/opener"),
            library("rfd [MIT]", "https://github.com/PolyMeilex/rfd"),
            library("num_enum [MIT]", "https://github.com/illicitonion/num_enum"),
            library("thiserror [MIT]", "https://github.com/dtolnay/thiserror"),
            library("tokio [MIT]", "https://github.com/tokio-rs/tokio"),
            library("strum [MIT]", "https://github.com/Peternator7/strum"),
            library("num [MIT]", "https://github.com/rust-num/num"),
            library(
                "dark-light [MIT]",
                "https://github.com/frewsxcv/rust-dark-light"
            ),
            library("winres [MIT]", "https://github.com/mxre/winres"),
        ]
        .width(Length::Fill)
        .spacing(12)
        .align_items(Alignment::Center);
        column![
            text(self.display_strings.description),
            scrollable(libraries)
        ]
        .width(Length::Fill)
        .spacing(8)
        .padding([8, 0])
        .align_items(Alignment::Center)
        .into()
    }
}
