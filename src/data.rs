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

pub struct WhipseeySaveData {
    pub options: Options,
    pub files: [File; 3],
}

#[derive(Clone, Debug)]
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
    let section = format!("{}{}", T::INI_SECTION_STR, n);
    value
        .section(Some(&section))
        .ok_or(Error::SectionMissing(section))?
        .try_into()
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

fn into_quoted_scaled<T, P>(value: T, scale: f64) -> String
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
macro_rules! ini_impl {
    ($self:ty, $section:ident, $key:literal, $scale:literal, $typ:ty) => {
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

        impl From<$self> for String {
            fn from(value: $self) -> Self {
                $crate::data::into_quoted_scaled::<$self, $typ>(value, $scale)
            }
        }
    };

    ($self:ty, $section:ident, $key:literal, $scale:literal) => {
        $crate::ini_impl!($self, $section, $key, $scale, u8);
    };

    ($self:ty, $section:ident, $key:literal) => {
        $crate::ini_impl!($self, $section, $key, 1.0);
    };

    ($self:ty, $section:ident, $key:literal, $typ:ty) => {
        $crate::ini_impl!($self, $section, $key, 1.0, $typ);
    };
}

#[macro_export]
macro_rules! primitive_impl {
    ($self:ty, $min:literal, $max:expr, $typ:ty) => {
        impl $self {
            const MIN: $typ = $min;
            const MAX: $typ = $max as $typ;
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
