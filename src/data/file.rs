use enumflags2::{bitflags, BitFlags};
use strum::{Display, VariantArray};

#[bitflags]
#[repr(u8)]
#[derive(Copy, Clone, Debug)]
pub enum BossNoDamageFlags {
    Forest = 1,
    Desert = 2,
    Castle = 4,
}
type BossNoDamage = BitFlags<BossNoDamageFlags>;

#[derive(Clone, Debug, Default)]
pub struct EnemiesDefeated(pub u32);

#[derive(Clone, Copy, Debug, Default, Display, Eq, PartialEq, VariantArray)]
pub enum Level {
    Castle = 32,
    Moon = 16,
    Snow = 8,
    Desert = 4,
    Forest = 2,
    #[default]
    Beach = 1,
}

#[derive(Clone, Debug, Default)]
pub enum Castle {
    #[default]
    Locked = 0,
    Unlocked = 32,
}

#[derive(Clone, Debug, Default)]
pub enum Moon {
    #[default]
    Locked = 0,
    Unlocked = 16,
}

#[derive(Clone, Debug, Default)]
pub enum Snow {
    #[default]
    Locked = 0,
    Unlocked = 8,
}

#[derive(Clone, Debug, Default)]
pub enum Desert {
    #[default]
    Locked = 0,
    Unlocked = 4,
}

#[derive(Clone, Debug, Default)]
pub enum Forest {
    #[default]
    Locked = 0,
    Unlocked = 2,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub enum Ending {
    #[default]
    Unwatched = 0,
    Watched = 1,
}
#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub enum Intro {
    #[default]
    Unwatched = 0,
    Watched = 1,
}

#[derive(Clone, Debug)]
pub struct Lives(pub u32);

impl Default for Lives {
    fn default() -> Self {
        Self(5)
    }
}

#[derive(Clone, Debug, Default)]
pub struct Gems(pub u8);

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
