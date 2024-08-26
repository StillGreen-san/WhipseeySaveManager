use crate::util::for_each_window_mut;
use ini::Ini;
use std::cmp::max;
use std::path::Path;

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
