use ini::Ini;
use std::cmp::max;
use std::path::Path;

pub async fn load_ini_file(path: impl AsRef<Path>) -> Result<Ini, ini::Error> {
    tokio::fs::read_to_string(path)
        .await
        .map_err(ini::Error::Io)
        .and_then(|content| Ini::load_from_str(&content).map_err(ini::Error::Parse))
}

pub async fn write_ini_file(path: impl AsRef<Path>, ini: &Ini) -> std::io::Result<()> {
    let mut content = Vec::new();
    content.reserve_exact(1024);
    ini.write_to(&mut content)?;
    content.resize(max(1024, content.len()), 0); // not expected to be > 1kb
    tokio::fs::write(path, &content).await
}
