use strum::{Display, VariantArray};

#[derive(Clone, Debug, Default, Display, VariantArray)]
pub enum Language {
    #[default]
    English = 0,
    Spanish = 1,
    French = 2,
    Italian = 3,
    German = 4,
    Russian = 5,
    Swedish = 6,
    Japanese = 7,
    Chinese = 8,
    Portuguese = 9,
}

#[derive(Clone, Debug, Default)]
pub enum Scale {
    #[default]
    R768x432 = 2,
    R1152x648 = 3,
    R1536x864 = 4,
}

#[derive(Clone, Debug, Default)]
pub enum Fullscreen {
    Disabled = 0,
    #[default]
    Enabled = 1,
}

#[derive(Clone, Debug, Default)]
pub enum LeftHanded {
    Disabled = 0,
    #[default]
    Enabled = 1,
}

#[derive(Clone, Debug, Default)]
pub enum SoundVolume {
    V0 = 0,
    V10 = 10,
    V20 = 20,
    V30 = 30,
    V40 = 40,
    V50 = 50,
    V60 = 60,
    V70 = 70,
    V80 = 80,
    V90 = 90,
    #[default]
    V100 = 100,
}

#[derive(Clone, Debug, Default)]
pub enum SoundToggle {
    Disabled = 0,
    #[default]
    Enabled = 1,
}

#[derive(Clone, Debug, Default)]
pub enum MusicVolume {
    V0 = 0,
    V10 = 10,
    V20 = 20,
    V30 = 30,
    V40 = 40,
    V50 = 50,
    V60 = 60,
    V70 = 70,
    V80 = 80,
    V90 = 90,
    #[default]
    V100 = 100,
}

#[derive(Clone, Debug, Default)]
pub enum MusicToggle {
    Disabled = 0,
    #[default]
    Enabled = 1,
}

#[derive(Clone, Default)]
pub struct Options {
    language: Language,
    scale: Scale,
    fullscreen: Fullscreen,
    left_handed: LeftHanded,
    sound_volume: SoundVolume,
    sound_toggle: SoundToggle,
    music_volume: MusicVolume,
    music_toggle: MusicToggle,
}
