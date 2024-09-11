use crate::data;
use crate::data::options;
use crate::gui::{ElementState, Tab, Theme};
use iced::alignment::Horizontal;
use iced::widget::{column, combo_box, row, text, Space};
use iced::{Alignment, Command, Element, Length, Renderer};
use iced_aw::TabLabel;
use std::fmt::Display;
use strum::VariantArray;

#[derive(Clone, Debug)]
pub enum Message {
    Language(options::Language),
    Scale(options::Scale),
    Fullscreen(options::Fullscreen),
    LeftHanded(options::LeftHanded),
    SoundVolume(options::SoundVolume),
    SoundToggle(options::SoundToggle),
    MusicVolume(options::MusicVolume),
    MusicToggle(options::MusicToggle),
}

impl Message {
    fn super_language_selected(language: options::Language) -> super::Message {
        super::Message::Options(Message::Language(language))
    }
    fn super_scale_selected(scale: options::Scale) -> super::Message {
        super::Message::Options(Message::Scale(scale))
    }
    fn super_fullscreen_selected(fullscreen: options::Fullscreen) -> super::Message {
        super::Message::Options(Message::Fullscreen(fullscreen))
    }
    fn super_left_handed_selected(left_handed: options::LeftHanded) -> super::Message {
        super::Message::Options(Message::LeftHanded(left_handed))
    }
    fn super_sound_volume_selected(sound_volume: options::SoundVolume) -> super::Message {
        super::Message::Options(Message::SoundVolume(sound_volume))
    }
    fn super_sound_toggle_selected(sound_toggle: options::SoundToggle) -> super::Message {
        super::Message::Options(Message::SoundToggle(sound_toggle))
    }
    fn super_music_volume_selected(music_volume: options::MusicVolume) -> super::Message {
        super::Message::Options(Message::MusicVolume(music_volume))
    }
    fn super_music_toggle_selected(music_toggle: options::MusicToggle) -> super::Message {
        super::Message::Options(Message::MusicToggle(music_toggle))
    }
}

pub struct DisplayStrings {
    pub title: &'static str,
    pub language: &'static str,
    pub scale: &'static str,
    pub fullscreen: &'static str,
    pub left_handed: &'static str,
    pub sound_volume: &'static str,
    pub sound_toggle: &'static str,
    pub music_volume: &'static str,
    pub music_toggle: &'static str,
}

pub struct Options {
    display_strings: DisplayStrings,
    options_state: data::Options,
    language_state: combo_box::State<options::Language>,
    scale_state: combo_box::State<options::Scale>,
    fullscreen_state: combo_box::State<options::Fullscreen>,
    left_handed_state: combo_box::State<options::LeftHanded>,
    sound_volume_state: combo_box::State<options::SoundVolume>,
    sound_toggle_state: combo_box::State<options::SoundToggle>,
    music_volume_state: combo_box::State<options::MusicVolume>,
    music_toggle_state: combo_box::State<options::MusicToggle>,
}

impl Options {
    pub fn new(display_strings: DisplayStrings) -> Self {
        Self {
            display_strings,
            options_state: Default::default(),
            language_state: combo_box::State::new(Vec::from(options::Language::VARIANTS)),
            scale_state: combo_box::State::new(Vec::from(options::Scale::VARIANTS)),
            fullscreen_state: combo_box::State::new(Vec::from(options::Fullscreen::VARIANTS)),
            left_handed_state: combo_box::State::new(Vec::from(options::LeftHanded::VARIANTS)),
            sound_volume_state: combo_box::State::new(Vec::from(options::SoundVolume::VARIANTS)),
            sound_toggle_state: combo_box::State::new(Vec::from(options::SoundToggle::VARIANTS)),
            music_volume_state: combo_box::State::new(Vec::from(options::MusicVolume::VARIANTS)),
            music_toggle_state: combo_box::State::new(Vec::from(options::MusicToggle::VARIANTS)),
        }
    }
}

fn option<'a, T>(
    state: &'a combo_box::State<T>,
    placeholder: &str,
    selection: &T,
    on_selected: impl Fn(T) -> super::Message + 'static,
) -> Element<'a, super::Message, Theme, Renderer>
where
    T: Clone + Display + 'static,
{
    row![
        Space::with_width(Length::FillPortion(20)),
        text(placeholder)
            .width(Length::FillPortion(23))
            .horizontal_alignment(Horizontal::Right),
        combo_box(state, placeholder, Some(selection), on_selected)
            .width(Length::FillPortion(37))
            .padding(3),
        Space::with_width(Length::FillPortion(20)),
    ]
    .spacing(8)
    .align_items(Alignment::Center)
    .width(Length::Fill)
    .into()
}

impl Tab for Options {
    type InMessage = Message;

    fn title(&self) -> String {
        self.display_strings.title.into()
    }

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.title())
    }

    fn update(&mut self, message: Self::InMessage) -> Command<super::Message> {
        match message {
            Message::Language(language) => {
                self.options_state.language = language;
                Command::none()
            }
            Message::Scale(scale) => {
                self.options_state.scale = scale;
                Command::none()
            }
            Message::Fullscreen(fullscreen) => {
                self.options_state.fullscreen = fullscreen;
                Command::none()
            }
            Message::LeftHanded(left_handed) => {
                self.options_state.left_handed = left_handed;
                Command::none()
            }
            Message::SoundVolume(sound_volume) => {
                self.options_state.sound_volume = sound_volume;
                Command::none()
            }
            Message::SoundToggle(sound_toggle) => {
                self.options_state.sound_toggle = sound_toggle;
                Command::none()
            }
            Message::MusicVolume(music_volume) => {
                self.options_state.music_volume = music_volume;
                Command::none()
            }
            Message::MusicToggle(music_toggle) => {
                self.options_state.music_toggle = music_toggle;
                Command::none()
            }
        }
    }

    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        column![
            option(
                &self.language_state,
                self.display_strings.language,
                &self.options_state.language,
                Message::super_language_selected,
            ),
            option(
                &self.scale_state,
                self.display_strings.scale,
                &self.options_state.scale,
                Message::super_scale_selected,
            ),
            option(
                &self.fullscreen_state,
                self.display_strings.fullscreen,
                &self.options_state.fullscreen,
                Message::super_fullscreen_selected,
            ),
            option(
                &self.left_handed_state,
                self.display_strings.left_handed,
                &self.options_state.left_handed,
                Message::super_left_handed_selected,
            ),
            option(
                &self.sound_volume_state,
                self.display_strings.sound_volume,
                &self.options_state.sound_volume,
                Message::super_sound_volume_selected,
            ),
            option(
                &self.sound_toggle_state,
                self.display_strings.sound_toggle,
                &self.options_state.sound_toggle,
                Message::super_sound_toggle_selected,
            ),
            option(
                &self.music_volume_state,
                self.display_strings.music_volume,
                &self.options_state.music_volume, // TODO volume as slider? (change enum to f32?)
                Message::super_music_volume_selected,
            ),
            option(
                &self.music_toggle_state,
                self.display_strings.music_toggle,
                &self.options_state.music_toggle, // TODO remove from gui, instead infer from volume?
                Message::super_music_toggle_selected,
            ),
        ]
        .spacing(4)
        .align_items(Alignment::Center)
        .into()
    }
}

impl ElementState for Options {
    type State = data::Options;

    fn get_state(&self) -> Self::State {
        self.options_state.clone()
    }

    fn set_state(&mut self, state: Self::State) {
        self.options_state = state;
    }
}
