use crate::data::file::{Ending, Intro, Level};
use crate::data::File;
use crate::gui::{Tab, TabState, Theme};
use iced::widget::tooltip::Position;
use iced::widget::{button, checkbox, column, radio, row, text, tooltip};
use iced::{Command, Element, Renderer};
use iced_aw::{card, number_input, TabLabel};
use strum::VariantArray;

#[derive(Clone, Debug)] // TODO shrink footprint
pub enum Message {
    Progress(usize, Level),
    Checked(usize, bool),
    Gems(usize, u8),
    CycleGems(usize),
    CycleLives(usize),
    Max(usize),
    Lives(usize, u32),
    Reset(usize),
}

pub struct DisplayStrings {
    pub titel: &'static str,
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
}

impl Files {
    pub fn new(display_strings: DisplayStrings) -> Self {
        Self {
            files_state: Default::default(),
            display_strings,
        }
    }

    fn file_view(&self, idx: usize) -> Element<'_, super::Message, Theme, Renderer> {
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
                    })
                    .map(Element::from),
            ),
        );
        let intro = column![
            tooltip(
                checkbox(
                    self.display_strings.intro_label,
                    self.files_state[idx].intro == Intro::Watched,
                )
                .on_toggle(move |toggled| super::Message::Files(Message::Checked(idx, toggled))),
                text(self.display_strings.intro_tooltip),
                Position::FollowCursor,
            ),
            text(self.display_strings.gems_label),
            tooltip(
                number_input(self.files_state[idx].gems.0, 99, move |gems| {
                    super::Message::Files(Message::Gems(idx, gems)) // TODO max from struct
                }),
                text(self.display_strings.gems_tooltip),
                Position::FollowCursor
            ),
            tooltip(
                button(text(self.display_strings.cycle_gems_label))
                    .on_press(super::Message::Files(Message::CycleGems(idx))),
                text(self.display_strings.cycle_gems_tooltip),
                Position::FollowCursor
            ),
            tooltip(
                button(text(self.display_strings.max_label))
                    .on_press(super::Message::Files(Message::Max(idx))),
                text(self.display_strings.max_tooltip),
                Position::FollowCursor
            ),
            tooltip(
                button(text(self.display_strings.save_label))
                    .on_press(super::Message::Saved(super::FileId::Save)), // TODO per file saving
                text(self.display_strings.save_tooltip),
                Position::FollowCursor
            )
        ];
        let ending = column![
            tooltip(
                checkbox(
                    self.display_strings.ending_label,
                    self.files_state[idx].ending == Ending::Watched,
                )
                .on_toggle(move |toggled| super::Message::Files(Message::Checked(idx, toggled))),
                text(self.display_strings.ending_tooltip),
                Position::FollowCursor,
            ),
            text(self.display_strings.lives_label),
            tooltip(
                number_input(self.files_state[idx].lives.0, 9999, move |lives| {
                    super::Message::Files(Message::Lives(idx, lives)) // TODO max from struct
                }),
                text(self.display_strings.lives_tooltip),
                Position::FollowCursor
            ),
            tooltip(
                button(text(self.display_strings.cycle_lives_label))
                    .on_press(super::Message::Files(Message::CycleLives(idx))),
                text(self.display_strings.cycle_lives_tooltip),
                Position::FollowCursor
            ),
            tooltip(
                button(text(self.display_strings.reset_label))
                    .on_press(super::Message::Files(Message::Reset(idx))),
                text(self.display_strings.reset_tooltip),
                Position::FollowCursor
            ),
            tooltip(
                button(text(self.display_strings.reload_label))
                    .on_press(super::Message::Saved(super::FileId::Save)), // TODO per file reload
                text(self.display_strings.reload_tooltip),
                Position::FollowCursor
            )
        ];
        let title = format!(
            "{}{} {} - {}",
            self.display_strings.file_titel,
            idx + 1,
            self.files_state[idx].boss_no_damage.bits(),
            self.files_state[idx].enemies_defeated.0
        );
        card(text(title), row![progress, intro, ending]).into()
    }
}

impl Tab for Files {
    type InMessage = Message;

    fn title(&self) -> String {
        self.display_strings.titel.into()
    }

    fn tab_label(&self) -> TabLabel {
        TabLabel::Text(self.title())
    }

    fn update(&mut self, message: Self::InMessage) -> Command<super::Message> {
        match message {
            Message::Progress(idx, level) => {
                self.files_state[idx].level = level;
                Command::none()
            }
            Message::Checked(idx, toggled) => {
                self.files_state[idx].intro = if toggled {
                    Intro::Watched
                } else {
                    Intro::Unwatched
                };
                Command::none()
            }
            Message::Gems(idx, gems) => {
                self.files_state[idx].gems.0 = gems;
                Command::none()
            }
            Message::CycleGems(idx) => todo!(),
            Message::CycleLives(idx) => todo!(),
            Message::Max(idx) => todo!(),
            Message::Lives(idx, lives) => {
                self.files_state[idx].lives.0 = lives;
                Command::none()
            }
            Message::Reset(idx) => {
                self.files_state[idx] = File::default();
                Command::none()
            }
        }
    }

    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        row([self.file_view(0), self.file_view(1), self.file_view(2)]).into()
    }
}

impl TabState for Files {
    type State = [File; 3];

    fn get_state(&self) -> Self::State {
        self.files_state.clone()
    }

    fn set_state(&mut self, state: Self::State) {
        self.files_state = state;
    }
}
