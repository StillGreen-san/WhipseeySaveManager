use crate::data::{IniKeyStr, IniSectionStr};
use crate::{data, ini_impl_quoted};
use ini::Properties;
use num_enum::{IntoPrimitive, TryFromPrimitive};
use strum::{Display, VariantArray};

#[derive(Clone, Debug, Default, Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
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
ini_impl_quoted!(Language, Options, "language");

#[derive(Clone, Debug, Default, Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Scale {
    R768x432 = 2,
    #[default]
    R1152x648 = 3,
    R1536x864 = 4,
}
ini_impl_quoted!(Scale, Options, "scale");

#[derive(Clone, Debug, Default, Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Fullscreen {
    Disabled = 0,
    #[default]
    Enabled = 1,
}
ini_impl_quoted!(Fullscreen, Options, "fullscreen");

#[derive(Clone, Debug, Default, Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum LeftHanded {
    #[default]
    Disabled = 0,
    Enabled = 1,
}
ini_impl_quoted!(LeftHanded, Options, "left_handed");

#[derive(Clone, Debug, Default, Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
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
ini_impl_quoted!(SoundVolume, Options, "sound_volume", 100.0);

#[derive(Clone, Debug, Default, Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum SoundToggle {
    Disabled = 0,
    #[default]
    Enabled = 1,
}
ini_impl_quoted!(SoundToggle, Options, "sound_toggle");

#[derive(Clone, Debug, Default, Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
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
ini_impl_quoted!(MusicVolume, Options, "music_volume", 100.0);

#[derive(Clone, Debug, Default, Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum MusicToggle {
    Disabled = 0,
    #[default]
    Enabled = 1,
}
ini_impl_quoted!(MusicToggle, Options, "music_toggle");

#[derive(Clone, Debug, Default)]
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

impl IniSectionStr for Options {
    const INI_SECTION_STR: &'static str = "options";
}

impl TryFrom<&Properties> for Options {
    type Error = data::Error;

    fn try_from(value: &Properties) -> Result<Self, Self::Error> {
        Ok(Self {
            language: value.try_into()?,
            scale: value.try_into()?,
            fullscreen: value.try_into()?,
            left_handed: value.try_into()?,
            sound_volume: value.try_into()?,
            sound_toggle: value.try_into()?,
            music_volume: value.try_into()?,
            music_toggle: value.try_into()?,
        })
    }
}

impl From<Options> for Properties {
    fn from(value: Options) -> Self {
        let mut props = Properties::new();
        props.insert(value.language.ini_key_str(), value.language);
        props.insert(value.scale.ini_key_str(), value.scale);
        props.insert(value.fullscreen.ini_key_str(), value.fullscreen);
        props.insert(value.left_handed.ini_key_str(), value.left_handed);
        props.insert(value.sound_volume.ini_key_str(), value.sound_volume);
        props.insert(value.sound_toggle.ini_key_str(), value.sound_toggle);
        props.insert(value.music_volume.ini_key_str(), value.music_volume);
        props.insert(value.music_toggle.ini_key_str(), value.music_toggle);
        props
    }
}
