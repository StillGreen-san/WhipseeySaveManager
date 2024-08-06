#[derive(Debug)]
pub enum CheatsEnabled {
    Disabled = 0,
    Enabled = 1,
}

pub struct Cheats {
    cheats_enabled: CheatsEnabled,
}
