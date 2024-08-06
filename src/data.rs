use std::path::PathBuf;

pub use cheats::Cheats;

pub mod cheats;

pub struct BfsSettings {
    path: PathBuf,
    cheats: Cheats,
}
