use crate::{data, util, BFS_SETTINGS_FILE_NAME, SAVEGAME_FILE_NAME};
use const_format::formatcp;
use data::file::{File1, File2, File3, Gems, Lives};
use iced::widget::scrollable::Direction;
use iced::widget::{
    center, column, container, mouse_area, opaque, scrollable, stack, text, tooltip, Tooltip,
};
use iced::{Color, Element, Length, Padding, Renderer, Task};
use iced_aw::{card, style, Card, TabLabel, Tabs};
use std::env::VarError;
use std::path::PathBuf;
use util::error::LocateError;

mod about;
mod cheats;
mod file_select;
mod files;
mod options;
mod theme;

use about::About;
use cheats::Cheats;
use file_select::FileSelect;
use files::Files;
use options::Options;
use theme::Theme;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum TabId {
    About,
    Cheats,
    Options,
    #[default]
    Files,
    #[cfg(debug_assertions)]
    TestButton,
}

#[derive(Debug, Clone, Copy, Eq, PartialEq)]
pub enum SaveSection {
    Files,
    Options,
    File1,
    File2,
    File3,
}

#[derive(Debug, Clone, Copy)]
pub enum FileSelectId {
    Save(SaveSection),
    Bfs,
}

#[derive(Debug, Clone)]
pub enum MessageSource {
    Click,
    Task,
}

#[derive(Debug, Clone)]
pub enum Message {
    TabSelected(TabId),
    About(about::Message),
    FileSelect(FileSelectId, file_select::Message),
    Save(FileSelectId),
    Load(FileSelectId),
    LoadedBfs(util::Result<(data::BfsSettings, Vec<util::Error>)>),
    LoadedSave(
        SaveSection,
        util::Result<(data::WhipseeySaveData, Vec<util::Error>)>,
    ),
    Saved(FileSelectId, util::Result<()>),
    Cheats(cheats::Message),
    Options(options::Message),
    Files(files::Message),
    LoadedFont,
    LoadedBfsSettingsPath(Result<Option<PathBuf>, LocateError>),
    LoadedSavegamePath(Result<Option<PathBuf>, VarError>),
    CloseError,
    Error((util::Error, String)),
    OpenModal((&'static str, &'static str)),
    CloseModal(MessageSource),
}

pub struct Gui {
    save_path: FileSelect,
    cheats_path: FileSelect,
    active_tab: TabId,
    about: About,
    cheats: Cheats,
    options: Options,
    files: Files,
    theme: Theme,
    errors: Vec<(util::Error, String)>,
    info_modal_data: Option<(&'static str, &'static str)>,
}

impl Gui {
    pub fn new() -> (Self, Task<Message>) {
        let about_strings = about::DisplayStrings {
            title: "About",
            description: "this program uses the following libraries:",
        };
        let save_strings = file_select::DisplayStrings {
            placeholder: "save game",
            open_label: "...",
            open_tooltip: "Open and Load a new File",
            save_label: "save",
            save_tooltip: "Save current Values to File",
            reload_label: "reload",
            reload_tooltip: "Reload Values from File",
            dialog_title: "open save game",
            dialog_file_name: SAVEGAME_FILE_NAME,
            dialog_filter_file: "save",
            dialog_filter_ext: "sav",
            dialog_filter_all: "all",
            modal_description: "waiting for a file to be chosen...",
        };
        let bfs_strings = file_select::DisplayStrings {
            placeholder: "bfs settings",
            dialog_title: "open bfs settings",
            dialog_file_name: BFS_SETTINGS_FILE_NAME,
            dialog_filter_file: "ini",
            dialog_filter_ext: "ini",
            ..save_strings
        };
        let cheats_strings = cheats::DisplayStrings {
            title: "Cheats",
            checkbox: "cheats enabled",
            description: "checking \"cheats enabled\" will enable some hotkeys in game:\
                          \nR  : restart room\
                          \nN  : next room\
                          \nP  : toggle fullscreen\
                          \n, . , .  : infinite flight\
                          \n, . , ,  : unlock all levels\
                          \n, , , .  : disable hud\
                          \n, , , ,  : invincibility",
        };
        let opt_strings = options::DisplayStrings {
            title: "Options",
            language: "Language",
            scale: "Scale",
            fullscreen: "Fullscreen",
            left_handed: "Left Handed",
            sound_volume: "Sound Volume",
            sound_toggle: "Sound Toggle",
            music_volume: "Music Volume",
            music_toggle: "Music Toggle",
            reset_label: "Reset",
            reset_tooltip: "Set defaults",
        };
        let files_strings = files::DisplayStrings {
            title: "Files",
            progress: "progress",
            file_titel: "file",
            intro_label: "Intro",
            intro_tooltip: "Will mark Slot as active, skipping Intro",
            gems_label: "Gems",
            gems_tooltip: formatcp!(
                "{min} - {max}",
                min = Gems::MIN_PRIMITIV,
                max = Gems::MAX_PRIMITIV
            ),
            cycle_gems_label: "Cycle",
            cycle_gems_tooltip: formatcp!(
                "Set Gems to {min} or {max}",
                min = Gems::MIN_PRIMITIV,
                max = Gems::MAX_PRIMITIV
            ),
            max_label: "Max",
            max_tooltip: formatcp!("100% File with {max} Lives", max = Lives::MAX_PRIMITIV),
            save_label: "Save",
            save_tooltip: "Save current Values to File for this Group",
            ending_label: "Ending",
            ending_tooltip: "Only used to calculate %Progress",
            lives_label: "Lives",
            lives_tooltip: formatcp!(
                "{min} - {max}",
                min = Lives::MIN_PRIMITIV,
                max = Lives::MAX_PRIMITIV
            ),
            cycle_lives_label: "Cycle",
            cycle_lives_tooltip: formatcp!(
                "Set Lives to {def} or {max}",
                def = Lives::DEFAULT_PRIMITIV,
                max = Lives::MAX_PRIMITIV
            ),
            reset_label: "Reset",
            reset_tooltip: "New File",
            reload_label: "Reload",
            reload_tooltip: "Reload Values from File for this Group",
        };
        (
            Self {
                active_tab: Default::default(),
                about: About::new(about_strings),
                save_path: FileSelect::new(FileSelectId::Save(SaveSection::Files), save_strings),
                cheats_path: FileSelect::new(FileSelectId::Bfs, bfs_strings),
                cheats: Cheats::new(cheats_strings),
                options: Options::new(opt_strings),
                files: Files::new(files_strings),
                theme: theme::default(),
                errors: Vec::with_capacity(3),
                info_modal_data: None,
            },
            Task::batch([
                Task::perform(
                    util::find_bfs_settings_path(),
                    Message::LoadedBfsSettingsPath,
                ),
                Task::perform(util::find_savegame_path(), Message::LoadedSavegamePath),
            ]),
        )
    }

    pub fn update(&mut self, message: Message) -> Task<Message> {
        match message {
            Message::TabSelected(id) => self.update_tab_selected(id),
            Message::About(message) => self.about.update(message),
            Message::FileSelect(id, message) => match id {
                FileSelectId::Save(_) => self.save_path_update(message),
                FileSelectId::Bfs => self.cheats_path.update(message),
            },
            Message::Save(id) => self.update_save(id),
            Message::Saved(id, result) => {
                match result {
                    Ok(()) => {}
                    Err(err) => self.errors.push((err, format!("saving {id:?}"))),
                }
                Task::none()
            }
            Message::Load(id) => match id {
                FileSelectId::Save(save_id) => {
                    let path = self.save_path.get_state();
                    Task::perform(
                        async {
                            Ok(data::WhipseeySaveData::from_ini(
                                util::load_ini_file(path).await?,
                            ))
                        },
                        move |result| Message::LoadedSave(save_id, result),
                    )
                }
                FileSelectId::Bfs => {
                    let path = self.cheats_path.get_state();
                    Task::perform(
                        async {
                            Ok(data::BfsSettings::from_ini(
                                util::load_ini_file(path).await?,
                            ))
                        },
                        Message::LoadedBfs,
                    )
                }
            },
            Message::LoadedBfs(result) => {
                let err_msg = "loading bfs_settings ini (using defaults)".to_string();
                match result {
                    Ok((bfs, errors)) => {
                        for err in errors {
                            self.errors.push((err, err_msg.clone()));
                        }
                        self.cheats.set_state(bfs)
                    }
                    Err(err) => self.errors.push((err, err_msg)),
                }
                Task::none()
            }
            Message::LoadedSave(id, result) => self.update_loaded_save(id, result),
            Message::Cheats(message) => self.cheats.update(message),
            Message::Options(message) => self.options.update(message),
            Message::Files(message) => self.files.update(message),
            Message::LoadedFont => Task::none(),
            Message::LoadedBfsSettingsPath(path) => match path {
                Ok(path) => path.map_or_else(Task::none, |path| {
                    self.cheats_path
                        .update(file_select::Message::Selected(Some(path)))
                }),
                Err(error) => {
                    self.errors
                        .push((error.into(), "loading bfs_settings path".into()));
                    Task::none()
                }
            },
            Message::LoadedSavegamePath(path) => match path {
                Ok(path) => path.map_or_else(Task::none, |path| {
                    self.save_path_update(file_select::Message::Selected(Some(path)))
                }),
                Err(error) => {
                    self.errors
                        .push((error.into(), "loading savegame path".into()));
                    Task::none()
                }
            },
            Message::CloseError => {
                self.errors.pop();
                Task::none()
            }
            Message::Error(error) => {
                self.errors.push(error);
                Task::none()
            }
            Message::OpenModal(messages) => {
                self.info_modal_data = Some(messages);
                Task::none()
            }
            Message::CloseModal(source) => {
                if self.info_modal_data.is_none() {
                    self.errors.pop();
                } else if let MessageSource::Task = source {
                    self.info_modal_data = None;
                }
                Task::none()
            }
        }
    }

    pub fn view(&self) -> Element<'_, Message, Theme, Renderer> {
        let tabs = Tabs::new(Message::TabSelected).tab_bar_style(theme::tab_bar);
        #[cfg(debug_assertions)]
        let tabs = tabs.push(TabId::TestButton, TabLabel::Text("Theme".into()), column![]);
        let tabs = tabs
            .push(
                TabId::Files,
                self.files.tab_label(),
                column![self.save_path.view(), self.files.view()]
                    .spacing(6)
                    .padding(4),
            )
            .push(
                TabId::Options,
                self.options.tab_label(),
                column![self.save_path.view(), self.options.view()]
                    .spacing(7)
                    .padding(4),
            )
            .push(
                TabId::Cheats,
                self.cheats.tab_label(),
                column![self.cheats_path.view(), self.cheats.view()]
                    .spacing(6)
                    .padding(4),
            )
            .push(TabId::About, self.about.tab_label(), self.about.view())
            .set_active_tab(&self.active_tab);
        modal(
            tabs,
            self.info_overlay().or_else(|| self.error_overlay()),
            Message::CloseModal(MessageSource::Click),
        )
    }

    pub fn theme(&self) -> Theme {
        self.theme.clone()
    }
}

impl Gui {
    /// calls [FileSelect::update] on save_path and updates other elements
    ///
    /// sets [Files::can_reload] from [FileSelect::does_path_exist]
    fn save_path_update(&mut self, message: file_select::Message) -> Task<Message> {
        let task = self.save_path.update(message);
        self.files.can_reload = self.save_path.does_path_exist();
        task
    }

    /// creates an error overlay [Element] when there are any errors
    fn error_overlay(&self) -> Option<Element<'_, Message, Theme, Renderer>> {
        let (error, origin) = match self.errors.last() {
            Some(error) => error,
            None => {
                return None;
            }
        };
        let elem = container(
            self.modal_card(format!("ERROR while {}", origin), format!("{:#?}", error))
                .on_close(Message::CloseError)
                .style(style::card::danger),
        )
        .center(Length::Shrink);
        Some(elem.into())
    }

    /// creates an informational overlay [Element] when a modal is active
    fn info_overlay(&self) -> Option<Element<'_, Message, Theme, Renderer>> {
        self.info_modal_data.map(|(title, body)| {
            container(self.modal_card(title.into(), body.into()))
                .center(Length::Shrink)
                .into()
        })
    }

    /// creates a fixed sized [Card] with the given title and scrollable body
    fn modal_card(&self, head: String, body: String) -> Card<'_, Message, Theme, Renderer> {
        card(
            text(head),
            scrollable(
                container(text(body).width(Length::Shrink).height(Length::Shrink))
                    .padding(Padding::ZERO.bottom(12)),
            )
            .direction(Direction::Both {
                horizontal: Default::default(),
                vertical: Default::default(),
            }),
        )
        .max_width(450.0)
        .max_height(230.0)
    }

    fn update_tab_selected(&mut self, id: TabId) -> Task<Message> {
        match id {
            #[cfg(debug_assertions)]
            TabId::TestButton => {
                self.theme = theme::cycle(&self.theme);
            }
            _ => self.active_tab = id,
        }
        match id {
            TabId::Files => self
                .save_path
                .set_id(FileSelectId::Save(SaveSection::Files)),
            TabId::Options => self
                .save_path
                .set_id(FileSelectId::Save(SaveSection::Options)),
            _ => {}
        }
        Task::none()
    }

    fn update_save(&mut self, id: FileSelectId) -> Task<Message> {
        match id {
            FileSelectId::Save(save_id) => {
                let save_now = data::WhipseeySaveData {
                    options: self.options.get_state(),
                    files: self.files.get_state(),
                };
                let path = self.save_path.get_state();
                Task::perform(
                    async move {
                        let (mut save_next, _) = data::WhipseeySaveData::from_ini(
                            util::load_ini_file(path.clone()).await?,
                        );
                        match save_id {
                            SaveSection::Files => save_next.files = save_now.files,
                            SaveSection::Options => save_next.options = save_now.options,
                            SaveSection::File1 => save_next.files[File1] = save_now.files[File1],
                            SaveSection::File2 => save_next.files[File2] = save_now.files[File2],
                            SaveSection::File3 => save_next.files[File3] = save_now.files[File3],
                        }
                        let ini = save_next.into();
                        Ok(util::write_ini_file_padded_no_spacing(path, &ini).await?)
                    },
                    move |result| Message::Saved(id, result),
                )
            }
            FileSelectId::Bfs => {
                let ini = self.cheats.get_state().into();
                let path = self.cheats_path.get_state();
                Task::perform(
                    async move { Ok(util::write_ini_file(path, &ini).await?) },
                    |result| Message::Saved(FileSelectId::Bfs, result),
                )
            }
        }
    }

    fn update_loaded_save(
        &mut self,
        id: SaveSection,
        result: util::Result<(data::WhipseeySaveData, Vec<util::Error>)>,
    ) -> Task<Message> {
        let err_msg = format!("loading {id:?} in savegame (using defaults)");
        let save = match result {
            Ok((save, errors)) => {
                for err in errors {
                    self.errors.push((err, err_msg.clone()));
                }
                save
            }
            Err(err) => {
                self.errors.push((err, err_msg));
                return Task::none();
            }
        };
        let mut files = self.files.get_state();
        match id {
            SaveSection::Files => self.files.set_state(save.files),
            SaveSection::Options => self.options.set_state(save.options),
            SaveSection::File1 => {
                files[File1] = save.files[File1];
                self.files.set_state(files);
            }
            SaveSection::File2 => {
                files[File2] = save.files[File2];
                self.files.set_state(files);
            }
            SaveSection::File3 => {
                files[File3] = save.files[File3];
                self.files.set_state(files);
            }
        }
        Task::none()
    }
}

/// A single tab in the [Tabs] widget.
trait Tab {
    type InMessage;

    /// Returns the (title) label of the [Tabs] tab.
    fn tab_label(&self) -> TabLabel;

    /// Handles a message and updates the state of the [Tabs] tab.
    ///
    /// Any Task returned will be executed immediately in the background.
    fn update(&mut self, message: Self::InMessage) -> Task<Message>;

    /// Returns the widgets to display in the [Tabs] tab.
    ///
    /// These widgets can produce messages based on user interaction.
    fn view(&self) -> Element<'_, Message, Theme, Renderer>;
}

/// allows to query and set the 'state' of an Element
trait ElementState {
    type State;

    fn get_state(&self) -> Self::State;

    fn set_state(&mut self, state: Self::State);
}

pub fn with_tooltip<'a>(
    content: impl Into<Element<'a, Message, Theme, Renderer>>,
    tooltip_text: impl text::IntoFragment<'a>,
    position: tooltip::Position,
) -> Tooltip<'a, Message, Theme, Renderer> {
    tooltip(content, text(tooltip_text), position).style(container::bordered_box)
}

fn modal<'a, Message>(
    base: impl Into<Element<'a, Message>>,
    content: Option<impl Into<Element<'a, Message>>>,
    on_background: Message,
) -> Element<'a, Message>
where
    Message: Clone + 'a,
{
    stack([base.into()])
        .push_maybe(content.map(move |content| {
            opaque(
                mouse_area(center(opaque(content)).style(|_theme| {
                    container::Style {
                        background: Some(
                            Color {
                                a: 0.6,
                                ..Color::BLACK
                            }
                            .into(),
                        ),
                        ..container::Style::default()
                    }
                }))
                .on_press(on_background),
            )
        }))
        .into()
}
