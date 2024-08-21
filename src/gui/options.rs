use crate::data;
use crate::data::options;
use crate::gui::{Tab, TabState, Theme};
use iced::widget::combo_box;
use iced::widget::{Column, ComboBox, Row, Text};
use iced::{Command, Element, Renderer};
use iced_aw::TabLabel;
use strum::VariantArray;

#[derive(Clone, Debug)]
pub enum Message {
    LanguageSelected(options::Language),
    ScaleSelected(options::Scale),
    FullscreenSelected(options::Fullscreen),
    LeftHandedSelected(options::LeftHanded),
    SoundVolumeSelected(options::SoundVolume),
    SoundToggleSelected(options::SoundToggle),
    MusicVolumeSelected(options::MusicVolume),
    MusicToggleSelected(options::MusicToggle),
}

impl Message {
    fn super_language_selected(language: options::Language) -> super::Message {
        super::Message::Options(Message::LanguageSelected(language))
    }
    fn super_scale_selected(scale: options::Scale) -> super::Message {
        super::Message::Options(Message::ScaleSelected(scale))
    }
    fn super_fullscreen_selected(fullscreen: options::Fullscreen) -> super::Message {
        super::Message::Options(Message::FullscreenSelected(fullscreen))
    }
    fn super_left_handed_selected(left_handed: options::LeftHanded) -> super::Message {
        super::Message::Options(Message::LeftHandedSelected(left_handed))
    }
    fn super_sound_volume_selected(sound_volume: options::SoundVolume) -> super::Message {
        super::Message::Options(Message::SoundVolumeSelected(sound_volume))
    }
    fn super_sound_toggle_selected(sound_toggle: options::SoundToggle) -> super::Message {
        super::Message::Options(Message::SoundToggleSelected(sound_toggle))
    }
    fn super_music_volume_selected(music_volume: options::MusicVolume) -> super::Message {
        super::Message::Options(Message::MusicVolumeSelected(music_volume))
    }
    fn super_music_toggle_selected(music_toggle: options::MusicToggle) -> super::Message {
        super::Message::Options(Message::MusicToggleSelected(music_toggle))
    }
}

pub struct Options {
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

impl Default for Options {
    fn default() -> Self {
        Self {
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
            Message::LanguageSelected(language) => {
                self.options_state.language = language;
                Command::none()
            }
            Message::ScaleSelected(scale) => {
                self.options_state.scale = scale;
                Command::none()
            }
            Message::FullscreenSelected(fullscreen) => {
                self.options_state.fullscreen = fullscreen;
                Command::none()
            }
            Message::LeftHandedSelected(left_handed) => {
                self.options_state.left_handed = left_handed;
                Command::none()
            }
            Message::SoundVolumeSelected(sound_volume) => {
                self.options_state.sound_volume = sound_volume;
                Command::none()
            }
            Message::SoundToggleSelected(sound_toggle) => {
                self.options_state.sound_toggle = sound_toggle;
                Command::none()
            }
            Message::MusicVolumeSelected(music_volume) => {
                self.options_state.music_volume = music_volume;
                Command::none()
            }
            Message::MusicToggleSelected(music_toggle) => {
                self.options_state.music_toggle = music_toggle;
                Command::none()
            }
        }
    }

    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        Column::new()
            .push(Row::new().push(Text::new("Language")).push(ComboBox::new(
                &self.language_state,
                "Language",
                Some(&self.options_state.language),
                Message::super_language_selected,
            )))
            .push(Row::new().push(Text::new("Scale")).push(ComboBox::new(
                &self.scale_state,
                "Scale",
                Some(&self.options_state.scale),
                Message::super_scale_selected,
            )))
            .push(Row::new().push(Text::new("Fullscreen")).push(ComboBox::new(
                &self.fullscreen_state,
                "Fullscreen",
                Some(&self.options_state.fullscreen),
                Message::super_fullscreen_selected,
            )))
            .push(
                Row::new()
                    .push(Text::new("Left Handed"))
                    .push(ComboBox::new(
                        &self.left_handed_state,
                        "Left Handed",
                        Some(&self.options_state.left_handed),
                        Message::super_left_handed_selected,
                    )),
            )
            .push(
                Row::new()
                    .push(Text::new("Sound Volume"))
                    .push(ComboBox::new(
                        &self.sound_volume_state,
                        "Sound Volume",
                        Some(&self.options_state.sound_volume),
                        Message::super_sound_volume_selected,
                    )),
            )
            .push(
                Row::new()
                    .push(Text::new("Sound Toggle"))
                    .push(ComboBox::new(
                        &self.sound_toggle_state,
                        "Sound Toggle",
                        Some(&self.options_state.sound_toggle),
                        Message::super_sound_toggle_selected,
                    )),
            )
            .push(
                Row::new()
                    .push(Text::new("Music Volume"))
                    .push(ComboBox::new(
                        &self.music_volume_state,
                        "Music Volume",
                        Some(&self.options_state.music_volume),
                        Message::super_music_volume_selected,
                    )),
            )
            .push(
                Row::new()
                    .push(Text::new("Music Toggle"))
                    .push(ComboBox::new(
                        &self.music_toggle_state,
                        "Music Toggle",
                        Some(&self.options_state.music_toggle),
                        Message::super_music_toggle_selected,
                    )),
            )
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
