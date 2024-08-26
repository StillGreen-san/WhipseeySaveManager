use iced::{window, Application, Pixels, Settings, Size};

pub mod data;
mod gui;
mod util;

fn main() -> Result<(), iced::Error> {
    let window = window::Settings {
        size: Size::from([678, 342]),
        position: Default::default(),
        min_size: None,
        max_size: None,
        visible: true,
        resizable: true,
        decorations: true,
        transparent: false,
        level: Default::default(),
        icon: None,
        exit_on_close_request: true,
        platform_specific: Default::default(),
    };
    let settings = Settings {
        id: Some("WhipseeySaveManager".into()),
        window: window,
        flags: Default::default(),
        fonts: Vec::new(),
        default_font: Default::default(),
        default_text_size: Pixels(16.0),
        antialiasing: false,
    };
    gui::Gui::run(settings)
}
