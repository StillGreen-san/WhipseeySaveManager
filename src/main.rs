#![windows_subsystem = "windows"]

use iced::{window, Pixels, Settings, Size};

use whipseey_save_manager::gui;

fn main() -> Result<(), iced::Error> {
    let window = window::Settings {
        size: Size::from([720.0, 334.0]),
        position: Default::default(),
        min_size: None,
        max_size: None,
        visible: true,
        resizable: false,
        decorations: true,
        transparent: false,
        level: Default::default(),
        icon: window::icon::from_file_data(include_bytes!("../res/icon.ico"), None).ok(),
        exit_on_close_request: true,
        platform_specific: Default::default(),
    };
    let settings = Settings {
        id: Some("WhipseeySaveManager".into()),
        fonts: Vec::new(),
        default_font: Default::default(),
        default_text_size: Pixels(16.0),
        antialiasing: false,
    };
    iced::application("Whipseey Save Manager", gui::Gui::update, gui::Gui::view)
        .settings(settings)
        .window(window)
        .theme(gui::Gui::theme)
        .font(iced_aw::iced_fonts::REQUIRED_FONT_BYTES)
        .run_with(gui::Gui::new)
}
