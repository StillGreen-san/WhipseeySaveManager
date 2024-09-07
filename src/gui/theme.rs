use iced::theme::Palette;

pub type Theme = iced::Theme;

#[derive(Debug, Clone)]
pub enum ThemeStore {
    Light(Theme),
    Dark(Theme),
}
use ThemeStore::Dark;
use ThemeStore::Light;

pub fn light() -> ThemeStore {
    let iced_light_palette = iced::Theme::Light.palette();
    let whipseey_light_palette = Palette {
        background: iced_light_palette.background,
        text: iced_light_palette.text,
        primary: iced_light_palette.primary,
        success: iced_light_palette.success,
        danger: iced_light_palette.danger,
    };
    Light(iced::Theme::custom(
        "WhipseeyLight".into(),
        whipseey_light_palette,
    ))
}

pub fn dark() -> ThemeStore {
    let iced_dark_palette = iced::Theme::Dark.palette();
    let whipseey_dark_palette = Palette {
        background: iced_dark_palette.background,
        text: iced_dark_palette.text,
        primary: iced_dark_palette.primary,
        success: iced_dark_palette.success,
        danger: iced_dark_palette.danger,
    };
    Dark(iced::Theme::custom(
        "WhipseeyDark".into(),
        whipseey_dark_palette,
    ))
}
