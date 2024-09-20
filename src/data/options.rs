use crate::data::{
    try_into_or_default_and_collect, FromPropsOrDefaulted, IniKeyStrFn, IniSectionStrVal,
};
use crate::{ini_impl_quoted, util};
use ini::Properties;
use num_enum::{IntoPrimitive, TryFromPrimitive};
use strum::{Display, VariantArray};

/// represents the `language` Ini property in a `options` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
#[repr(u8)]
#[derive(Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
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

/// represents the `scale` Ini property in a `options` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
#[repr(u8)]
#[derive(Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
pub enum Scale {
    R768x432 = 2,
    #[default]
    R1152x648 = 3,
    R1536x864 = 4,
}
ini_impl_quoted!(Scale, Options, "scale");

/// represents the `fullscreen` Ini property in a `options` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
#[repr(u8)]
#[derive(Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
pub enum Fullscreen {
    Disabled = 0,
    #[default]
    Enabled = 1,
}
ini_impl_quoted!(Fullscreen, Options, "fullscreen");

/// represents the `left_handed` Ini property in a `options` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
#[repr(u8)]
#[derive(Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
pub enum LeftHanded {
    #[default]
    Disabled = 0,
    Enabled = 1,
}
ini_impl_quoted!(LeftHanded, Options, "left_handed");

/// represents the `sound_volume` Ini property in a `options` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
#[repr(u8)]
#[derive(Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
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

/// represents the `sound_toggle` Ini property in a `options` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
#[repr(u8)]
#[derive(Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
pub enum SoundToggle {
    Disabled = 0,
    #[default]
    Enabled = 1,
}
ini_impl_quoted!(SoundToggle, Options, "sound_toggle");

/// represents the `music_volume` Ini property in a `options` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
#[repr(u8)]
#[derive(Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
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

/// represents the `music_toggle` Ini property in a `options` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
#[repr(u8)]
#[derive(Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
pub enum MusicToggle {
    Disabled = 0,
    #[default]
    Enabled = 1,
}
ini_impl_quoted!(MusicToggle, Options, "music_toggle");

/// represents the `options` Ini section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
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

impl IniSectionStrVal for Options {
    const INI_SECTION_STR: &'static str = "options";
}

impl FromPropsOrDefaulted for Options {
    type Error = util::Error;
    fn from_props_or_defaulted(value: &Properties) -> (Self, Vec<Self::Error>) {
        let mut errors = Vec::new();
        (
            Self {
                language: try_into_or_default_and_collect(value, &mut errors),
                scale: try_into_or_default_and_collect(value, &mut errors),
                fullscreen: try_into_or_default_and_collect(value, &mut errors),
                left_handed: try_into_or_default_and_collect(value, &mut errors),
                sound_volume: try_into_or_default_and_collect(value, &mut errors),
                sound_toggle: try_into_or_default_and_collect(value, &mut errors),
                music_volume: try_into_or_default_and_collect(value, &mut errors),
                music_toggle: try_into_or_default_and_collect(value, &mut errors),
            },
            errors,
        )
    }
}

impl TryFrom<&Properties> for Options {
    type Error = util::Error;

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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::data::IniKeyStrVal;
    use crate::{assert_matches, TEST_FAIL_STR};
    use ini::Ini;

    #[test]
    fn options_into_properties() {
        let options = Options {
            language: Language::Japanese,
            scale: Scale::R1152x648,
            fullscreen: Fullscreen::Disabled,
            left_handed: LeftHanded::Enabled,
            sound_volume: SoundVolume::V40,
            sound_toggle: SoundToggle::Enabled,
            music_volume: MusicVolume::V0,
            music_toggle: MusicToggle::Disabled,
        };
        let props: Properties = options.into();
        assert_eq!(
            props.get(Language::INI_KEY_STR),
            Some(String::from(options.language).as_str())
        );
        assert_eq!(
            props.get(Scale::INI_KEY_STR),
            Some(String::from(options.scale).as_str())
        );
        assert_eq!(
            props.get(Fullscreen::INI_KEY_STR),
            Some(String::from(options.fullscreen).as_str())
        );
        assert_eq!(
            props.get(LeftHanded::INI_KEY_STR),
            Some(String::from(options.left_handed).as_str())
        );
        assert_eq!(
            props.get(SoundVolume::INI_KEY_STR),
            Some(String::from(options.sound_volume).as_str())
        );
        assert_eq!(
            props.get(SoundToggle::INI_KEY_STR),
            Some(String::from(options.sound_toggle).as_str())
        );
        assert_eq!(
            props.get(MusicVolume::INI_KEY_STR),
            Some(String::from(options.music_volume).as_str())
        );
        assert_eq!(
            props.get(MusicToggle::INI_KEY_STR),
            Some(String::from(options.music_toggle).as_str())
        );
    }

    #[test]
    fn options_default() {
        let ini = Ini::load_from_str(util::test::ini::DEFAULT).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let options_loaded = Options::try_from(section).expect(TEST_FAIL_STR);
        let options_default = Options::default();
        assert_eq!(options_loaded, options_default);
    }

    #[test]
    fn options_try_from_properties_valid() {
        let ini = Ini::load_from_str(util::test::ini::VALID).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let options = Options::try_from(section).expect(TEST_FAIL_STR);
        assert_eq!(options.language, Language::Japanese);
        assert_eq!(options.scale, Scale::R1536x864);
        assert_eq!(options.fullscreen, Fullscreen::Disabled);
        assert_eq!(options.left_handed, LeftHanded::Enabled);
        assert_eq!(options.sound_volume, SoundVolume::V30);
        assert_eq!(options.sound_toggle, SoundToggle::Enabled);
        assert_eq!(options.music_volume, MusicVolume::V0);
        assert_eq!(options.music_toggle, MusicToggle::Disabled);
    }

    #[test]
    fn options_from_properties_valid() {
        let ini = Ini::load_from_str(util::test::ini::VALID).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let (options, errors) = Options::from_props_or_defaulted(section);
        assert!(errors.is_empty());
        assert_eq!(options.language, Language::Japanese);
        assert_eq!(options.scale, Scale::R1536x864);
        assert_eq!(options.fullscreen, Fullscreen::Disabled);
        assert_eq!(options.left_handed, LeftHanded::Enabled);
        assert_eq!(options.sound_volume, SoundVolume::V30);
        assert_eq!(options.sound_toggle, SoundToggle::Enabled);
        assert_eq!(options.music_volume, MusicVolume::V0);
        assert_eq!(options.music_toggle, MusicToggle::Disabled);
    }

    #[test]
    fn options_try_from_properties_lenient_values() {
        let ini = Ini::load_from_str(util::test::ini::LENIENT_VALUES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let options = Options::try_from(section).expect(TEST_FAIL_STR);
        assert_eq!(options.language, Language::Japanese);
        assert_eq!(options.scale, Scale::R1536x864);
        assert_eq!(options.fullscreen, Fullscreen::Disabled);
        assert_eq!(options.left_handed, LeftHanded::Enabled);
        assert_eq!(options.sound_volume, SoundVolume::V30);
        assert_eq!(options.sound_toggle, SoundToggle::Enabled);
        assert_eq!(options.music_volume, MusicVolume::V0);
        assert_eq!(options.music_toggle, MusicToggle::Disabled);
    }

    #[test]
    fn options_from_properties_lenient_values() {
        let ini = Ini::load_from_str(util::test::ini::LENIENT_VALUES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);

        let (options, errors) = Options::from_props_or_defaulted(section);
        assert!(errors.is_empty());
        assert_eq!(options.language, Language::Japanese);
        assert_eq!(options.scale, Scale::R1536x864);
        assert_eq!(options.fullscreen, Fullscreen::Disabled);
        assert_eq!(options.left_handed, LeftHanded::Enabled);
        assert_eq!(options.sound_volume, SoundVolume::V30);
        assert_eq!(options.sound_toggle, SoundToggle::Enabled);
        assert_eq!(options.music_volume, MusicVolume::V0);
        assert_eq!(options.music_toggle, MusicToggle::Disabled);
    }

    #[test]
    fn options_try_from_properties_invalid_keys() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_KEYS).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let error = Options::try_from(section).expect_err(TEST_FAIL_STR);
        assert_matches!(
            error,
            util::Error::KeyMissing(key) if key == Language::INI_KEY_STR
                                        || key == Scale::INI_KEY_STR
                                        || key == Fullscreen::INI_KEY_STR
                                        || key == LeftHanded::INI_KEY_STR
                                        || key == SoundVolume::INI_KEY_STR
                                        || key == SoundToggle::INI_KEY_STR
                                        || key == MusicVolume::INI_KEY_STR
                                        || key == MusicToggle::INI_KEY_STR
        )
    }

    #[test]
    fn options_from_properties_invalid_keys() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_KEYS).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let (options, errors) = Options::from_props_or_defaulted(section);
        assert_eq!(options, Default::default());
        for error in errors {
            assert_matches!(
                error,
                util::Error::KeyMissing(key) if key == Language::INI_KEY_STR
                                            || key == Scale::INI_KEY_STR
                                            || key == Fullscreen::INI_KEY_STR
                                            || key == LeftHanded::INI_KEY_STR
                                            || key == SoundVolume::INI_KEY_STR
                                            || key == SoundToggle::INI_KEY_STR
                                            || key == MusicVolume::INI_KEY_STR
                                            || key == MusicToggle::INI_KEY_STR
            );
        }
    }

    #[test]
    fn options_try_from_properties_invalid_value_ranges() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_RANGES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let error = Options::try_from(section).expect_err(TEST_FAIL_STR);
        assert_matches!(error, util::Error::TryFromPrimitive(_));
    }

    #[test]
    fn options_from_properties_invalid_value_ranges() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_RANGES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let (options, errors) = Options::from_props_or_defaulted(section);
        assert_eq!(options, Default::default());
        for error in errors {
            assert_matches!(error, util::Error::TryFromPrimitive(_));
        }
    }

    #[test]
    fn options_try_from_properties_invalid_value_types() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_TYPES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let error = Options::try_from(section).expect_err(TEST_FAIL_STR);
        assert_matches!(
            error,
            util::Error::NumCast(_) | util::Error::ParseInt(_) | util::Error::ParseFloat(_)
        );
    }

    #[test]
    fn options_from_properties_invalid_value_types() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_TYPES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Options::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let (options, errors) = Options::from_props_or_defaulted(section);
        assert_eq!(options, Default::default());
        for error in errors {
            assert_matches!(
                error,
                util::Error::NumCast(_) | util::Error::ParseInt(_) | util::Error::ParseFloat(_)
            );
        }
    }
}
