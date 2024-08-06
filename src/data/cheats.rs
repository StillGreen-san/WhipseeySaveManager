use num_enum::TryFromPrimitive;

#[derive(Debug, Default, Eq, PartialEq, TryFromPrimitive)]
#[repr(u8)]
pub enum CheatsEnabled {
    #[default]
    Disabled = 0,
    Enabled = 1,
}

#[derive(Default)]
pub struct Cheats {
    pub cheats_enabled: CheatsEnabled,
}
