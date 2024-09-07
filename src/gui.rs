use crate::{data, util, BFS_SETTINGS_FILE_NAME, SAVEGAME_FILE_NAME};
use dark_light::Mode;
use data::file::{File1, File2, File3};
use iced::widget::{column, text, tooltip, Tooltip};
use iced::{font, theme, Application, Command, Element, Renderer};
use iced_aw::{TabBarStyles, TabLabel, Tabs};
use std::env::VarError;
use std::path::PathBuf;

mod about;
mod cheats;
mod file_select;
mod files;
mod options;

use crate::util::LocateError;
use about::About;
use cheats::Cheats;
use file_select::FileSelect;
use files::Files;
use options::Options;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum TabId {
    About,
    Cheats,
    Options,
    #[default]
    Files,
}

#[derive(Debug, Clone, Copy, Eq, PartialEq)]
pub enum SaveId {
    Files,
    Options,
    File1,
    File2,
    File3,
}

#[derive(Debug, Clone, Copy)]
pub enum FileId {
    Save(SaveId),
    Bfs,
}

#[derive(Debug, Clone)]
pub enum Message {
    TabSelected(TabId),
    About(about::Message),
    FileSelect(FileId, file_select::Message),
    Save(FileId),
    Load(FileId),
    LoadedBfs(data::Result<data::BfsSettings>),
    LoadedSave(SaveId, data::Result<data::WhipseeySaveData>),
    Saved(FileId, data::Result<()>),
    Cheats(cheats::Message),
    Options(options::Message),
    Files(files::Message),
    LoadedFont(Result<(), font::Error>),
    LoadedBfsSettingsPath(Result<Option<PathBuf>, LocateError>),
    LoadedSavegamePath(Result<Option<PathBuf>, VarError>),
    UpdatedTheme(Theme),
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
}

type Theme = iced::Theme;

impl Application for Gui {
    type Executor = iced::executor::Default;
    type Message = Message;
    type Theme = Theme;
    type Flags = ();

    fn new(_flags: Self::Flags) -> (Self, Command<Self::Message>) {
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
            dialog_title: "save game",
            dialog_file_name: SAVEGAME_FILE_NAME,
            dialog_filter_file: "save",
            dialog_filter_ext: "sav",
            dialog_filter_all: "all",
        }; // TODO localize (these and in general)
        let bfs_strings = file_select::DisplayStrings {
            placeholder: "bfs settings",
            dialog_title: "bfs settings",
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
        };
        let files_strings = files::DisplayStrings {
            titel: "Files",
            progress: "progress",
            file_titel: "file",
            intro_label: "Intro",
            intro_tooltip: "Will mark Slot as active, skipping Intro",
            gems_label: "Gems",
            gems_tooltip: "0 - 99",
            cycle_gems_label: "Cycle",
            cycle_gems_tooltip: "Set Gems to 0 or 99",
            max_label: "Max",
            max_tooltip: "100% File with 99999 Lives",
            save_label: "Save",
            save_tooltip: "Save current Values to File for this Group",
            ending_label: "Ending",
            ending_tooltip: "Only used to calculate %Progress",
            lives_label: "Lives",
            lives_tooltip: "1 - 99999", // TODO vals from struct
            cycle_lives_label: "Cycle",
            cycle_lives_tooltip: "Set Lives to 5 or 99999",
            reset_label: "Reset",
            reset_tooltip: "New File",
            reload_label: "Reload",
            reload_tooltip: "Reload Values from File for this Group",
        };
        (
            Self {
                active_tab: Default::default(),
                about: About::new(about_strings),
                save_path: FileSelect::new(FileId::Save(SaveId::Files), save_strings),
                cheats_path: FileSelect::new(FileId::Bfs, bfs_strings),
                cheats: Cheats::new(cheats_strings),
                options: Options::new(opt_strings),
                files: Files::new(files_strings),
                theme: Theme::default(),
            },
            Command::batch([
                font::load(iced_aw::BOOTSTRAP_FONT_BYTES).map(Message::LoadedFont),
                Command::perform(
                    util::find_bfs_settings_path(),
                    Message::LoadedBfsSettingsPath,
                ),
                Command::perform(util::find_savegame_path(), Message::LoadedSavegamePath),
                Command::perform(
                    async {
                        match dark_light::detect() {
                            Mode::Dark => Theme::Dark,
                            Mode::Light | Mode::Default => Theme::Light,
                        }
                    },
                    Message::UpdatedTheme,
                ),
            ]),
        )
    }

    fn title(&self) -> String {
        "Whipseey Save Manager".into()
    }

    fn update(&mut self, message: Self::Message) -> Command<Self::Message> {
        match message {
            Message::TabSelected(id) => {
                self.active_tab = id;
                match id {
                    TabId::Files => self.save_path.set_id(FileId::Save(SaveId::Files)),
                    TabId::Options => self.save_path.set_id(FileId::Save(SaveId::Options)),
                    _ => {}
                }
                Command::none()
            }
            Message::About(message) => self.about.update(message),
            Message::FileSelect(id, message) => match id {
                FileId::Save(_) => self.save_path.update(message),
                FileId::Bfs => self.cheats_path.update(message),
            },
            Message::Save(id) => match id {
                FileId::Save(save_id) => {
                    let save_now = data::WhipseeySaveData {
                        options: self.options.get_state(),
                        files: self.files.get_state(),
                    };
                    let path = self.save_path.get_state();
                    Command::perform(
                        async move {
                            let mut save_next: data::WhipseeySaveData =
                                util::load_ini_file(path.clone()).await?.try_into()?;
                            match save_id {
                                SaveId::Files => save_next.files = save_now.files,
                                SaveId::Options => save_next.options = save_now.options,
                                SaveId::File1 => save_next.files[File1] = save_now.files[File1],
                                SaveId::File2 => save_next.files[File2] = save_now.files[File2],
                                SaveId::File3 => save_next.files[File3] = save_now.files[File3],
                            }
                            let ini = save_next.into();
                            Ok(util::write_ini_file_padded(path, &ini).await?)
                        },
                        move |result| Message::Saved(id, result),
                    )
                }
                FileId::Bfs => {
                    let bfs = data::BfsSettings {
                        cheats: self.cheats.get_state(),
                    };
                    let ini = bfs.into();
                    let path = self.cheats_path.get_state();
                    Command::perform(
                        async move { Ok(util::write_ini_file(path, &ini).await?) },
                        |result| Message::Saved(FileId::Bfs, result),
                    )
                }
            },
            Message::Saved(_id, result) => {
                result.unwrap(); // TODO error handling
                Command::none()
            }
            Message::Load(id) => match id {
                FileId::Save(save_id) => {
                    let path = self.save_path.get_state();
                    Command::perform(
                        async { util::load_ini_file(path).await?.try_into() },
                        move |result| Message::LoadedSave(save_id, result),
                    )
                }
                FileId::Bfs => {
                    let path = self.cheats_path.get_state();
                    Command::perform(
                        async { util::load_ini_file(path).await?.try_into() },
                        Message::LoadedBfs,
                    )
                }
            },
            Message::LoadedBfs(result) => {
                self.cheats.set_state(result.unwrap().cheats); // TODO error handling (all unwraps)
                Command::none()
            }
            Message::LoadedSave(id, result) => {
                let save = result.unwrap();
                let mut files = self.files.get_state();
                match id {
                    SaveId::Files => self.files.set_state(save.files),
                    SaveId::Options => self.options.set_state(save.options),
                    SaveId::File1 => {
                        files[File1] = save.files[File1];
                        self.files.set_state(files);
                    }
                    SaveId::File2 => {
                        files[File2] = save.files[File2];
                        self.files.set_state(files);
                    }
                    SaveId::File3 => {
                        files[File3] = save.files[File3];
                        self.files.set_state(files);
                    }
                }
                Command::none()
            }
            Message::Cheats(message) => self.cheats.update(message),
            Message::Options(message) => self.options.update(message),
            Message::Files(message) => self.files.update(message),
            Message::LoadedFont(result) => {
                result.expect("loading font from const bytes should not be able to fail");
                Command::none()
            }
            Message::LoadedBfsSettingsPath(path) => {
                match path {
                    Ok(Some(path)) => self
                        .cheats_path
                        .update(file_select::Message::Selected(Some(path))),
                    _ => Command::none(), // TODO error handling
                }
            }
            Message::LoadedSavegamePath(path) => {
                match path {
                    Ok(Some(path)) => self
                        .save_path
                        .update(file_select::Message::Selected(Some(path))),
                    _ => Command::none(), // TODO error handling
                }
            }
            Message::UpdatedTheme(theme) => {
                self.theme = theme;
                Command::none()
            }
        }
    }

    fn view(&self) -> Element<'_, Self::Message, Self::Theme, Renderer> {
        Tabs::new(Message::TabSelected)
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
            .set_active_tab(&self.active_tab)
            .tab_bar_style(match self.theme {
                Theme::Light => TabBarStyles::Default,
                Theme::Dark => TabBarStyles::Dark,
                _ => unimplemented!(),
            })
            .into()
    }

    fn theme(&self) -> Self::Theme {
        self.theme.clone()
    }
}

trait Tab {
    type InMessage;

    fn title(&self) -> String;

    fn tab_label(&self) -> TabLabel;

    fn update(&mut self, message: Self::InMessage) -> Command<Message>;

    fn view(&self) -> Element<'_, Message, Theme, Renderer>;
}

trait ElementState {
    type State;

    fn get_state(&self) -> Self::State;

    fn set_state(&mut self, state: Self::State);
}

pub fn with_tooltip<'a>(
    content: impl Into<Element<'a, Message, Theme, Renderer>>,
    tooltip_text: impl ToString,
    position: tooltip::Position,
) -> Tooltip<'a, Message, Theme, Renderer> {
    tooltip(content, text(tooltip_text), position).style(theme::Container::Box)
}
