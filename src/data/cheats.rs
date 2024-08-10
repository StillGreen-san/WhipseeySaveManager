use crate::data;
use ini::Ini;
use num_enum::TryFromPrimitive;

#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive)]
#[repr(u8)]
pub enum CheatsEnabled {
    #[default]
    Disabled = 0,
    Enabled = 1,
}

#[derive(Clone, Debug, Default)]
pub struct Cheats {
    pub cheats_enabled: CheatsEnabled,
}

impl TryFrom<&Ini> for CheatsEnabled {
    type Error = data::Error;

    fn try_from(value: &Ini) -> Result<Self, Self::Error> {
        let section = value
            .section(Some("Cheats"))
            .ok_or(data::Error::SectionMissing("Cheats".into()))?;
        let val_str = section
            .get("cheats_enabled")
            .ok_or(data::Error::KeyMissing("cheats_enabled".into()))?;
        let val: u8 = val_str.parse()?;
        let cheats_enabled = val.try_into()?;
        Ok(cheats_enabled)
    }
}
