use iced::{window, Application, Pixels, Settings, Size};

use whipseey_save_manager::gui;

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
