use std::path::PathBuf;
use crate::data::cheats::CheatsEnabled;
use num_enum::{TryFromPrimitive, TryFromPrimitiveError};
use std::num::ParseIntError;
use thiserror::Error;

pub use cheats::Cheats;

pub mod cheats;

pub struct BfsSettings {
    path: PathBuf,
    cheats: Cheats,
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
    IniError(String),
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
            ini::Error::Io(err) => Error::IniError(err.to_string()),
            ini::Error::Parse(err) => Error::IniError(err.to_string()),
        }
    }
}
