use crate::data;
use ini::Ini;
use crate::data::{IniKeyStr, IniSectionStr};
use num_enum::TryFromPrimitive;

#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive)]
#[repr(u8)]
pub enum CheatsEnabled {
    #[default]
    Disabled = 0,
    Enabled = 1,
}

impl IniSectionStr for CheatsEnabled {
    const INI_SECTION_STR: &'static str = Cheats::INI_SECTION_STR;
}

impl IniKeyStr for CheatsEnabled {
    const INI_KEY_STR: &'static str = "cheats_enabled";
}

#[derive(Clone, Debug, Default)]
pub struct Cheats {
    pub cheats_enabled: CheatsEnabled,
}

impl IniSectionStr for Cheats {
    const INI_SECTION_STR: &'static str = "Cheats";
}

impl TryFrom<&Ini> for CheatsEnabled {
    type Error = data::Error;

    fn try_from(value: &Ini) -> Result<Self, Self::Error> {
        let section = value
            .section(Some(Self::INI_SECTION_STR))
            .ok_or(data::Error::SectionMissing(Self::INI_SECTION_STR.into()))?;
        let val_str = section
            .get(Self::INI_KEY_STR)
            .ok_or(data::Error::KeyMissing(Self::INI_KEY_STR.into()))?;
        let val: u8 = val_str.parse()?;
        let cheats_enabled = val.try_into()?;
        Ok(cheats_enabled)
    }
}
