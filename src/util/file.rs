//! utilities to work with the file system and paths

use crate::util::for_each_window_mut;
use crate::{BFS_SETTINGS_FILE_NAME, SAVEGAME_FILE_NAME, WHIPSEEY_APP_ID};
use ini::Ini;
use std::cmp::max;
use std::env::VarError;
use std::path::Path;
use std::path::PathBuf;
use steamlocate::error::ParseErrorKind;
use steamlocate::SteamDir;
use thiserror::Error;

/// tries to load an INI file at `path` and tries to parse it into an [Ini] struct
///
/// trailing null bytes are removed before parsing
pub async fn load_ini_file(path: impl AsRef<Path>) -> Result<Ini, ini::Error> {
    parse_ini_file(tokio::fs::read_to_string(path).await?)
}

/// tries to parse `content` into an [Ini] struct
///
/// trailing null bytes are removed before parsing
fn parse_ini_file(mut content: String) -> Result<Ini, ini::Error> {
    content.truncate(content.find('\0').unwrap_or(content.len()));
    let opt = ini::ParseOption {
        enabled_quote: true,
        ..Default::default()
    };
    Ini::load_from_str_opt(&content, opt).map_err(ini::Error::Parse)
}

pub async fn write_ini_file(path: impl AsRef<Path>, ini: &Ini) -> std::io::Result<()> {
    let mut content = Vec::new();
    ini.write_to(&mut content)?;
    tokio::fs::write(path, &content).await
}

pub async fn write_ini_file_padded(path: impl AsRef<Path>, ini: &Ini) -> std::io::Result<()> {
    let mut content = Vec::new();
    content.reserve_exact(1024);
    let opt = ini::WriteOption {
        line_separator: ini::LineSeparator::CRLF,
        kv_separator: "=",
        ..Default::default()
    };
    ini.write_to_opt(&mut content, opt)?;
    for_each_window_mut(&mut content, 4, |window| {
        if window == b"\r\n\r\n" {
            window[0] = 0;
            window[1] = 0;
        }
    });
    content.retain(|&c| c != 0);
    content.resize(max(1024, content.len()), 0); // not expected to be > 1kb
    tokio::fs::write(path, &content).await
}

pub async fn find_savegame_path() -> Result<Option<PathBuf>, VarError> {
    let local_appdata = std::env::var("LOCALAPPDATA")?;
    let mut path = PathBuf::from(local_appdata);
    path.push("Whipseey");
    path.push("savedata");
    path.push(SAVEGAME_FILE_NAME);
    Ok(path.exists().then_some(path)) // TODO non windows support
}

pub async fn find_bfs_settings_path() -> Result<Option<PathBuf>, LocateError> {
    Ok(SteamDir::locate()?
        .find_app(WHIPSEEY_APP_ID)?
        .map(|(app, lib)| {
            let mut path = lib.resolve_app_dir(&app);
            path.push(BFS_SETTINGS_FILE_NAME);
            path
        }))
}

pub fn trim_to_existing_path(path: &Path) -> &Path {
    let mut path = if path.is_file() {
        path.parent().unwrap_or(path)
    } else {
        path
    };
    while !path.exists() {
        if let Some(parent) = path.parent() {
            path = parent;
        } else {
            path = path.strip_prefix(path).unwrap_or(path);
            break;
        };
    }
    path
}

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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::assert_matches;

    #[test]
    fn parse_ini_file_valid() {
        let content = "[section]\nkey = \"value\"".to_string();
        let ini = parse_ini_file(content).unwrap();
        assert_eq!(
            ini.section(Some("section")).unwrap().get("key").unwrap(),
            "value"
        );
    }

    #[test]
    fn parse_ini_file_padded() {
        let content = "\nkey = \"value\"\0\0\0".to_string();
        let ini = parse_ini_file(content).unwrap();
        assert_eq!(ini.general_section().get("key").unwrap(), "value");
    }

    #[test]
    fn parse_ini_file_invalid() {
        let content = "just a sentence".to_string();
        let result = parse_ini_file(content);
        assert_matches!(result, Err(ini::Error::Parse { .. }));
    }
}
