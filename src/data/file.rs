use crate::data::{IniKeyStr, IniSectionStr};
use crate::{data, ini_impl, primitive_impl};
use ini::Properties;
use num_enum::{IntoPrimitive, TryFromPrimitive};
use strum::{Display, VariantArray};

#[repr(u8)]
#[derive(Copy, Clone, Debug, Default, TryFromPrimitive, IntoPrimitive)]
pub enum BossNoDamage {
    #[default]
    None = 0,
    Forest = 1,
    Desert = 2,
    ForestDesert = 3,
    Castle = 4,
    ForstCastle = 5,
    DesertCastle = 6,
    ForstDesertCastle = 7,
}
ini_impl!(BossNoDamage, File, "boss_no_damage");

#[derive(Clone, Debug, Default)]
pub struct EnemiesDefeated(u32);
primitive_impl!(EnemiesDefeated, 0, 1677215, u32);
ini_impl!(EnemiesDefeated, File, "enemies_defeated", u32);

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
}

impl Level {
    fn into_parts(self) -> (Castle, Moon, Snow, Desert, Forest) {
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
    }

    fn from_parts(parts: (Castle, Moon, Snow, Desert, Forest)) -> Self {
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

#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Castle {
    #[default]
    Locked = 0,
    Unlocked = 32,
}
ini_impl!(Castle, File, "castle");

#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Moon {
    #[default]
    Locked = 0,
    Unlocked = 16,
}
ini_impl!(Moon, File, "moon");

#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Snow {
    #[default]
    Locked = 0,
    Unlocked = 8,
}
ini_impl!(Snow, File, "snow");

#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Desert {
    #[default]
    Locked = 0,
    Unlocked = 4,
}
ini_impl!(Desert, File, "desert");

#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Forest {
    #[default]
    Locked = 0,
    Unlocked = 2,
}
ini_impl!(Forest, File, "forest");

#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Ending {
    #[default]
    Unwatched = 0,
    Watched = 1,
}
ini_impl!(Ending, File, "ending");

#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum Intro {
    #[default]
    Unwatched = 0,
    Watched = 1,
}
ini_impl!(Intro, File, "intro");

#[derive(Clone, Debug)]
pub struct Lives(u32);
primitive_impl!(Lives, 0, 16777215, u32);
ini_impl!(Lives, File, "lives", u32);

impl Default for Lives {
    fn default() -> Self {
        Self(5)
    }
}

#[derive(Clone, Debug, Default)]
pub struct Gems(u8);
primitive_impl!(Gems, 0, 99, u8);
ini_impl!(Gems, File, "gems");

#[derive(Clone, Debug, Default)]
pub struct File {
    pub boss_no_damage: BossNoDamage,
    pub enemies_defeated: EnemiesDefeated,
    pub level: Level,
    pub ending: Ending,
    pub intro: Intro,
    pub lives: Lives,
    pub gems: Gems,
}

impl IniSectionStr for File {
    const INI_SECTION_STR: &'static str = "file";
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
            boss_no_damage: value.try_into()?,
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
        properties.insert(value.boss_no_damage.ini_key_str(), value.boss_no_damage);
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
