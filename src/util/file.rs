use crate::util::for_each_window_mut;
use crate::{BFS_SETTINGS_FILE_NAME, WHIPSEEY_APP_ID};
use ini::Ini;
use std::cmp::max;
use std::path::Path;
use std::path::PathBuf;
use steamlocate::error::ParseErrorKind;
use steamlocate::SteamDir;
use thiserror::Error;

pub async fn load_ini_file(path: impl AsRef<Path>) -> Result<Ini, ini::Error> {
    tokio::fs::read_to_string(path)
        .await
        .map(|mut content| {
            content.truncate(content.find('\0').unwrap_or(content.len()));
            content
        })
        .map_err(ini::Error::Io)
        .and_then(|content| Ini::load_from_str(&content).map_err(ini::Error::Parse))
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

pub async fn find_bfs_settings_path() -> Result<Option<PathBuf>, LocateError> {
    Ok(SteamDir::locate()?
        .find_app(WHIPSEEY_APP_ID)?
        .map(|(app, lib)| {
            let mut path = lib.resolve_app_dir(&app);
            path.push(BFS_SETTINGS_FILE_NAME);
            path
        }))
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
