use crate::gui::{Tab, Theme};
use iced::alignment::Horizontal;
use iced::widget::{Button, Column, Row, Scrollable, Text};
use iced::{Alignment, Color, Command, Element, Length, Renderer};
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

impl About {
    fn library(
        &self,
        label: &'static str,
        link: &'static str,
    ) -> Element<'_, super::Message, Theme, Renderer> {
        Row::new()
            .push(
                Text::new(label)
                    .width(Length::FillPortion(1))
                    .horizontal_alignment(Horizontal::Right),
            )
            .push(
                Button::new(
                    Text::new(link)
                        .horizontal_alignment(Horizontal::Left)
                        .style(Color::from_rgb8(50, 90, 220)),
                )
                .on_press(super::Message::About(Message::Link(link)))
                .style(iced::theme::Button::Text)
                .width(Length::FillPortion(2))
                .height(Length::Shrink),
            )
            .align_items(Alignment::Center)
            .width(Length::Fill)
            .into()
    }
}

impl Tab for About {
    type InMessage = Message;

    fn title(&self) -> String {
        self.display_strings.title.into()
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

    // TODO dynamic loading
    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        let libraries = Column::new()
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
            .push(self.library("tokio [MIT]", "https://github.com/tokio-rs/tokio"))
            .push(self.library("strum [MIT]", "https://github.com/Peternator7/strum"))
            .push(self.library("num [MIT]", "https://github.com/rust-num/num"))
            .width(Length::Fill)
            .spacing(4.0)
            .align_items(Alignment::Center);
        Column::new()
            .push(Text::new(self.display_strings.description))
            .push(Scrollable::new(libraries))
            .width(Length::Fill)
            .spacing(8.0)
            .padding([8.0, 0.0])
            .align_items(Alignment::Center)
            .into()
    }
}
