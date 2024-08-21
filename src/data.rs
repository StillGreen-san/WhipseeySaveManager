use crate::data::cheats::CheatsEnabled;
use ini::Ini;
use num_enum::{TryFromPrimitive, TryFromPrimitiveError};
use std::num::ParseIntError;
use thiserror::Error;

pub mod cheats;
pub mod options;

pub use cheats::Cheats;
pub use options::Options;

pub mod cheats;

#[derive(Clone, Debug)]
pub struct BfsSettings {
    pub cheats: Cheats,
}

impl TryFrom<Ini> for BfsSettings {
    type Error = Error;

    fn try_from(value: Ini) -> Result<Self> {
        Ok(BfsSettings {
            cheats: Cheats {
                cheats_enabled: CheatsEnabled::try_from(&value)?,
            },
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
    ParseIntError(#[from] ParseIntError),
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
