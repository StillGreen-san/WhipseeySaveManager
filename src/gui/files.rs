use crate::data::file::{Ending, File1, File2, File3, FileIndex, Gems, Intro, Level, Lives};
use crate::data::File;
use crate::gui::{with_tooltip, Tab, TabState, Theme};
use iced::alignment::Horizontal;
use iced::widget::tooltip::Position;
use iced::widget::{button, checkbox, column, radio, row, text};
use iced::{Alignment, Command, Element, Length, Renderer};
use iced_aw::{card, number_input, CardStyles, TabLabel};
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

    fn file_view(&self, idx: FileIndex) -> Element<'_, super::Message, Theme, Renderer> {
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
        .style(CardStyles::Secondary)
        .padding_head([10, 7].into())
        .padding_body([10, 6].into())
        .width(Length::FillPortion(36));
        let intro = column![
            with_tooltip(
                checkbox(
                    self.display_strings.intro_label,
                    self.files_state[idx].intro == Intro::Watched,
                )
                .on_toggle(move |toggled| super::Message::Files(Message::Intro(idx, toggled)))
                .size(18)
                .spacing(4),
                self.display_strings.intro_tooltip,
                Position::Top
            ),
            column![
                text(self.display_strings.gems_label),
                with_tooltip(
                    number_input(
                        self.files_state[idx].gems.value(),
                        Gems::MAX_PRIMITIV,
                        move |gems| { super::Message::Files(Message::Gems(idx, gems)) }
                    )
                    .width(Length::Fill),
                    self.display_strings.gems_tooltip,
                    Position::Left
                )
            ],
            with_tooltip(
                button(
                    text(self.display_strings.cycle_gems_label)
                        .horizontal_alignment(Horizontal::Center)
                        .width(Length::Fill) // TODO abstract common layouting into functions
                )
                .on_press(super::Message::Files(Message::CycleGems(idx)))
                .width(Length::Fill),
                self.display_strings.cycle_gems_tooltip,
                Position::Left
            ),
            with_tooltip(
                button(
                    text(self.display_strings.max_label)
                        .horizontal_alignment(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press(super::Message::Files(Message::Max(idx)))
                .width(Length::Fill),
                self.display_strings.max_tooltip,
                Position::Left
            ),
            with_tooltip(
                button(
                    text(self.display_strings.save_label)
                        .horizontal_alignment(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press(super::Message::Saved(super::FileId::Save))
                .width(Length::Fill), // TODO per file saving
                self.display_strings.save_tooltip,
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
                self.display_strings.ending_tooltip,
                Position::Top
            ),
            column![
                text(self.display_strings.lives_label),
                with_tooltip(
                    number_input(
                        self.files_state[idx].lives.value(),
                        Lives::MAX_PRIMITIV,
                        move |lives| { super::Message::Files(Message::Lives(idx, lives)) }
                    )
                    .width(Length::Fill),
                    self.display_strings.lives_tooltip,
                    Position::Left
                )
            ],
            with_tooltip(
                button(
                    text(self.display_strings.cycle_lives_label)
                        .horizontal_alignment(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press(super::Message::Files(Message::CycleLives(idx)))
                .width(Length::Fill),
                self.display_strings.cycle_lives_tooltip,
                Position::Right
            ),
            with_tooltip(
                button(
                    text(self.display_strings.reset_label)
                        .horizontal_alignment(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press(super::Message::Files(Message::Reset(idx)))
                .width(Length::Fill),
                self.display_strings.reset_tooltip,
                Position::Right
            ),
            with_tooltip(
                button(
                    text(self.display_strings.reload_label)
                        .horizontal_alignment(Horizontal::Center)
                        .width(Length::Fill)
                )
                .on_press(super::Message::Saved(super::FileId::Save))
                .width(Length::Fill), // TODO per file reload
                self.display_strings.reload_tooltip,
                Position::Right
            )
        ]
        .spacing(4)
        .width(Length::FillPortion(35));
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
                .align_items(Alignment::Center),
        )
        .padding_body([10, 8].into())
        .into()
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
            Message::Intro(idx, toggled) => {
                self.files_state[idx].intro = (toggled as u8).try_into().unwrap();
                Command::none()
            }
            Message::Ending(idx, toggled) => {
                self.files_state[idx].ending = (toggled as u8).try_into().unwrap();
                Command::none()
            }
            Message::Gems(idx, gems) => {
                self.files_state[idx].gems = gems.try_into().unwrap();
                Command::none()
            }
            Message::CycleGems(idx) => {
                self.files_state[idx].gems = match self.files_state[idx].gems {
                    Gems::DEFAULT => Gems::MAX,
                    _ => Gems::DEFAULT,
                };
                Command::none()
            }
            Message::CycleLives(idx) => {
                self.files_state[idx].lives = match self.files_state[idx].lives {
                    Lives::DEFAULT => Lives::MAX,
                    _ => Lives::DEFAULT,
                };
                Command::none()
            }
            Message::Max(idx) => {
                self.files_state[idx].lives = Lives::MAX;
                self.files_state[idx].gems = Gems::MAX;
                self.files_state[idx].level = Level::Castle;
                self.files_state[idx].intro = Intro::Watched;
                self.files_state[idx].ending = Ending::Watched;
                Command::none()
            }
            Message::Lives(idx, lives) => {
                self.files_state[idx].lives = lives.try_into().unwrap();
                Command::none()
            }
            Message::Reset(idx) => {
                self.files_state[idx] = File::default();
                Command::none()
            }
        }
    }

    fn view(&self) -> Element<'_, super::Message, Theme, Renderer> {
        row![
            self.file_view(File1),
            self.file_view(File2),
            self.file_view(File3),
        ]
        .spacing(4)
        .into()
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
