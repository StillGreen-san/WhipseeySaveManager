use iced::{window, Application, Pixels, Settings, Size};

pub mod data;
mod gui;
mod util;

const WHIPSEEY_APP_ID: u32 = 1034920;
const BFS_SETTINGS_FILE_NAME: &'static str = "bfs_settings.ini";
const SAVEGAME_FILE_NAME: &'static str = "whipseey.sav";

fn main() -> Result<(), iced::Error> {
    let window = window::Settings {
        size: Size::from([720, 334]),
        position: Default::default(),
        min_size: None,
        max_size: None,
        visible: true,
        resizable: false,
        decorations: true,
        transparent: false,
        level: Default::default(),
        icon: None,
        exit_on_close_request: true,
        platform_specific: Default::default(),
    };
    let settings = Settings {
        id: Some("WhipseeySaveManager".into()),
        window,
        flags: Default::default(),
        fonts: Vec::new(),
        default_font: Default::default(),
        default_text_size: Pixels(16.0),
        antialiasing: false,
    };
    gui::Gui::run(settings)
}
