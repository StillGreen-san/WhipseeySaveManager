use crate::data::{
    try_into_or_default_and_collect, FromPropsOrDefaulted, IniKeyStrFn, IniSectionStrVal,
};
use crate::{ini_impl_common, util};
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

impl FromPropsOrDefaulted for Cheats {
    type Error = util::Error;

    fn from_props_or_defaulted(value: &Properties) -> (Self, Vec<Self::Error>) {
        let mut errors = Vec::new();
        (
            Self {
                cheats_enabled: try_into_or_default_and_collect(value, &mut errors),
            },
            errors,
        )
    }
}

impl TryFrom<&Properties> for Cheats {
    type Error = util::Error;

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
    use crate::data::IniKeyStrVal;
    use crate::{assert_matches, TEST_FAIL_STR};
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
        let ini = Ini::load_from_str(util::test::ini::DEFAULT).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let cheats_loaded = Cheats::try_from(section).expect(TEST_FAIL_STR);
        let cheats_default = Cheats::default();
        assert_eq!(cheats_loaded, cheats_default);
    }

    #[test]
    fn cheats_try_from_properties_valid() {
        let ini = Ini::load_from_str(util::test::ini::VALID).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let cheats = Cheats::try_from(section).expect(TEST_FAIL_STR);
        assert_eq!(cheats.cheats_enabled, CheatsEnabled::Enabled);
    }

    #[test]
    fn cheats_from_properties_valid() {
        let ini = Ini::load_from_str(util::test::ini::VALID).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let (cheats, errors) = Cheats::from_props_or_defaulted(section);
        assert!(errors.is_empty());
        assert_eq!(cheats.cheats_enabled, CheatsEnabled::Enabled);
    }

    #[test]
    fn cheats_try_from_properties_lenient() {
        let ini = Ini::load_from_str(util::test::ini::LENIENT_VALUES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let cheats = Cheats::try_from(section).expect(TEST_FAIL_STR);
        assert_eq!(cheats.cheats_enabled, CheatsEnabled::Enabled);
    }

    #[test]
    fn cheats_from_properties_lenient() {
        let ini = Ini::load_from_str(util::test::ini::LENIENT_VALUES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let (cheats, errors) = Cheats::from_props_or_defaulted(section);
        assert!(errors.is_empty());
        assert_eq!(cheats.cheats_enabled, CheatsEnabled::Enabled);
    }

    #[test]
    fn cheats_try_from_properties_invalid_keys() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_KEYS).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let error = Cheats::try_from(section).expect_err(TEST_FAIL_STR);
        assert_matches!(
            error,
            util::Error::KeyMissing(key) if key == CheatsEnabled::INI_KEY_STR
        );
    }

    #[test]
    fn cheats_from_properties_invalid_keys() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_KEYS).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let (cheats, errors) = Cheats::from_props_or_defaulted(section);
        assert_eq!(cheats, Default::default());
        for error in errors {
            assert_matches!(
                error,
                util::Error::KeyMissing(key) if key == CheatsEnabled::INI_KEY_STR
            );
        }
    }

    #[test]
    fn cheats_try_from_properties_invalid_value_ranges() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_RANGES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let error = Cheats::try_from(section).expect_err(TEST_FAIL_STR);
        assert_matches!(error, util::Error::TryFromPrimitive(_));
    }

    #[test]
    fn cheats_from_properties_invalid_value_ranges() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_RANGES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let (cheats, errors) = Cheats::from_props_or_defaulted(section);
        assert_eq!(cheats, Default::default());
        for error in errors {
            assert_matches!(error, util::Error::TryFromPrimitive(_));
        }
    }

    #[test]
    fn cheats_try_from_properties_invalid_value_types() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_TYPES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let error = Cheats::try_from(section).expect_err(TEST_FAIL_STR);
        assert_matches!(
            error,
            util::Error::NumCast(_) | util::Error::ParseInt(_) | util::Error::ParseFloat(_)
        );
    }

    #[test]
    fn cheats_from_properties_invalid_value_types() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_TYPES).expect(TEST_FAIL_STR);
        let section = ini
            .section(Some(Cheats::INI_SECTION_STR))
            .expect(TEST_FAIL_STR);
        let (cheats, errors) = Cheats::from_props_or_defaulted(section);
        assert_eq!(cheats, Default::default());
        for error in errors {
            assert_matches!(
                error,
                util::Error::NumCast(_) | util::Error::ParseInt(_) | util::Error::ParseFloat(_)
            );
        }
    }
}
