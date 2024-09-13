use num_enum::{TryFromPrimitive, TryFromPrimitiveError};
use std::env::VarError;
use std::num::{ParseFloatError, ParseIntError};
use std::path::PathBuf;
use steamlocate::error::ParseErrorKind;
use thiserror::Error;

pub type Result<T> = std::result::Result<T, Error>;

/// catch all error type
///
/// provides clone ability for various error types that would otherwise not be
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
    #[error(transparent)]
    LocateError(#[from] LocateError),
    #[error(transparent)]
    VarError(#[from] VarError),
} // TODO refactor & improve user facing messages

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

impl From<std::io::Error> for Error {
    fn from(value: std::io::Error) -> Self {
        Error::Io(value.to_string())
    }
}

/// cloneable alternative to [steamlocate::Error]
#[derive(Error, Debug, Clone)]
pub enum LocateError {
    #[error("{0}")]
    FailedLocate(steamlocate::error::LocateError),
    #[error("{0}")]
    InvalidSteamDir(steamlocate::error::ValidationError),
    #[error("Encountered an I/O error: {inner} at {}", path.display())]
    Io { inner: String, path: PathBuf },
    #[error("Failed parsing VDF file. File kind: {kind:?}, Error: {error} at {}", path.display())]
    Parse {
        kind: ParseErrorKind,
        error: String,
        path: PathBuf,
    },
    #[error("Missing expected app with id: {app_id}")]
    MissingExpectedApp { app_id: u32 },
}

impl From<steamlocate::Error> for LocateError {
    fn from(value: steamlocate::Error) -> Self {
        match value {
            steamlocate::Error::FailedLocate(locate_error) => {
                LocateError::FailedLocate(locate_error)
            }
            steamlocate::Error::InvalidSteamDir(validation_error) => {
                LocateError::InvalidSteamDir(validation_error)
            }
            steamlocate::Error::Io { inner, path } => LocateError::Io {
                inner: inner.to_string(),
                path,
            },
            steamlocate::Error::Parse { kind, error, path } => LocateError::Parse {
                kind,
                error: error.to_string(),
                path,
            },
            steamlocate::Error::MissingExpectedApp { app_id } => {
                LocateError::MissingExpectedApp { app_id }
            }
            _ => unimplemented!(),
        }
    }
}
