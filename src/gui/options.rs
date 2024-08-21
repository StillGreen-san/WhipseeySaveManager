use crate::data;
use crate::data::options;
use crate::gui::options::Message::LanguageSelected;
use crate::gui::{Tab, TabState, Theme};
use iced::widget::combo_box;
use iced::widget::{Column, ComboBox, Row, Text};
use iced::{Command, Element, Renderer};
use iced_aw::TabLabel;
use strum::VariantArray;

#[derive(Clone, Debug)]
pub enum Message {
    LanguageSelected(options::Language),
}

impl Message {
    fn super_language_selected(language: options::Language) -> super::Message {
        super::Message::Options(LanguageSelected(language))
    }
}

pub struct Options {
    options_state: data::Options,
    language_state: combo_box::State<options::Language>,
    language_selected: Option<options::Language>,
}

impl Default for Options {
    fn default() -> Self {
        Self {
            options_state: Default::default(),
            language_state: combo_box::State::new(Vec::from(options::Language::VARIANTS)),
            language_selected: Some(Default::default()),
        }
    }
}

impl Tab for Options {
    type InMessage = Message;

    fn title(&self) -> String {
        "Options".into()
    }

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.title())
    }

    fn update(&mut self, message: Self::InMessage) -> Command<super::Message> {
        match message {
            LanguageSelected(language) => {
                self.language_selected = Some(language);
                Command::none()
            }
        }
    }

    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        Column::new()
            .push(Row::new().push(Text::new("Language")).push(ComboBox::new(
                &self.language_state,
                "Language",
                self.language_selected.as_ref(),
                Message::super_language_selected,
            )))
            .into()
    }
}

impl TabState for Options {
    type State = data::Options;

    fn get_state(&self) -> Self::State {
        self.options_state.clone()
    }

    fn set_state(&mut self, state: Self::State) {
        self.options_state = state;
    }
}
