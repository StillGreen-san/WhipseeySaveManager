use iced::theme::Palette;
use iced::widget::{button as iced_button, text_input as iced_text_input};
use iced::{color, Border, Color};
use iced_aw::style as iced_style;
use iced_aw::{card as iced_card, tab_bar as iced_tab_bar};

use iced::theme::palette::Extended;
pub use iced::Theme;

const WHIPSEEY_LOGO_PINK: Color = color!(0xE977A8);
const WHIPSEEY_LIGHT_PALETTE: Palette = Palette {
    background: Palette::LIGHT.background,
    text: Palette::LIGHT.text,
    primary: WHIPSEEY_LOGO_PINK,
    success: Palette::LIGHT.success,
    danger: Palette::LIGHT.danger,
};
const WHIPSEEY_DARK_PALETTE: Palette = Palette {
    background: Palette::DARK.background,
    text: Palette::DARK.text,
    primary: WHIPSEEY_LOGO_PINK,
    success: Palette::DARK.success,
    danger: Palette::DARK.danger,
};

pub fn default() -> Theme {
    match Theme::default() {
        Theme::Dark => dark(),
        _ => light(),
    }
}

pub fn light() -> Theme {
    Theme::custom_with_fn(
        "WhipseeyLight".into(),
        WHIPSEEY_LIGHT_PALETTE,
        generate_extended,
    )
}

pub fn dark() -> Theme {
    Theme::custom_with_fn(
        "WhipseeyDark".into(),
        WHIPSEEY_DARK_PALETTE,
        generate_extended,
    )
}

fn generate_extended(palette: Palette) -> Extended {
    let mut extended = Extended::generate(palette);
    extended.primary.strong.text = Color::WHITE;
    extended.primary.base.text = Color::WHITE;
    extended.primary.weak.text = Color::WHITE;
    extended
}

#[cfg(debug_assertions)]
pub fn cycle(theme: &Theme) -> Theme {
    if theme.to_string() == "WhipseeyLight" {
        dark()
    } else {
        light()
    }
}

pub fn tab_bar(theme: &Theme, status: iced_style::Status) -> iced_tab_bar::Style {
    let base = iced_style::tab_bar::primary(theme, status);
    let palette = theme.extended_palette();

    match status {
        iced_style::Status::Selected => iced_tab_bar::Style { ..base },
        iced_style::Status::Active => iced_tab_bar::Style {
            tab_label_background: palette.primary.strong.color.into(),
            text_color: palette.primary.strong.text,
            ..base
        },
        iced_style::Status::Hovered => iced_tab_bar::Style {
            tab_label_background: palette.primary.base.color.into(),
            text_color: palette.primary.base.text,
            ..base
        },
        iced_style::Status::Pressed => iced_tab_bar::Style { ..base },
        iced_style::Status::Disabled => iced_tab_bar::Style {
            tab_label_background: palette.background.weak.color.into(),
            text_color: palette.background.weak.text,
            ..base
        },
        iced_style::Status::Focused => base,
    }
}

pub fn text_input(theme: &Theme, status: iced_text_input::Status) -> iced_text_input::Style {
    let palette = theme.extended_palette();

    let base = iced_text_input::default(theme, status);

    match status {
        iced_text_input::Status::Active => iced_text_input::Style {
            border: Border {
                color: palette.primary.weak.color,
                ..base.border
            },
            ..base
        },
        iced_text_input::Status::Hovered => iced_text_input::Style {
            border: Border {
                color: palette.primary.base.color,
                ..base.border
            },
            ..base
        },
        iced_text_input::Status::Focused => iced_text_input::Style {
            border: Border {
                width: 2.0,
                ..base.border
            },
            ..base
        },
        iced_text_input::Status::Disabled => base,
    }
}

pub fn button_link(theme: &Theme, status: iced_button::Status) -> iced_button::Style {
    let palette = theme.extended_palette();
    let base = iced_button::text(theme, status);
    match status {
        iced_button::Status::Active => iced_button::Style {
            text_color: palette.primary.strong.color,
            ..base
        },
        iced_button::Status::Hovered => iced_button::Style {
            text_color: palette.primary.base.color,
            ..base
        },
        iced_button::Status::Pressed => iced_button::Style {
            text_color: palette.primary.weak.color,
            ..base
        },
        iced_button::Status::Disabled => iced_button::Style {
            text_color: palette.primary.strong.color.scale_alpha(0.5),
            ..base
        },
    }
}

pub fn card_primary(theme: &Theme, status: iced_style::Status) -> iced_card::Style {
    let palette = theme.extended_palette();
    let base = iced_style::card::primary(theme, status);
    match status {
        iced_style::Status::Active => iced_card::Style {
            border_color: palette.primary.strong.color,
            head_background: palette.primary.strong.color.into(),
            head_text_color: palette.primary.strong.text,
            ..base
        },
        _ => base,
    }
}
pub fn card_secondary(theme: &Theme, status: iced_style::Status) -> iced_card::Style {
    let palette = theme.extended_palette();
    let base = iced_style::card::primary(theme, status);
    match status {
        iced_style::Status::Active => iced_card::Style {
            border_color: palette.primary.weak.color,
            head_background: palette.primary.weak.color.into(),
            head_text_color: palette.primary.weak.text,
            ..base
        },
        _ => base,
    }
}
