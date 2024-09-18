use crate::util;
use file::{File1, File2, File3};
use ini::{Ini, Properties};
use num::cast::AsPrimitive;
use num::NumCast;
use num_enum::{TryFromPrimitive, TryFromPrimitiveError};
use util::Error;
use util::Result;

pub mod cheats;
pub mod file;
pub mod options;

pub use cheats::Cheats;
pub use file::File;
pub use options::Options;

/// representation of whipseey.sav savegame data
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct WhipseeySaveData {
    pub options: Options,
    pub files: [File; 3],
}

impl TryFrom<Ini> for WhipseeySaveData {
    type Error = Error;

    fn try_from(value: Ini) -> Result<Self> {
        Ok(WhipseeySaveData {
            options: try_from(&value)?,
            files: [
                try_from_with_section_from(&value, File1)?,
                try_from_with_section_from(&value, File2)?,
                try_from_with_section_from(&value, File3)?,
            ],
        })
    }
}

impl From<WhipseeySaveData> for Ini {
    fn from(value: WhipseeySaveData) -> Self {
        let mut ini = Ini::new();
        ini.entry(Some(value.options.ini_section_str().into()))
            .or_insert(value.options.into());
        ini.entry(Some(File3.ini_section_str().into()))
            .or_insert(value.files[File3].into());
        ini.entry(Some(File2.ini_section_str().into()))
            .or_insert(value.files[File2].into());
        ini.entry(Some(File1.ini_section_str().into()))
            .or_insert(value.files[File1].into());
        ini
    }
}

/// representation of bfs_settings.ini data
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct BfsSettings {
    pub cheats: Cheats,
}

impl TryFrom<Ini> for BfsSettings {
    type Error = Error;

    fn try_from(value: Ini) -> Result<Self> {
        Ok(BfsSettings {
            cheats: try_from(&value)?,
        })
    } // TODO review try_from setup
}

impl From<BfsSettings> for Ini {
    fn from(value: BfsSettings) -> Self {
        let mut ini = Ini::new();
        ini.entry(Some(value.cheats.ini_section_str().into()))
            .or_insert(value.cheats.into());
        ini
    }
}

/// tries to convert from &[Properties] to `T`
///
/// returns [T::default()] if the key is missing
fn try_from_opt_key<'a, T>(value: &'a Properties) -> Result<T>
where
    T: Default + TryFrom<&'a Properties, Error = Error>,
{
    match value.try_into() {
        Ok(boss_no_damage_progress) => Ok(boss_no_damage_progress),
        Err(Error::KeyMissing(_)) => Ok(T::default()),
        Err(err) => Err(err),
    }
}

/// tries to convert from &[Ini] to `T`
fn try_from<'a, T>(value: &'a Ini) -> Result<T>
where
    T: IniSectionStrVal + TryFrom<&'a Properties, Error = Error>,
{
    value
        .section(Some(T::INI_SECTION_STR))
        .ok_or_else(|| Error::SectionMissing(T::INI_SECTION_STR.into()))?
        .try_into()
}

/// tries to convert from &[Ini] to `T` with a specific `section`
fn try_from_with_section_from<'a, S, T>(value: &'a Ini, section: S) -> Result<T>
where
    T: TryFrom<&'a Properties, Error = Error>,
    S: IniSectionStrFn,
{
    value
        .section(Some(section.ini_section_str()))
        .ok_or_else(|| Error::SectionMissing(section.ini_section_str().into()))?
        .try_into()
}

/// tries to convert from &[Properties] to `T`
///
/// reads a potentially quoted [f64] from `value` and multiplies it by `scale` before converting it to `T`
fn try_from_scaled<T, P>(value: &Properties, scale: f64) -> Result<T>
where
    T: IniKeyStrVal + TryFromPrimitive + TryFrom<P, Error = TryFromPrimitiveError<T>>,
    P: NumCast,
{
    let val_str = value
        .get(T::INI_KEY_STR)
        .ok_or(Error::KeyMissing(T::INI_KEY_STR.into()))?;
    let val: f64 = val_str.trim_matches('"').parse()?;
    let scaled = val * scale;
    let primitiv: P = num::cast(scaled).ok_or_else(|| {
        Error::NumCast(format!(
            "{} cannot be represented as {}",
            scaled,
            std::any::type_name::<P>()
        ))
    })?;
    let cheats_enabled = primitiv.try_into()?;
    Ok(cheats_enabled)
}

/// converts `value` to [f64] and multiplies it by `scale` before converting it to a quoted [String]
///
/// precision is set to 6 decimal places
fn into_quoted_scaled_float_string<T, P>(value: T, scale: f64) -> String
where
    T: Into<P>,
    P: AsPrimitive<f64>,
{
    let primitiv: P = value.into();
    let double: f64 = primitiv.as_();
    let scaled = double / scale;
    format!("\"{:.6}\"", scaled)
}

/// trait for getting the section string of an ini object
///
/// a blanked implementation for [IniSectionStrFn] is provided
trait IniSectionStrVal {
    const INI_SECTION_STR: &'static str;
}

/// trait for getting the section string of an ini object
///
/// prefer to use [IniSectionStrVal] if possible, it provides a blanked implementation for this trait
trait IniSectionStrFn {
    fn ini_section_str(&self) -> &'static str;
}
impl<T: IniSectionStrVal> IniSectionStrFn for T {
    fn ini_section_str(&self) -> &'static str {
        Self::INI_SECTION_STR
    }
}

/// trait for getting the key string of an ini object
///
/// a blanked implementation for [IniKeyStrFn] is provided
trait IniKeyStrVal {
    const INI_KEY_STR: &'static str;
}

/// trait for getting the key string of an ini object
///
/// prefer to use [IniKeyStrVal] if possible, it provides a blanked implementation for this trait
trait IniKeyStrFn {
    fn ini_key_str(&self) -> &'static str;
}
impl<T: IniKeyStrVal> IniKeyStrFn for T {
    fn ini_key_str(&self) -> &'static str {
        Self::INI_KEY_STR
    }
}

/// common implementation for Ini types
///
/// provides [IniSectionStrVal], [IniKeyStr], [TryFrom<&Properties>] and `.value()`
#[macro_export]
macro_rules! ini_impl_common {
    ($self:ty, $section:ident, $key:literal, $scale:literal, $typ:ty) => {
        $crate::ini_impl_common!($self, $key, $scale, $typ);
        impl $crate::data::IniSectionStrVal for $self {
            const INI_SECTION_STR: &'static str = $section::INI_SECTION_STR;
        }
    };

    ($self:ty, $key:literal, $scale:literal, $typ:ty) => {
        impl $crate::data::IniKeyStrVal for $self {
            const INI_KEY_STR: &'static str = $key;
        }
        impl TryFrom<&ini::Properties> for $self {
            type Error = $crate::util::Error;

            fn try_from(value: &ini::Properties) -> Result<Self, Self::Error> {
                $crate::data::try_from_scaled::<$self, $typ>(value, $scale)
            }
        }
    };

    ($self:ty, $section:ident, $key:literal) => {
        $crate::ini_impl_common!($self, $section, $key, 1.0, u8);
    };
}

/// quoted *(float)* implementation for Ini types
///
/// provides [IniSectionStrVal], [IniKeyStr], [TryFrom<&Properties>], `.value()` and [From<Self>]`<Self>` for [String]
#[macro_export]
macro_rules! ini_impl_quoted {
    ($self:ty, $section:ident, $key:literal, $scale:literal, $typ:ty) => {
        $crate::ini_impl_common!($self, $section, $key, $scale, $typ);
        impl From<$self> for String {
            fn from(value: $self) -> Self {
                $crate::data::into_quoted_scaled_float_string::<$self, $typ>(value, $scale)
            }
        }
    };

    ($self:ty, $key:literal, $scale:literal, $typ:ty) => {
        $crate::ini_impl_common!($self, $key, $scale, $typ);
        impl From<$self> for String {
            fn from(value: $self) -> Self {
                $crate::data::into_quoted_scaled_float_string::<$self, $typ>(value, $scale)
            }
        }
    };

    ($self:ty, $section:ident, $key:literal, $scale:literal) => {
        $crate::ini_impl_quoted!($self, $section, $key, $scale, u8);
    };

    ($self:ty, $section:ident, $key:literal) => {
        $crate::ini_impl_quoted!($self, $section, $key, 1.0);
    };

    ($self:ty, $section:ident, $key:literal, $typ:ty) => {
        $crate::ini_impl_quoted!($self, $section, $key, 1.0, $typ);
    };

    ($self:ty, $key:literal) => {
        $crate::ini_impl_quoted!($self, $key, 1.0, u8);
    };

    ($self:ty, $key:literal, $typ:ty) => {
        $crate::ini_impl_quoted!($self, $key, 1.0, $typ);
    };
}

/// primitive implementation for Ini types
///
/// provides [TryFromPrimitive], [IntoPrimitive], [Default], [PartialEq] as well as min/max constants
#[macro_export]
macro_rules! primitive_impl {
    ($self:ty, $min:literal, $default:literal, $max:expr, $typ:ty) => {
        impl $self {
            pub const MIN_PRIMITIV: $typ = $min;
            pub const MAX_PRIMITIV: $typ = $max as $typ;
            pub const DEFAULT_PRIMITIV: $typ = $default as $typ;
            pub const MIN: Self = Self($min);
            pub const MAX: Self = Self($max as $typ);
            pub const DEFAULT: Self = Self($default);
            pub fn value(&self) -> $typ {
                self.0
            }
        }
        impl ::num_enum::TryFromPrimitive for $self {
            type Primitive = $typ;
            type Error = ::num_enum::TryFromPrimitiveError<Self>;
            const NAME: &'static str = stringify!($self);
            fn try_from_primitive(
                number: Self::Primitive,
            ) -> ::core::result::Result<Self, ::num_enum::TryFromPrimitiveError<Self>> {
                #[deny(unreachable_patterns)]
                match number {
                    Self::MIN_PRIMITIV..=Self::MAX_PRIMITIV => {
                        ::core::result::Result::Ok(Self(number))
                    }
                    #[allow(unreachable_patterns)]
                    _ => ::core::result::Result::Err(
                        ::num_enum::TryFromPrimitiveError::<Self>::new(number),
                    ),
                }
            }
        }
        impl ::core::convert::TryFrom<$typ> for $self {
            type Error = ::num_enum::TryFromPrimitiveError<Self>;
            #[inline]
            fn try_from(
                number: $typ,
            ) -> ::core::result::Result<Self, ::num_enum::TryFromPrimitiveError<Self>> {
                ::num_enum::TryFromPrimitive::try_from_primitive(number)
            }
        }
        #[doc(hidden)]
        impl ::num_enum::CannotDeriveBothFromPrimitiveAndTryFromPrimitive for $self {}
        impl From<$self> for $typ {
            fn from(value: $self) -> Self {
                value.0
            }
        }
        impl Default for $self {
            fn default() -> Self {
                Self::DEFAULT
            }
        }
        impl PartialEq<$typ> for $self {
            fn eq(&self, other: &$typ) -> bool {
                self.0 == *other
            }
        }
    };
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::{assert_matches, TEST_FAIL_STR};
    use cheats::CheatsEnabled;
    use file::{
        BossNoDamageProgress, Castle, Desert, Ending, EnemiesDefeated, Forest, Gems, Intro, Level,
        Lives, Moon, Snow,
    };
    use options::{
        Fullscreen, Language, LeftHanded, MusicToggle, MusicVolume, Scale, SoundToggle, SoundVolume,
    };

    #[test]
    fn bfs_settings_into_ini() {
        let settings = BfsSettings {
            cheats: Cheats {
                cheats_enabled: CheatsEnabled::Disabled,
            },
        };
        let value: String = settings.cheats.cheats_enabled.into();
        let ini: Ini = settings.into();
        assert_eq!(
            ini.get_from(Some(Cheats::INI_SECTION_STR), CheatsEnabled::INI_KEY_STR),
            Some(value.as_str())
        );
    }

    #[test]
    fn bfs_settings_default() {
        let ini = Ini::load_from_str(util::test::ini::DEFAULT).expect(TEST_FAIL_STR);
        let settings_loaded = BfsSettings::try_from(ini).expect(TEST_FAIL_STR);
        let settings_default = BfsSettings::default();
        assert_eq!(settings_loaded, settings_default);
    }

    #[test]
    fn bfs_settings_try_from_ini_valid() {
        let ini = Ini::load_from_str(util::test::ini::VALID).expect(TEST_FAIL_STR);
        let settings = BfsSettings::try_from(ini).expect(TEST_FAIL_STR);
        assert_eq!(settings.cheats.cheats_enabled, CheatsEnabled::Enabled);
    }

    #[test]
    fn bfs_settings_try_from_ini_lenient() {
        let ini = Ini::load_from_str(util::test::ini::LENIENT_VALUES).expect(TEST_FAIL_STR);
        let settings = BfsSettings::try_from(ini).expect(TEST_FAIL_STR);
        assert_eq!(settings.cheats.cheats_enabled, CheatsEnabled::Enabled);
    }

    #[test]
    fn bfs_settings_try_from_ini_invalid_sections() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_SECTIONS).expect(TEST_FAIL_STR);
        let error = BfsSettings::try_from(ini).expect_err(TEST_FAIL_STR);
        assert_matches!(error, Error::SectionMissing(section) if section == Cheats::INI_SECTION_STR);
    }

    #[test]
    fn bfs_settings_try_from_ini_invalid_keys() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_KEYS).expect(TEST_FAIL_STR);
        let error = BfsSettings::try_from(ini).expect_err(TEST_FAIL_STR);
        assert_matches!(error, Error::KeyMissing(key) if key == CheatsEnabled::INI_KEY_STR);
    }

    #[test]
    fn bfs_settings_try_from_ini_invalid_value_ranges() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_RANGES).expect(TEST_FAIL_STR);
        let error = BfsSettings::try_from(ini).expect_err(TEST_FAIL_STR);
        assert_matches!(error, Error::TryFromPrimitive(_));
    }

    #[test]
    fn bfs_settings_try_from_ini_invalid_value_types() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_TYPES).expect(TEST_FAIL_STR);
        let error = BfsSettings::try_from(ini).expect_err(TEST_FAIL_STR);
        assert_matches!(
            error,
            Error::NumCast(_) | Error::ParseInt(_) | Error::ParseFloat(_)
        );
    }

    #[test]
    fn whipseey_save_data_into_ini() {
        let file1 = File {
            boss_no_damage_progress: BossNoDamageProgress::Desert,
            enemies_defeated: EnemiesDefeated::try_from(0).expect(TEST_FAIL_STR),
            level: Level::Beach,
            ending: Ending::Unwatched,
            intro: Intro::Unwatched,
            lives: Lives::try_from(1).expect(TEST_FAIL_STR),
            gems: Gems::try_from(12).expect(TEST_FAIL_STR),
        };
        let file2 = File {
            level: Level::Snow,
            ending: Ending::Watched,
            ..file1
        };
        let file3 = File {
            level: Level::Castle,
            ..file1
        };
        let data = WhipseeySaveData {
            options: Options {
                language: Language::French,
                scale: Scale::R768x432,
                fullscreen: Fullscreen::Disabled,
                left_handed: LeftHanded::Disabled,
                sound_volume: SoundVolume::V70,
                sound_toggle: SoundToggle::Disabled,
                music_volume: MusicVolume::V50,
                music_toggle: MusicToggle::Enabled,
            },
            files: [file1, file2, file3],
        };
        let ini: Ini = data.into();
        assert_eq!(
            ini.get_from(Some(Language::INI_SECTION_STR), Language::INI_KEY_STR),
            Some(String::from(data.options.language).as_str())
        );
        assert_eq!(
            ini.get_from(Some(Scale::INI_SECTION_STR), Scale::INI_KEY_STR),
            Some(String::from(data.options.scale).as_str())
        );
        assert_eq!(
            ini.get_from(Some(Fullscreen::INI_SECTION_STR), Fullscreen::INI_KEY_STR),
            Some(String::from(data.options.fullscreen).as_str())
        );
        assert_eq!(
            ini.get_from(Some(LeftHanded::INI_SECTION_STR), LeftHanded::INI_KEY_STR),
            Some(String::from(data.options.left_handed).as_str())
        );
        assert_eq!(
            ini.get_from(Some(SoundVolume::INI_SECTION_STR), SoundVolume::INI_KEY_STR),
            Some(String::from(data.options.sound_volume).as_str())
        );
        assert_eq!(
            ini.get_from(Some(SoundToggle::INI_SECTION_STR), SoundToggle::INI_KEY_STR),
            Some(String::from(data.options.sound_toggle).as_str())
        );
        assert_eq!(
            ini.get_from(Some(MusicVolume::INI_SECTION_STR), MusicVolume::INI_KEY_STR),
            Some(String::from(data.options.music_volume).as_str())
        );
        assert_eq!(
            ini.get_from(Some(MusicToggle::INI_SECTION_STR), MusicToggle::INI_KEY_STR),
            Some(String::from(data.options.music_toggle).as_str())
        );
        assert_eq!(
            ini.get_from(
                Some(File1.ini_section_str()),
                BossNoDamageProgress::INI_KEY_STR
            ),
            Some(String::from(data.files[File1].boss_no_damage_progress).as_str())
        );
        assert_eq!(
            ini.get_from(Some(File2.ini_section_str()), EnemiesDefeated::INI_KEY_STR),
            Some(String::from(data.files[File2].enemies_defeated).as_str())
        );
        let (_, file1_moon, _, _, file1_forest) = data.files[File1].level.into();
        let (_, _, file2_snow, _, _) = data.files[File2].level.into();
        let (file3_castle, _, _, file3_desert, _) = data.files[File3].level.into();
        assert_eq!(
            ini.get_from(Some(File3.ini_section_str()), Castle::INI_KEY_STR),
            Some(String::from(file3_castle).as_str())
        );
        assert_eq!(
            ini.get_from(Some(File1.ini_section_str()), Moon::INI_KEY_STR),
            Some(String::from(file1_moon).as_str())
        );
        assert_eq!(
            ini.get_from(Some(File2.ini_section_str()), Snow::INI_KEY_STR),
            Some(String::from(file2_snow).as_str())
        );
        assert_eq!(
            ini.get_from(Some(File3.ini_section_str()), Desert::INI_KEY_STR),
            Some(String::from(file3_desert).as_str())
        );
        assert_eq!(
            ini.get_from(Some(File1.ini_section_str()), Forest::INI_KEY_STR),
            Some(String::from(file1_forest).as_str())
        );
        assert_eq!(
            ini.get_from(Some(File2.ini_section_str()), Ending::INI_KEY_STR),
            Some(String::from(data.files[File2].ending).as_str())
        );
        assert_eq!(
            ini.get_from(Some(File3.ini_section_str()), Intro::INI_KEY_STR),
            Some(String::from(data.files[File3].intro).as_str())
        );
        assert_eq!(
            ini.get_from(Some(File1.ini_section_str()), Lives::INI_KEY_STR),
            Some(String::from(data.files[File1].lives).as_str())
        );
        assert_eq!(
            ini.get_from(Some(File2.ini_section_str()), Gems::INI_KEY_STR),
            Some(String::from(data.files[File2].gems).as_str())
        );
    }

    #[test]
    fn whipseey_save_data_default() {
        let ini = Ini::load_from_str(util::test::ini::DEFAULT).expect(TEST_FAIL_STR);
        let save_loaded = WhipseeySaveData::try_from(ini).expect(TEST_FAIL_STR);
        let save_default = WhipseeySaveData::default();
        assert_eq!(save_loaded, save_default);
    }

    #[test]
    fn whipseey_save_data_try_from_ini_valid() {
        let ini = Ini::load_from_str(util::test::ini::VALID).expect(TEST_FAIL_STR);
        let save = WhipseeySaveData::try_from(ini).expect(TEST_FAIL_STR);
        assert_eq!(save.options.sound_volume, SoundVolume::V30);
        assert_eq!(
            save.files[File1].boss_no_damage_progress,
            BossNoDamageProgress::ForestDesert
        );
        assert_eq!(
            save.files[File2].boss_no_damage_progress,
            BossNoDamageProgress::None
        );
        assert_eq!(save.files[File3].ending, Ending::Watched);
    }

    #[test]
    fn whipseey_save_data_try_from_ini_lenient() {
        let ini = Ini::load_from_str(util::test::ini::LENIENT_VALUES).expect(TEST_FAIL_STR);
        let save = WhipseeySaveData::try_from(ini).expect(TEST_FAIL_STR);
        assert_eq!(save.options.scale, Scale::R1536x864);
        assert_eq!(
            save.files[File3].gems,
            Gems::try_from(40).expect(TEST_FAIL_STR)
        );
        assert_eq!(save.files[File2].level, Level::Castle);
        assert_eq!(save.files[File1].ending, Ending::Watched);
    }

    #[test]
    fn whipseey_save_data_try_from_ini_invalid_sections() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_SECTIONS).expect(TEST_FAIL_STR);
        let error = WhipseeySaveData::try_from(ini).expect_err(TEST_FAIL_STR);
        assert_matches!(
            error,
            Error::SectionMissing(section) if section == Options::INI_SECTION_STR
                                        || section == File1.ini_section_str()
                                        || section == File2.ini_section_str()
                                        || section == File3.ini_section_str()
        );
    }

    #[test]
    fn whipseey_save_data_try_from_ini_invalid_keys() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_KEYS).expect(TEST_FAIL_STR);
        let error = WhipseeySaveData::try_from(ini).expect_err(TEST_FAIL_STR);
        assert_matches!(
            error,
            Error::KeyMissing(key) if key == Language::INI_KEY_STR
                                || key == Scale::INI_KEY_STR
                                || key == Fullscreen::INI_KEY_STR
                                || key == LeftHanded::INI_KEY_STR
                                || key == SoundVolume::INI_KEY_STR
                                || key == SoundToggle::INI_KEY_STR
                                || key == MusicVolume::INI_KEY_STR
                                || key == MusicToggle::INI_KEY_STR
                                || key == BossNoDamageProgress::INI_KEY_STR
                                || key == EnemiesDefeated::INI_KEY_STR
                                || key == Castle::INI_KEY_STR
                                || key == Moon::INI_KEY_STR
                                || key == Snow::INI_KEY_STR
                                || key == Desert::INI_KEY_STR
                                || key == Forest::INI_KEY_STR
                                || key == Ending::INI_KEY_STR
                                || key == Intro::INI_KEY_STR
                                || key == Lives::INI_KEY_STR
                                || key == Gems::INI_KEY_STR
        );
    }

    #[test]
    fn whipseey_save_data_try_from_ini_invalid_value_ranges() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_RANGES).expect(TEST_FAIL_STR);
        let error = WhipseeySaveData::try_from(ini).expect_err(TEST_FAIL_STR);
        assert_matches!(error, Error::TryFromPrimitive(_));
    }

    #[test]
    fn whipseey_save_data_try_from_ini_invalid_value_types() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_TYPES).expect(TEST_FAIL_STR);
        let error = WhipseeySaveData::try_from(ini).expect_err(TEST_FAIL_STR);
        assert_matches!(
            error,
            Error::NumCast(_) | Error::ParseInt(_) | Error::ParseFloat(_)
        );
    }
}
