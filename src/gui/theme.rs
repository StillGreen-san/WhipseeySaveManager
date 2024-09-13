use iced::theme::{palette, Palette};
use iced::widget::{
    button, checkbox, container, overlay::menu, radio, scrollable, text, text_input,
};
use iced::{application, color, theme, Background, Border, Color, Vector};
use iced_aw::{card, modal, number_input, style, tab_bar};

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
        // adapted from iced_aw-0.9.3/src/style/tab_bar.rs:104
        let theme = match self {
            Light(theme) | Dark(theme) => theme,
        };
        let palette = theme.extended_palette();
        let pair = if is_active {
            palette.primary.strong
        } else {
            palette.background.weak
        };
        match style {
            Self::Style::Default => tab_bar::Appearance {
                tab_label_background: Background::Color(pair.color),
                text_color: pair.text,
                ..Default::default()
            },
            _ => theme.active(style, is_active),
        }
    }

    fn hovered(&self, style: &Self::Style, is_active: bool) -> tab_bar::Appearance {
        // adapted from iced_aw-0.9.3/src/style/tab_bar.rs:190
        let theme = match self {
            Light(theme) | Dark(theme) => theme,
        };
        let palette = theme.extended_palette();
        match style {
            Self::Style::Default => tab_bar::Appearance {
                tab_label_background: Background::Color(palette.primary.base.color),
                text_color: palette.primary.base.text,
                ..self.active(style, is_active)
            },
            _ => theme.hovered(style, is_active),
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
        let theme = match self {
            Light(theme) | Dark(theme) => theme,
        };
        let mut appearance = theme.active(style);
        appearance.border.color = theme.extended_palette().primary.weak.color;
        appearance
    }

    fn focused(&self, style: &Self::Style) -> text_input::Appearance {
        let theme = match self {
            Light(theme) | Dark(theme) => theme,
        };
        let mut appearance = theme.focused(style);
        appearance.border.width = 2.0;
        appearance
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
        let theme = match self {
            Light(theme) | Dark(theme) => theme,
        };
        let mut appearance = theme.hovered(style);
        appearance.border.color = theme.extended_palette().primary.base.color;
        appearance
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
        let theme = match self {
            Light(theme) | Dark(theme) => theme,
        };
        let palette = theme.extended_palette();
        let appearance = button::Appearance {
            border: Border::with_radius(2),
            ..button::Appearance::default()
        };
        let from_pair = |pair: palette::Pair| button::Appearance {
            background: Some(pair.color.into()),
            text_color: pair.text,
            ..appearance
        };

        match style {
            theme::Button::Primary => from_pair(palette.primary.strong),
            theme::Button::Secondary => from_pair(palette.secondary.base),
            theme::Button::Positive => from_pair(palette.success.base),
            theme::Button::Destructive => from_pair(palette.danger.base),
            theme::Button::Text => button::Appearance {
                text_color: palette.primary.strong.color,
                ..appearance
            },
            theme::Button::Custom(custom) => custom.active(theme),
        }
    }

    fn hovered(&self, style: &Self::Style) -> button::Appearance {
        let theme = match self {
            Light(theme) | Dark(theme) => theme,
        };

        if let theme::Button::Custom(custom) = style {
            return custom.hovered(theme);
        }

        let palette = theme.extended_palette();
        let active = theme.active(style);
        let pair = match style {
            theme::Button::Primary => palette.primary.base,
            theme::Button::Secondary => palette.background.strong,
            theme::Button::Positive => palette.success.strong,
            theme::Button::Destructive => palette.danger.strong,
            theme::Button::Text => {
                return button::Appearance {
                    background: None,
                    text_color: palette.primary.base.color,
                    ..active
                }
            }
            theme::Button::Custom(_) => unreachable!(),
        };

        button::Appearance {
            background: Some(pair.color.into()),
            text_color: pair.text,
            ..active
        }
    }

    fn pressed(&self, style: &Self::Style) -> button::Appearance {
        let theme = match self {
            Light(theme) | Dark(theme) => theme,
        };

        if let theme::Button::Custom(custom) = style {
            return custom.pressed(theme);
        }

        let palette = theme.extended_palette();
        let pair = match style {
            theme::Button::Primary => palette.primary.weak,
            theme::Button::Secondary => palette.secondary.weak,
            theme::Button::Positive => palette.success.weak,
            theme::Button::Destructive => palette.danger.weak,
            theme::Button::Text => {
                return button::Appearance {
                    shadow_offset: Vector::default(),
                    background: None,
                    text_color: palette.primary.weak.color,
                    ..self.active(style)
                }
            }
            theme::Button::Custom(_) => unreachable!(),
        };

        button::Appearance {
            shadow_offset: Vector::default(),
            background: Some(pair.color.into()),
            text_color: pair.text,
            ..self.active(style)
        }
    }

    fn disabled(&self, style: &Self::Style) -> button::Appearance {
        let theme = match self {
            Light(theme) | Dark(theme) => theme,
        };
        if let theme::Button::Custom(custom) = style {
            return custom.disabled(theme);
        }

        let active = theme.active(style);

        button::Appearance {
            shadow_offset: Vector::default(),
            background: active.background.map(|background| match background {
                Background::Color(color) => Background::Color(Color {
                    a: color.a * 0.5,
                    ..color
                }),
                Background::Gradient(gradient) => Background::Gradient(gradient.mul_alpha(0.5)),
            }),
            text_color: Color {
                a: active.text_color.a * 0.5,
                ..active.text_color
            },
            ..active
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
        // adapted from iced_aw-0.9.3/src/style/card.rs:81
        let theme = match self {
            Light(theme) | Dark(theme) => theme,
        };
        let palette = theme.extended_palette();
        let foreground = theme.palette();

        let backing_with_text = |pair: palette::Pair| card::Appearance {
            border_color: pair.color,
            head_background: pair.color.into(),
            head_text_color: pair.text,
            close_color: pair.text,
            background: palette.background.base.color.into(),
            body_text_color: foreground.text,
            foot_text_color: foreground.text,
            ..card::Appearance::default()
        };

        match style {
            Self::Style::Primary => backing_with_text(palette.primary.strong),
            Self::Style::Secondary => backing_with_text(palette.primary.weak),
            Self::Style::Default => backing_with_text(palette.background.weak),
            _ => theme.active(style),
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

impl modal::StyleSheet for Theme {
    type Style = style::ModalStyles;

    fn active(&self, style: &Self::Style) -> style::modal::Appearance {
        match self {
            Light(theme) | Dark(theme) => theme.active(style),
        }
    }
}
