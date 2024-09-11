use crate::data::{IniKeyStr, IniSectionStrVal};
use crate::{data, ini_impl_common};
use ini::Properties;
use num_enum::{IntoPrimitive, TryFromPrimitive};

/// represents the `cheats_enabled` Ini property in the `Cheats` section in `bfs_settings.ini`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq, TryFromPrimitive, IntoPrimitive)]
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

/// represents the `Cheats` Ini section in `bfs_settings.ini`
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct Cheats {
    pub cheats_enabled: CheatsEnabled,
}

impl IniSectionStrVal for Cheats {
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::{assert_matches, util};
    use ini::Ini;

    #[test]
    fn cheats_into_properties() {
        let cheats = Cheats {
            cheats_enabled: CheatsEnabled::Enabled,
        };
        let props: Properties = cheats.into();
        assert_eq!(
            props.get(CheatsEnabled::INI_KEY_STR),
            Some(String::from(cheats.cheats_enabled).as_str())
        );
    }

    #[test]
    fn cheats_default() {
        let ini = Ini::load_from_str(util::test::ini::DEFAULT).unwrap();
        let section = ini.section(Some(Cheats::INI_SECTION_STR)).unwrap();
        let cheats_loaded = Cheats::try_from(section).unwrap();
        let cheats_default = Cheats::default();
        assert_eq!(cheats_loaded, cheats_default);
    }

    #[test]
    fn cheats_try_from_properties_valid() {
        let ini = Ini::load_from_str(util::test::ini::VALID).unwrap();
        let section = ini.section(Some(Cheats::INI_SECTION_STR)).unwrap();
        let cheats = Cheats::try_from(section).unwrap();
        assert_eq!(cheats.cheats_enabled, CheatsEnabled::Enabled);
    }

    #[test]
    fn cheats_try_from_properties_lenient() {
        let ini = Ini::load_from_str(util::test::ini::LENIENT_VALUES).unwrap();
        let section = ini.section(Some(Cheats::INI_SECTION_STR)).unwrap();
        let cheats = Cheats::try_from(section).unwrap();
        assert_eq!(cheats.cheats_enabled, CheatsEnabled::Enabled);
    }

    #[test]
    fn cheats_try_from_properties_invalid_keys() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_KEYS).unwrap();
        let section = ini.section(Some(Cheats::INI_SECTION_STR)).unwrap();
        let error = Cheats::try_from(section).unwrap_err();
        assert_matches!(
            error,
            data::Error::KeyMissing(key) if key == CheatsEnabled::INI_KEY_STR
        );
    }

    #[test]
    fn cheats_try_from_properties_invalid_value_ranges() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_RANGES).unwrap();
        let section = ini.section(Some(Cheats::INI_SECTION_STR)).unwrap();
        let error = Cheats::try_from(section).unwrap_err();
        assert_matches!(error, data::Error::TryFromPrimitive(_));
    }

    #[test]
    fn cheats_try_from_properties_invalid_value_types() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_TYPES).unwrap();
        let section = ini.section(Some(Cheats::INI_SECTION_STR)).unwrap();
        let error = Cheats::try_from(section).unwrap_err();
        assert_matches!(
            error,
            data::Error::NumCast(_) | data::Error::ParseInt(_) | data::Error::ParseFloat(_)
        );
    }
}
