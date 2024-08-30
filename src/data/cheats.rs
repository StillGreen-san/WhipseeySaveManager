use crate::data::{IniKeyStr, IniSectionStr};
use crate::{data, ini_impl_common};
use ini::Properties;
use num_enum::{IntoPrimitive, TryFromPrimitive};

#[derive(Clone, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
#[repr(u8)]
pub enum CheatsEnabled {
    #[default]
    Disabled = 0,
    Enabled = 1,
}
ini_impl_common!(CheatsEnabled, Cheats, "cheats_enabled");
impl From<CheatsEnabled> for String {
    fn from(value: CheatsEnabled) -> Self {
        u8::from(value).to_string()
    }
}

#[derive(Clone, Debug, Default)]
pub struct Cheats {
    pub cheats_enabled: CheatsEnabled,
}

impl IniSectionStr for Cheats {
    const INI_SECTION_STR: &'static str = "Cheats";
}

impl TryFrom<&Properties> for Cheats {
    type Error = data::Error;

    fn try_from(value: &Properties) -> Result<Self, Self::Error> {
        Ok(Self {
            cheats_enabled: value.try_into()?,
        })
    }
}

impl From<Cheats> for Properties {
    fn from(value: Cheats) -> Self {
        let mut props = Properties::new();
        props.insert(value.cheats_enabled.ini_key_str(), value.cheats_enabled);
        props
    }
}
