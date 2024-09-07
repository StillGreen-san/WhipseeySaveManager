use iced::theme::Palette;
use iced::widget::{
    button, checkbox, container, overlay::menu, radio, scrollable, text, text_input,
};
use iced::Color;
use iced::{application, theme};
use iced_aw::{card, number_input, style, tab_bar};

#[derive(Debug, Clone, PartialEq)]
pub enum Theme {
    Light(iced::Theme),
    Dark(iced::Theme),
}
use Theme::Dark;
use Theme::Light;

impl Default for Theme {
    fn default() -> Self {
        light()
    }
}

const WHIPSEEY_LOGO_PINK: Color = Color::from_rgb(
    0xE9 as f32 / 255.0,
    0x77 as f32 / 255.0,
    0xA8 as f32 / 255.0,
);
const WHIPSEEY_LIGHT_PALETTE: Palette = Palette {
    background: Palette::LIGHT.background,
    text: Palette::LIGHT.text,
    primary: Palette::LIGHT.primary,
    success: Palette::LIGHT.success,
    danger: Palette::LIGHT.danger,
};
const WHIPSEEY_DARK_PALETTE: Palette = Palette {
    background: Palette::DARK.background,
    text: Palette::DARK.text,
    primary: Palette::DARK.primary,
    success: Palette::DARK.success,
    danger: Palette::DARK.danger,
};

pub fn light() -> Theme {
    Light(iced::Theme::custom(
        "WhipseeyLight".into(),
        WHIPSEEY_LIGHT_PALETTE,
    ))
}

pub fn dark() -> Theme {
    Dark(iced::Theme::custom(
        "WhipseeyDark".into(),
        WHIPSEEY_DARK_PALETTE,
    ))
}

impl application::StyleSheet for Theme {
    type Style = theme::Application;

    fn appearance(&self, style: &Self::Style) -> application::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.appearance(style),
        }
    }
}

impl text::StyleSheet for Theme {
    type Style = theme::Text;

    fn appearance(&self, style: Self::Style) -> text::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.appearance(style),
        }
    }
}

impl tab_bar::StyleSheet for Theme {
    type Style = style::TabBarStyles;

    fn active(&self, style: &Self::Style, is_active: bool) -> tab_bar::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.active(style, is_active),
        }
    }

    fn hovered(&self, style: &Self::Style, is_active: bool) -> tab_bar::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.hovered(style, is_active),
        }
    }
}

impl container::StyleSheet for Theme {
    type Style = theme::Container;

    fn appearance(&self, style: &Self::Style) -> container::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.appearance(style),
        }
    }
}

impl checkbox::StyleSheet for Theme {
    type Style = theme::Checkbox;

    fn active(&self, style: &Self::Style, is_checked: bool) -> checkbox::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.active(style, is_checked),
        }
    }

    fn hovered(&self, style: &Self::Style, is_checked: bool) -> checkbox::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.hovered(style, is_checked),
        }
    }

    fn disabled(&self, style: &Self::Style, is_checked: bool) -> checkbox::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.disabled(style, is_checked),
        }
    }
}

impl number_input::StyleSheet for Theme {
    type Style = style::NumberInputStyles;

    fn active(&self, style: &Self::Style) -> number_input::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.active(style),
        }
    }

    fn pressed(&self, style: &Self::Style) -> number_input::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.pressed(style),
        }
    }

    fn disabled(&self, style: &Self::Style) -> number_input::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.disabled(style),
        }
    }
}

impl text_input::StyleSheet for Theme {
    type Style = theme::TextInput;

    fn active(&self, style: &Self::Style) -> text_input::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.active(style),
        }
    }

    fn focused(&self, style: &Self::Style) -> text_input::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.focused(style),
        }
    }

    fn placeholder_color(&self, style: &Self::Style) -> Color {
        match self {
            Light(theme) | Dark(theme) => theme.placeholder_color(style),
        }
    }

    fn value_color(&self, style: &Self::Style) -> Color {
        match self {
            Light(theme) | Dark(theme) => theme.value_color(style),
        }
    }

    fn disabled_color(&self, style: &Self::Style) -> Color {
        match self {
            Light(theme) | Dark(theme) => theme.disabled_color(style),
        }
    }

    fn selection_color(&self, style: &Self::Style) -> Color {
        match self {
            Light(theme) | Dark(theme) => theme.selection_color(style),
        }
    }

    fn hovered(&self, style: &Self::Style) -> text_input::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.hovered(style),
        }
    }

    fn disabled(&self, style: &Self::Style) -> text_input::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.disabled(style),
        }
    }
}

impl button::StyleSheet for Theme {
    type Style = theme::Button;

    fn active(&self, style: &Self::Style) -> button::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.active(style),
        }
    }

    fn hovered(&self, style: &Self::Style) -> button::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.hovered(style),
        }
    }

    fn pressed(&self, style: &Self::Style) -> button::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.pressed(style),
        }
    }

    fn disabled(&self, style: &Self::Style) -> button::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.disabled(style),
        }
    }
}

impl radio::StyleSheet for Theme {
    type Style = theme::Radio;

    fn active(&self, style: &Self::Style, is_selected: bool) -> radio::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.active(style, is_selected),
        }
    }

    fn hovered(&self, style: &Self::Style, is_selected: bool) -> radio::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.hovered(style, is_selected),
        }
    }
}

impl card::StyleSheet for Theme {
    type Style = style::CardStyles;

    fn active(&self, style: &Self::Style) -> card::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.active(style),
        }
    }
}

impl scrollable::StyleSheet for Theme {
    type Style = theme::Scrollable;

    fn active(&self, style: &Self::Style) -> scrollable::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.active(style),
        }
    }

    fn hovered(
        &self,
        style: &Self::Style,
        is_mouse_over_scrollbar: bool,
    ) -> scrollable::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.hovered(style, is_mouse_over_scrollbar),
        }
    }

    fn dragging(&self, style: &Self::Style) -> scrollable::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.dragging(style),
        }
    }
}

impl menu::StyleSheet for Theme {
    type Style = theme::Menu;

    fn appearance(&self, style: &Self::Style) -> menu::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.appearance(style),
        }
    }
}
