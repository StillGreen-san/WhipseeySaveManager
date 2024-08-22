use enumflags2::{bitflags, BitFlags};

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
pub struct EnemiesDefeated(u32);

#[derive(Clone, Debug, Default)]
pub enum Level {
    #[default]
    Beach = 1,
    Forest = 2,
    Desert = 4,
    Snow = 8,
    Moon = 16,
    Castle = 32,
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

#[derive(Clone, Debug, Default)]
pub enum Ending {
    #[default]
    Unwatched = 0,
    Watched = 1,
}
#[derive(Clone, Debug, Default)]
pub enum Intro {
    #[default]
    Unwatched = 0,
    Watched = 1,
}

#[derive(Clone, Debug)]
pub struct Lives(u32);

impl Default for Lives {
    fn default() -> Self {
        Self(5)
    }
}

#[derive(Clone, Debug, Default)]
pub struct Gems(u8);

#[derive(Clone, Debug, Default)]
pub struct File {
    boss_no_damage: BossNoDamage,
    enemies_defeated: EnemiesDefeated,
    level: Level,
    ending: Ending,
    intro: Intro,
    lives: Lives,
    gems: Gems,
}
