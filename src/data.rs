use ini::{Ini, Properties};
use num::cast::AsPrimitive;
use num::NumCast;
use num_enum::{TryFromPrimitive, TryFromPrimitiveError};
use std::num::{ParseFloatError, ParseIntError};
use thiserror::Error;

pub mod cheats;
pub mod file;
pub mod options;

pub use cheats::Cheats;
pub use file::File;
pub use options::Options;

#[derive(Clone, Debug)]
pub struct WhipseeySaveData {
    pub options: Options,
    pub files: [File; 3],
}

impl TryFrom<Ini> for WhipseeySaveData {
    type Error = Error;

    fn try_from(value: Ini) -> Result<Self> {
        Ok(WhipseeySaveData {
            options: try_from(&value)?,
            files: [
                try_from_n(&value, 1)?,
                try_from_n(&value, 2)?,
                try_from_n(&value, 3)?,
            ],
        })
    }
}

impl From<WhipseeySaveData> for Ini {
    fn from(value: WhipseeySaveData) -> Self {
        let mut ini = Ini::new();
        ini.entry(Some(value.options.ini_section_str().into()))
            .or_insert(value.options.into());
        let [file1, file2, file3] = value.files;
        ini.entry(Some(numbered_section::<File>(1)))
            .or_insert(file1.into());
        ini.entry(Some(numbered_section::<File>(2)))
            .or_insert(file2.into());
        ini.entry(Some(numbered_section::<File>(3)))
            .or_insert(file3.into());
        ini
    }
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct BfsSettings {
    pub cheats: Cheats,
}

impl TryFrom<Ini> for BfsSettings {
    type Error = Error;

    fn try_from(value: Ini) -> Result<Self> {
        Ok(BfsSettings {
            cheats: try_from(&value)?,
        })
    }
}

impl From<BfsSettings> for Ini {
    fn from(value: BfsSettings) -> Self {
        let mut ini = Ini::new();
        ini.entry(Some(value.cheats.ini_section_str().into()))
            .or_insert(value.cheats.into());
        ini
    }
}

fn try_from<'a, T>(value: &'a Ini) -> Result<T>
where
    T: IniSectionStr + TryFrom<&'a Properties, Error = Error>,
{
    value
        .section(Some(T::INI_SECTION_STR))
        .ok_or_else(|| Error::SectionMissing(T::INI_SECTION_STR.into()))?
        .try_into()
}
fn try_from_n<'a, T>(value: &'a Ini, n: usize) -> Result<T>
where
    T: IniSectionStr + TryFrom<&'a Properties, Error = Error>,
{
    let section = numbered_section::<T>(n);
    value
        .section(Some(&section))
        .ok_or(Error::SectionMissing(section))?
        .try_into()
}
fn numbered_section<T>(n: usize) -> String
where
    T: IniSectionStr,
{
    format!("{}{}", T::INI_SECTION_STR, n)
}

fn try_from_scaled<T, P>(value: &Properties, scale: f64) -> Result<T>
where
    T: IniKeyStr + TryFromPrimitive + TryFrom<P, Error = TryFromPrimitiveError<T>>,
    P: NumCast,
{
    let val_str = value
        .get(T::INI_KEY_STR)
        .ok_or(Error::KeyMissing(T::INI_KEY_STR.into()))?;
    let val: f64 = val_str.trim_matches('"').parse()?;
    let scaled = val * scale;
    let primitiv: P = num::cast(scaled).ok_or_else(|| {
        Error::NumCast(format!(
            "{} cannot be represented as {}",
            scaled,
            std::any::type_name::<P>()
        ))
    })?;
    let cheats_enabled = primitiv.try_into()?;
    Ok(cheats_enabled)
}

fn into_quoted_scaled_float<T, P>(value: T, scale: f64) -> String
where
    T: Into<P>,
    P: AsPrimitive<f64>,
{
    let primitiv: P = value.into();
    let double: f64 = primitiv.as_();
    let scaled = double / scale;
    format!("\"{:.6}\"", scaled)
}

trait IniSectionStr {
    const INI_SECTION_STR: &'static str;

    fn ini_section_str(&self) -> &'static str {
        Self::INI_SECTION_STR
    }
}

trait IniKeyStr: IniSectionStr {
    const INI_KEY_STR: &'static str;

    fn ini_key_str(&self) -> &'static str {
        Self::INI_KEY_STR
    }
}

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Error, Debug, Clone)]
pub enum Error {
    #[error("Section {0} not found")]
    SectionMissing(String),
    #[error("Key {0} not found")]
    KeyMissing(String),
    #[error(transparent)]
    ParseInt(#[from] ParseIntError),
    #[error(transparent)]
    ParseFloat(#[from] ParseFloatError),
    #[error("{0}")]
    NumCast(String),
    #[error("{0}")]
    IniParse(String),
    #[error("{0}")]
    Io(String),
    #[error("{0}")]
    TryFromPrimitive(String),
}

impl<Enum: TryFromPrimitive> From<TryFromPrimitiveError<Enum>> for Error {
    fn from(value: TryFromPrimitiveError<Enum>) -> Self {
        Self::TryFromPrimitive(value.to_string())
    }
}

impl From<ini::Error> for Error {
    fn from(value: ini::Error) -> Self {
        match value {
            ini::Error::Io(err) => Error::Io(err.to_string()),
            ini::Error::Parse(err) => Error::IniParse(err.to_string()),
        }
    }
}

#[macro_export]
macro_rules! ini_impl_common {
    ($self:ty, $section:ident, $key:literal, $scale:literal, $typ:ty) => {
        impl $self {
            pub fn value(&self) -> $typ {
                self.clone().into()
            }
        }
        impl $crate::data::IniSectionStr for $self {
            const INI_SECTION_STR: &'static str = $section::INI_SECTION_STR;
        }
        impl $crate::data::IniKeyStr for $self {
            const INI_KEY_STR: &'static str = $key;
        }
        impl TryFrom<&ini::Properties> for $self {
            type Error = $crate::data::Error;

            fn try_from(value: &ini::Properties) -> Result<Self, Self::Error> {
                $crate::data::try_from_scaled::<$self, $typ>(value, $scale)
            }
        }
    };

    ($self:ty, $section:ident, $key:literal) => {
        $crate::ini_impl_common!($self, $section, $key, 1.0, u8);
    };
}

#[macro_export]
macro_rules! ini_impl_quoted {
    ($self:ty, $section:ident, $key:literal, $scale:literal, $typ:ty) => {
        $crate::ini_impl_common!($self, $section, $key, $scale, $typ);
        impl From<$self> for String {
            fn from(value: $self) -> Self {
                $crate::data::into_quoted_scaled_float::<$self, $typ>(value, $scale)
            }
        }
    };

    ($self:ty, $section:ident, $key:literal, $scale:literal) => {
        $crate::ini_impl_quoted!($self, $section, $key, $scale, u8);
    };

    ($self:ty, $section:ident, $key:literal) => {
        $crate::ini_impl_quoted!($self, $section, $key, 1.0);
    };

    ($self:ty, $section:ident, $key:literal, $typ:ty) => {
        $crate::ini_impl_quoted!($self, $section, $key, 1.0, $typ);
    };
}

#[macro_export]
macro_rules! primitive_impl {
    ($self:ty, $min:literal, $max:expr, $typ:ty) => {
        impl $self {
            pub const MIN: $typ = $min;
            pub const MAX: $typ = $max as $typ;
            pub fn min(&self) -> $typ {
                Self::MIN
            }
            pub fn max(&self) -> $typ {
                Self::MAX
            }
        }
        impl ::num_enum::TryFromPrimitive for $self {
            type Primitive = $typ;
            type Error = ::num_enum::TryFromPrimitiveError<Self>;
            const NAME: &'static str = stringify!($self);
            fn try_from_primitive(
                number: Self::Primitive,
            ) -> ::core::result::Result<Self, ::num_enum::TryFromPrimitiveError<Self>> {
                #[deny(unreachable_patterns)]
                match number {
                    Self::MIN..Self::MAX => ::core::result::Result::Ok(Self(number)),
                    #[allow(unreachable_patterns)]
                    _ => ::core::result::Result::Err(
                        ::num_enum::TryFromPrimitiveError::<Self>::new(number),
                    ),
                }
            }
        }
        impl ::core::convert::TryFrom<$typ> for $self {
            type Error = ::num_enum::TryFromPrimitiveError<Self>;
            #[inline]
            fn try_from(
                number: $typ,
            ) -> ::core::result::Result<Self, ::num_enum::TryFromPrimitiveError<Self>> {
                ::num_enum::TryFromPrimitive::try_from_primitive(number)
            }
        }
        #[doc(hidden)]
        impl ::num_enum::CannotDeriveBothFromPrimitiveAndTryFromPrimitive for $self {}
        impl From<$self> for $typ {
            fn from(value: $self) -> Self {
                value.0
            }
        }
    };
}

#[cfg(test)]
mod test {
    use super::*;
    use crate::{assert_matches, util};
    use cheats::CheatsEnabled;

    #[test]
    fn bfs_settings_into() {
        let settings = BfsSettings {
            cheats: Cheats {
                cheats_enabled: CheatsEnabled::Disabled,
            },
        };
        let value: String = settings.cheats.cheats_enabled.clone().into();
        let ini: Ini = settings.into();
        assert_eq!(
            ini.get_from(Some(Cheats::INI_SECTION_STR), CheatsEnabled::INI_KEY_STR),
            Some(value.as_str())
        );
    }

    #[test]
    fn bfs_settings_default() {
        let ini = Ini::load_from_str(util::test::ini::DEFAULT).unwrap();
        let settings_loaded = BfsSettings::try_from(ini).unwrap();
        let settings_default = BfsSettings::default();
        assert_eq!(settings_loaded, settings_default);
    }

    #[test]
    fn bfs_settings_try_from_valid() {
        let ini = Ini::load_from_str(util::test::ini::VALID).unwrap();
        let settings = BfsSettings::try_from(ini).unwrap();
        assert_eq!(settings.cheats.cheats_enabled, CheatsEnabled::Enabled);
    }

    #[test]
    fn bfs_settings_try_from_lenient() {
        let ini = Ini::load_from_str(util::test::ini::LENIENT_VALUES).unwrap();
        let settings = BfsSettings::try_from(ini).unwrap();
        assert_eq!(settings.cheats.cheats_enabled, CheatsEnabled::Enabled);
    }

    #[test]
    fn bfs_settings_try_from_invalid_sections() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_SECTIONS).unwrap();
        let error = BfsSettings::try_from(ini).unwrap_err();
        assert_matches!(error, Error::SectionMissing(section) if section == Cheats::INI_SECTION_STR);
    }

    #[test]
    fn bfs_settings_try_from_invalid_keys() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_KEYS).unwrap();
        let error = BfsSettings::try_from(ini).unwrap_err();
        assert_matches!(error, Error::KeyMissing(key) if key == CheatsEnabled::INI_KEY_STR);
    }

    #[test]
    fn bfs_settings_try_from_invalid_value_ranges() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_RANGES).unwrap();
        let error = BfsSettings::try_from(ini).unwrap_err();
        assert_matches!(error, Error::TryFromPrimitive(_));
    }

    #[test]
    fn bfs_settings_try_from_invalid_value_types() {
        let ini = Ini::load_from_str(util::test::ini::INVALID_VALUE_TYPES).unwrap();
        let error = BfsSettings::try_from(ini).unwrap_err();
        assert_matches!(
            error,
            Error::NumCast(_) | Error::ParseInt(_) | Error::ParseFloat(_)
        );
    }
}
