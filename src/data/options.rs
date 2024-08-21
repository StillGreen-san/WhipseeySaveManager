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

#[derive(Clone, Debug, Default, Display, VariantArray)]
pub enum Scale {
    #[default]
    R768x432 = 2,
    R1152x648 = 3,
    R1536x864 = 4,
}

#[derive(Clone, Debug, Default, Display, VariantArray)]
pub enum Fullscreen {
    Disabled = 0,
    #[default]
    Enabled = 1,
}

#[derive(Clone, Debug, Default, Display, VariantArray)]
pub enum LeftHanded {
    Disabled = 0,
    #[default]
    Enabled = 1,
}

#[derive(Clone, Debug, Default, Display, VariantArray)]
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

#[derive(Clone, Debug, Default, Display, VariantArray)]
pub enum SoundToggle {
    Disabled = 0,
    #[default]
    Enabled = 1,
}

#[derive(Clone, Debug, Default, Display, VariantArray)]
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

#[derive(Clone, Debug, Default, Display, VariantArray)]
pub enum MusicToggle {
    Disabled = 0,
    #[default]
    Enabled = 1,
}

#[derive(Clone, Default)]
pub struct Options {
    pub language: Language,
    pub scale: Scale,
    pub fullscreen: Fullscreen,
    pub left_handed: LeftHanded,
    pub sound_volume: SoundVolume,
    pub sound_toggle: SoundToggle,
    pub music_volume: MusicVolume,
    pub music_toggle: MusicToggle,
}
