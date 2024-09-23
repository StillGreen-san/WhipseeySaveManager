use crate::data::file::{Ending, File1, File2, File3, FileIndex, Gems, Intro, Level, Lives};
use crate::data::File;
use crate::gui::{theme, with_tooltip, ElementState, Tab, Theme};
use iced::alignment::{Horizontal, Vertical};
use iced::widget::tooltip::Position;
use iced::widget::{button, checkbox, column, radio, row, text};
use iced::{Element, Length, Renderer, Task};
use iced_aw::{card, number_input, TabLabel};
use strum::VariantArray;

#[derive(Clone, Debug)]
pub enum Message {
    Progress(FileIndex, Level),
    Intro(FileIndex, bool),
    Ending(FileIndex, bool),
    Gems(FileIndex, u8),
    CycleGems(FileIndex),
    CycleLives(FileIndex),
    Max(FileIndex),
    Lives(FileIndex, u32),
    Reset(FileIndex),
}

pub struct DisplayStrings {
    pub title: &'static str,
    pub progress: &'static str,
    pub file_titel: &'static str,
    pub intro_label: &'static str,
    pub intro_tooltip: &'static str,
    pub gems_label: &'static str,
    pub gems_tooltip: &'static str,
    pub cycle_gems_label: &'static str,
    pub cycle_gems_tooltip: &'static str,
    pub max_label: &'static str,
    pub max_tooltip: &'static str,
    pub save_label: &'static str,
    pub save_tooltip: &'static str,
    pub ending_label: &'static str,
    pub ending_tooltip: &'static str,
    pub lives_label: &'static str,
    pub lives_tooltip: &'static str,
    pub cycle_lives_label: &'static str,
    pub cycle_lives_tooltip: &'static str,
    pub reset_label: &'static str,
    pub reset_tooltip: &'static str,
    pub reload_label: &'static str,
    pub reload_tooltip: &'static str,
}

pub struct Files {
    files_state: [File; 3],
    display_strings: DisplayStrings,
    pub can_reload: bool,
}

impl Files {
    pub fn new(display_strings: DisplayStrings) -> Self {
        Self {
            files_state: Default::default(),
            display_strings,
            can_reload: true,
        }
    }

    /// build a new file element for the given `idx`
    fn file_view(
        &self,
        idx: FileIndex,
        show_tooltips: bool,
    ) -> Element<'_, super::Message, Theme, Renderer> {
        let progress = card(
            text(self.display_strings.progress),
            column(
                Level::VARIANTS
                    .iter()
                    .copied()
                    .map(|level| {
                        radio(
                            level.to_string(),
                            level,
                            Some(self.files_state[idx].level),
                            |level| super::Message::Files(Message::Progress(idx, level)),
                        )
                        .size(14)
                        .spacing(5)
                    })
                    .map(Element::from),
            ),
        )
        .style(theme::card_secondary)
        .padding_head([10, 7].into())
        .padding_body([10, 6].into())
        .width(Length::FillPortion(37));
        let intro = column![
            with_tooltip(
                checkbox(
                    self.display_strings.intro_label,
                    self.files_state[idx].intro == Intro::Watched,
                )
                .on_toggle(move |toggled| super::Message::Files(Message::Intro(idx, toggled)))
                .size(18)
                .spacing(4),
                show_tooltips.then_some(self.display_strings.intro_tooltip),
                Position::Top
            ),
            column![
                text(self.display_strings.gems_label),
                with_tooltip(
                    number_input(
                        self.files_state[idx].gems.value(),
                        Gems::MIN_PRIMITIV..Gems::MAX_PRIMITIV,
                        move |gems| { super::Message::Files(Message::Gems(idx, gems)) }
                    )
                    .input_style(theme::text_input)
                    .width(Length::Fill),
                    show_tooltips.then_some(self.display_strings.gems_tooltip),
                    Position::Left
                )
            ],
            with_tooltip(
                button(
                    text(self.display_strings.cycle_gems_label)
                        .align_x(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press(super::Message::Files(Message::CycleGems(idx)))
                .width(Length::Fill),
                show_tooltips.then_some(self.display_strings.cycle_gems_tooltip),
                Position::Left
            ),
            with_tooltip(
                button(
                    text(self.display_strings.max_label)
                        .align_x(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press(super::Message::Files(Message::Max(idx)))
                .width(Length::Fill),
                show_tooltips.then_some(self.display_strings.max_tooltip),
                Position::Left
            ),
            with_tooltip(
                button(
                    text(self.display_strings.save_label)
                        .align_x(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press(super::Message::Save(super::FileSelectId::Save(match idx {
                    File1 => super::SaveSection::File1,
                    File2 => super::SaveSection::File2,
                    File3 => super::SaveSection::File3,
                })))
                .width(Length::Fill),
                show_tooltips.then_some(self.display_strings.save_tooltip),
                Position::Left
            )
        ]
        .spacing(4)
        .width(Length::FillPortion(29));
        let ending = column![
            with_tooltip(
                checkbox(
                    self.display_strings.ending_label,
                    self.files_state[idx].ending == Ending::Watched,
                )
                .on_toggle(move |toggled| super::Message::Files(Message::Ending(idx, toggled)))
                .size(18)
                .spacing(4),
                show_tooltips.then_some(self.display_strings.ending_tooltip),
                Position::Top
            ),
            column![
                text(self.display_strings.lives_label),
                with_tooltip(
                    number_input(
                        self.files_state[idx].lives.value(),
                        Lives::MIN_PRIMITIV..Lives::MAX_PRIMITIV,
                        move |lives| { super::Message::Files(Message::Lives(idx, lives)) }
                    )
                    .input_style(theme::text_input)
                    .width(Length::Fill),
                    show_tooltips.then_some(self.display_strings.lives_tooltip),
                    Position::Left
                )
            ],
            with_tooltip(
                button(
                    text(self.display_strings.cycle_lives_label)
                        .align_x(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press(super::Message::Files(Message::CycleLives(idx)))
                .width(Length::Fill),
                show_tooltips.then_some(self.display_strings.cycle_lives_tooltip),
                Position::Right
            ),
            with_tooltip(
                button(
                    text(self.display_strings.reset_label)
                        .align_x(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press(super::Message::Files(Message::Reset(idx)))
                .width(Length::Fill),
                show_tooltips.then_some(self.display_strings.reset_tooltip),
                Position::Right
            ),
            with_tooltip(
                button(
                    text(self.display_strings.reload_label)
                        .align_x(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press_maybe(self.can_reload.then_some(super::Message::Load(
                    super::FileSelectId::Save(match idx {
                        File1 => super::SaveSection::File1,
                        File2 => super::SaveSection::File2,
                        File3 => super::SaveSection::File3,
                    })
                )))
                .width(Length::Fill),
                show_tooltips.then_some(self.display_strings.reload_tooltip),
                Position::Right
            )
        ]
        .spacing(4)
        .width(Length::FillPortion(34));
        let title = format!(
            "{}{} {} - {}",
            self.display_strings.file_titel,
            (idx as usize) + 1,
            self.files_state[idx].boss_no_damage_progress.value(),
            self.files_state[idx].enemies_defeated.value()
        );
        card(
            text(title),
            row![progress, intro, ending]
                .spacing(4)
                .align_y(Vertical::Center),
        )
        .padding_body([10, 8].into())
        .style(theme::card_primary)
        .into()
    }
}

impl Tab for Files {
    type InMessage = Message;

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.display_strings.title.into())
    }

    fn update(&mut self, message: Self::InMessage) -> Task<super::Message> {
        match message {
            Message::Progress(idx, level) => {
                self.files_state[idx].level = level;
                Task::none()
            }
            Message::Intro(idx, toggled) => {
                self.files_state[idx].intro = (toggled as u8)
                    .try_into()
                    .expect("bool should always be convertible to Intro");
                Task::none()
            }
            Message::Ending(idx, toggled) => {
                self.files_state[idx].ending = (toggled as u8)
                    .try_into()
                    .expect("bool should always be convertible to Ending");
                Task::none()
            }
            Message::Gems(idx, gems) => {
                self.files_state[idx].gems = gems
                    .try_into()
                    .expect("number should have been clamped by number_input");
                Task::none()
            }
            Message::CycleGems(idx) => {
                self.files_state[idx].gems = match self.files_state[idx].gems {
                    Gems::DEFAULT => Gems::MAX,
                    _ => Gems::DEFAULT,
                };
                Task::none()
            }
            Message::CycleLives(idx) => {
                self.files_state[idx].lives = match self.files_state[idx].lives {
                    Lives::DEFAULT => Lives::MAX,
                    _ => Lives::DEFAULT,
                };
                Task::none()
            }
            Message::Max(idx) => {
                self.files_state[idx].lives = Lives::MAX;
                self.files_state[idx].gems = Gems::MAX;
                self.files_state[idx].level = Level::Castle;
                self.files_state[idx].intro = Intro::Watched;
                self.files_state[idx].ending = Ending::Watched;
                Task::none()
            }
            Message::Lives(idx, lives) => {
                self.files_state[idx].lives = lives
                    .try_into()
                    .expect("number should have been clamped by number_input");
                Task::none()
            }
            Message::Reset(idx) => {
                self.files_state[idx] = File::default();
                Task::none()
            }
        }
    }

    fn view(&self, show_tooltips: bool) -> Element<'_, super::Message, Theme, Renderer> {
        row![
            self.file_view(File1, show_tooltips),
            self.file_view(File2, show_tooltips),
            self.file_view(File3, show_tooltips),
        ]
        .spacing(4)
        .into()
    }
}

impl ElementState for Files {
    type State = [File; 3];

    fn get_state(&self) -> Self::State {
        self.files_state
    }

    fn set_state(&mut self, state: Self::State) {
        self.files_state = state;
    }
}
