use crate::data::{try_from_opt_key, IniKeyStr, IniSectionStrFn};
use crate::{data, ini_impl_quoted, primitive_impl};
use ini::Properties;
use num_enum::{IntoPrimitive, TryFromPrimitive};
use std::ops::{Index, IndexMut};
use strum::{Display, VariantArray};

/// represents the `boss_no_damage_progress` Ini property in a `File` section in `whipseey.sav`
#[repr(u8)]
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
pub enum BossNoDamageProgress {
    #[default]
    None = 0,
    Forest = 1,
    Desert = 2,
    ForestDesert = 3,
    Castle = 4,
    ForestCastle = 5,
    DesertCastle = 6,
    ForestDesertCastle = 7,
}
ini_impl_quoted!(BossNoDamageProgress, "boss_no_damage_progress");

/// represents the `enemies_defeated` Ini property in a `File` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct EnemiesDefeated(u32);
primitive_impl!(EnemiesDefeated, 0, 0, 1677215, u32);
ini_impl_quoted!(EnemiesDefeated, "enemies_defeated", u32);

/// combines the level Ini properties in a `File` section in `whipseey.sav` into a single enum
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq, Ord, PartialOrd)]
#[repr(u8)]
#[derive(Display, VariantArray, TryFromPrimitive, IntoPrimitive)]
pub enum Level {
    Castle = 32,
    Moon = 16,
    Snow = 8,
    Desert = 4,
    Forest = 2,
    #[default]
    Beach = 1,
} // TODO make individual levels private?

impl Level {
    pub fn into_parts(self) -> (Castle, Moon, Snow, Desert, Forest) {
        let mut parts: (Castle, Moon, Snow, Desert, Forest) = Default::default();
        if self == Level::Castle {
            parts.0 = Castle::Unlocked;
        }
        if self >= Level::Moon {
            parts.1 = Moon::Unlocked;
        }
        if self >= Level::Snow {
            parts.2 = Snow::Unlocked;
        }
        if self >= Level::Desert {
            parts.3 = Desert::Unlocked;
        }
        if self >= Level::Forest {
            parts.4 = Forest::Unlocked;
        }
        parts
    } // TODO into/from impls?

    pub fn from_parts(parts: (Castle, Moon, Snow, Desert, Forest)) -> Self {
        if parts.0 == Castle::Unlocked {
            return Self::Castle;
        }
        if parts.1 == Moon::Unlocked {
            return Self::Moon;
        }
        if parts.2 == Snow::Unlocked {
            return Self::Snow;
        }
        if parts.3 == Desert::Unlocked {
            return Self::Desert;
        }
        if parts.4 == Forest::Unlocked {
            return Self::Forest;
        }
        Self::Beach
    }
}

/// represents the `castle` Ini property in a `File` section in `whipseey.sav`
///
/// this is an intermediate representation, convert to [Level] for actual use
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Castle {
    #[default]
    Locked = 0,
    Unlocked = 32,
}
ini_impl_quoted!(Castle, "castle");

/// represents the `moon` Ini property in a `File` section in `whipseey.sav`
///
/// this is an intermediate representation, convert to [Level] for actual use
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Moon {
    #[default]
    Locked = 0,
    Unlocked = 16,
}
ini_impl_quoted!(Moon, "moon");

/// represents the `snow` Ini property in a `File` section in `whipseey.sav`
///
/// this is an intermediate representation, convert to [Level] for actual use
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Snow {
    #[default]
    Locked = 0,
    Unlocked = 8,
}
ini_impl_quoted!(Snow, "snow");

/// represents the `desert` Ini property in a `File` section in `whipseey.sav`
///
/// this is an intermediate representation, convert to [Level] for actual use
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Desert {
    #[default]
    Locked = 0,
    Unlocked = 4,
}
ini_impl_quoted!(Desert, "desert");

/// represents the `forest` Ini property in a `File` section in `whipseey.sav`
///
/// this is an intermediate representation, convert to [Level] for actual use
#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Forest {
    #[default]
    Locked = 0,
    Unlocked = 2,
}
ini_impl_quoted!(Forest, "forest");

/// represents the `ending` Ini property in a `File` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Ending {
    #[default]
    Unwatched = 0,
    Watched = 1,
}
ini_impl_quoted!(Ending, "ending");

/// represents the `intro` Ini property in a `File` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Intro {
    #[default]
    Unwatched = 0,
    Watched = 1,
}
ini_impl_quoted!(Intro, "intro");

/// represents the `lives` Ini property in a `File` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct Lives(u32);
primitive_impl!(Lives, 1, 5, 99999, u32);
ini_impl_quoted!(Lives, "lives", u32);

/// represents the `gems` Ini property in a `File` section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct Gems(u8);
primitive_impl!(Gems, 0, 0, 99, u8);
ini_impl_quoted!(Gems, "gems");

/// represents a `File` Ini section in `whipseey.sav`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct File {
    pub boss_no_damage_progress: BossNoDamageProgress,
    pub enemies_defeated: EnemiesDefeated,
    pub level: Level,
    pub ending: Ending,
    pub intro: Intro,
    pub lives: Lives,
    pub gems: Gems,
}

impl TryFrom<&Properties> for File {
    type Error = data::Error;

    fn try_from(value: &Properties) -> Result<Self, Self::Error> {
        let castle = value.try_into()?;
        let moon = value.try_into()?;
        let snow = value.try_into()?;
        let desert = value.try_into()?;
        let forest = value.try_into()?;
        Ok(Self {
            boss_no_damage_progress: try_from_opt_key(value)?,
            enemies_defeated: value.try_into()?,
            level: Level::from_parts((castle, moon, snow, desert, forest)),
            ending: value.try_into()?,
            intro: value.try_into()?,
            lives: value.try_into()?,
            gems: value.try_into()?,
        })
    }
}

impl From<File> for Properties {
    fn from(value: File) -> Self {
        let mut properties = Properties::new();
        properties.insert(
            value.boss_no_damage_progress.ini_key_str(),
            value.boss_no_damage_progress,
        );
        properties.insert(value.enemies_defeated.ini_key_str(), value.enemies_defeated);
        let (castle, moon, snow, desert, forest) = value.level.into_parts();
        properties.insert(castle.ini_key_str(), castle);
        properties.insert(moon.ini_key_str(), moon);
        properties.insert(snow.ini_key_str(), snow);
        properties.insert(desert.ini_key_str(), desert);
        properties.insert(forest.ini_key_str(), forest);
        properties.insert(value.ending.ini_key_str(), value.ending);
        properties.insert(value.intro.ini_key_str(), value.intro);
        properties.insert(value.lives.ini_key_str(), value.lives);
        properties.insert(value.gems.ini_key_str(), value.gems);
        properties
    }
}

/// array index to access the `file1`, `file2`, and `file3` Ini sections of a `File` section in `whipseey.sav`
///
/// implements [Index] and [IndexMut]
#[derive(Clone, Copy, Debug)]
#[repr(u8)]
pub enum FileIndex {
    /// index for `file1` Ini section in `whipseey.sav`
    File1 = 0,
    /// index for `file2` Ini section in `whipseey.sav`
    File2 = 1,
    /// index for `file3` Ini section in `whipseey.sav`
    File3 = 2,
}
pub use FileIndex::*;
impl Index<FileIndex> for [File] {
    type Output = File;
    #[inline]
    fn index(&self, val: FileIndex) -> &Self::Output {
        self.get(val as usize).unwrap()
    }
}
impl IndexMut<FileIndex> for [File] {
    #[inline]
    fn index_mut(&mut self, val: FileIndex) -> &mut Self::Output {
        self.get_mut(val as usize).unwrap()
    }
}
impl IniSectionStrFn for FileIndex {
    fn ini_section_str(&self) -> &'static str {
        match self {
            File1 => "file1",
            File2 => "file2",
            File3 => "file3",
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::{assert_matches, util};
    use ini::Ini;

    #[test]
    fn file_into() {
        let file = File {
            boss_no_damage_progress: BossNoDamageProgress::ForestCastle,
            enemies_defeated: EnemiesDefeated::try_from(1337).unwrap(),
            level: Level::Moon,
            ending: Ending::Watched,
            intro: Intro::Watched,
            lives: Lives::try_from(5).unwrap(),
            gems: Gems::try_from(99).unwrap(),
        };
        let props: Properties = file.into();
        assert_eq!(
            props.get(BossNoDamageProgress::INI_KEY_STR),
            Some(String::from(file.boss_no_damage_progress).as_str())
        );
        assert_eq!(
            props.get(EnemiesDefeated::INI_KEY_STR),
            Some(String::from(file.enemies_defeated).as_str())
        );
        let (castle, moon, snow, desert, forest) = file.level.into_parts();
        assert_eq!(
            props.get(Castle::INI_KEY_STR),
            Some(String::from(castle).as_str())
        );
        assert_eq!(
            props.get(Moon::INI_KEY_STR),
            Some(String::from(moon).as_str())
        );
        assert_eq!(
            props.get(Snow::INI_KEY_STR),
            Some(String::from(snow).as_str())
        );
        assert_eq!(
            props.get(Desert::INI_KEY_STR),
            Some(String::from(desert).as_str())
        );
        assert_eq!(
            props.get(Forest::INI_KEY_STR),
            Some(String::from(forest).as_str())
        );
        assert_eq!(
            props.get(Ending::INI_KEY_STR),
            Some(String::from(file.ending).as_str())
        );
        assert_eq!(
            props.get(Intro::INI_KEY_STR),
            Some(String::from(file.intro).as_str())
        );
        assert_eq!(
            props.get(Lives::INI_KEY_STR),
            Some(String::from(file.lives).as_str())
        );
        assert_eq!(
            props.get(Gems::INI_KEY_STR),
            Some(String::from(file.gems).as_str())
        );
    }

    #[test]
    fn file_default() {
        let ini = Ini::load_from_str(util::test::ini::DEFAULT).unwrap();
        let section = ini.section(Some(File1.ini_section_str())).unwrap();
        let file_loaded = File::try_from(section).unwrap();
        let file_default = File::default();
        assert_eq!(file_loaded, file_default);
    }

    #[test]
    fn file_try_from_valid_complete() {
        let ini = Ini::load_from_str(util::test::ini::VALID).unwrap();
        let section = ini.section(Some(File1.ini_section_str())).unwrap();
        let file = File::try_from(section).unwrap();
        assert_eq!(
            file.boss_no_damage_progress,
            BossNoDamageProgress::ForestDesert
        );
        assert_eq!(
            file.enemies_defeated,
            EnemiesDefeated::try_from(2442).unwrap()
        );
        assert_eq!(file.level, Level::Castle);
        assert_eq!(file.ending, Ending::Watched);
        assert_eq!(file.intro, Intro::Watched);
        assert_eq!(file.lives, Lives::try_from(9118).unwrap());
        assert_eq!(file.gems, Gems::try_from(40).unwrap());
    }

    #[test]
    fn file_try_from_valid_missing() {
        let ini = Ini::load_from_str(util::test::ini::VALID).unwrap();
        let section = ini.section(Some(File3.ini_section_str())).unwrap();
        let file = File::try_from(section).unwrap();
        assert_eq!(file.boss_no_damage_progress, BossNoDamageProgress::None);
        assert_eq!(
            file.enemies_defeated,
            EnemiesDefeated::try_from(2442).unwrap()
        );
        assert_eq!(file.level, Level::Castle);
        assert_eq!(file.ending, Ending::Watched);
        assert_eq!(file.intro, Intro::Watched);
        assert_eq!(file.lives, Lives::try_from(9118).unwrap());
        assert_eq!(file.gems, Gems::try_from(40).unwrap());
    }

    #[test]
    fn file_try_from_lenient_values() {
        let ini = Ini::load_from_str(util::test::ini::LENIENT_VALUES).unwrap();
        let section = ini.section(Some(File2.ini_section_str())).unwrap();
        let file = File::try_from(section).unwrap();
        assert_eq!(
            file.boss_no_damage_progress,
            BossNoDamageProgress::ForestDesert
        );
        assert_eq!(
            file.enemies_defeated,
            EnemiesDefeated::try_from(2442).unwrap()
        );
        assert_eq!(file.level, Level::Castle);
        assert_eq!(file.ending, Ending::Watched);
        assert_eq!(file.intro, Intro::Watched);
        assert_eq!(file.lives, Lives::try_from(9118).unwrap());
        assert_eq!(file.gems, Gems::try_from(40).unwrap());
    }

    #[test]
    fn file_try_from_invalid_keys() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_KEYS).unwrap();
        let section = ini.section(Some(File1.ini_section_str())).unwrap();
        let error = File::try_from(section).unwrap_err();
        assert_matches!(
            error,
            data::Error::KeyMissing(key) if key == BossNoDamageProgress::INI_KEY_STR
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
        )
    }

    #[test]
    fn file_try_from_invalid_value_ranges() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_RANGES).unwrap();
        let section = ini.section(Some(File1.ini_section_str())).unwrap();
        let error = File::try_from(section).unwrap_err();
        assert_matches!(
            error,
            data::Error::TryFromPrimitive(_) | data::Error::NumCast(_)
        );
    }

    #[test]
    fn file_try_from_invalid_value_types() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_TYPES).unwrap();
        let section = ini.section(Some(File1.ini_section_str())).unwrap();
        let error = File::try_from(section).unwrap_err();
        assert_matches!(
            error,
            data::Error::NumCast(_) | data::Error::ParseInt(_) | data::Error::ParseFloat(_)
        );
    }
}
