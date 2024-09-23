use crate::gui::{theme, Tab, Theme};
use iced::alignment::{Horizontal, Vertical};
use iced::widget::{button, column, row, scrollable, text, Space};
use iced::{Element, Length, Renderer, Task};
use iced_aw::TabLabel;

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
        text(label)
            .width(Length::FillPortion(35))
            .align_x(Horizontal::Right),
        row![
            button(text(link).align_x(Horizontal::Left))
                .on_press(super::Message::About(Message::Link(link)))
                .style(theme::button_link)
                .padding(0),
            Space::with_width(Length::Fill)
        ]
        .width(Length::FillPortion(65))
    ]
    .spacing(4)
    .align_y(Vertical::Center)
    .width(Length::Fill)
    .into()
}

impl Tab for About {
    type InMessage = Message;

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.display_strings.title.into())
    }

    fn update(&mut self, message: Self::InMessage) -> Task<super::Message> {
        match message {
            Message::Link(link) => match opener::open(link) {
                Ok(()) => Task::none(),
                Err(error) => Task::done(super::Message::Error((
                    error.into(),
                    "trying to open a link".into(),
                ))),
            },
        }
    }

    fn view(&self, _show_tooltips: bool) -> Element<'_, super::Message, Theme, Renderer> {
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
            library("winres [MIT]", "https://github.com/mxre/winres"),
            library(
                "const_format [ZLIB]",
                "https://github.com/rodrimati1992/const_format_crates/"
            ),
        ]
        .width(Length::Fill)
        .spacing(12)
        .align_x(Horizontal::Center);
        column![
            text(self.display_strings.description),
            scrollable(libraries)
        ]
        .width(Length::Fill)
        .spacing(8)
        .padding([8, 0])
        .align_x(Horizontal::Center)
        .into()
    }
}
