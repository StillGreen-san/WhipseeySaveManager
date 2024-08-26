use crate::{data, util};
use iced::widget::Column;
use iced::{Application, Command, Element, Renderer};
use iced_aw::{TabLabel, Tabs};
use std::path::PathBuf;

mod about;
mod cheats;
mod file_select;
mod files;
mod options;

use about::About;
use cheats::Cheats;
use file_select::FileSelect;
use files::Files;
use options::Options;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
enum TabId {
    #[default]
    About,
    Cheats,
    Options,
    Files,
}

#[derive(Debug, Clone, Copy)]
enum FileId {
    Save,
    Bfs,
}

#[derive(Debug, Clone)]
pub enum Message {
    TabSelected(TabId),
    About(about::Message),
    FileSelect(FileId, file_select::Message),
    Save(FileId, PathBuf),
    Load(FileId, PathBuf),
    LoadedBfs(data::Result<data::BfsSettings>),
    LoadedSave(data::Result<data::WhipseeySaveData>),
    Saved(FileId),
    Cheats(cheats::Message),
    Options(options::Message),
    Files(files::Message),
}

pub struct Gui {
    save: FileSelect,
    bfs: FileSelect,
    active_tab: TabId,
    about: About,
    cheats: Cheats,
    options: Options,
    files: Files,
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
            dialog_filter_file: "save",
            dialog_filter_ext: "sav",
            dialog_filter_all: "all",
        }; // TODO localize (these and in general)
        let bfs_strings = file_select::DisplayStrings {
            placeholder: "bfs settings",
            dialog_title: "bfs settings",
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
            intro_tooltip: "Will skip Intro if checked",
            gems_label: "Gems",
            gems_tooltip: "0 - 99",
            cycle_gems_label: "Cycle",
            cycle_gems_tooltip: "Set Gems to 0 or 99",
            max_label: "Max",
            max_tooltip: "100% File with 9999 Lives",
            save_label: "Save",
            save_tooltip: "Save current Values to File for this Group",
            ending_label: "Ending",
            ending_tooltip: "Only used to calculate %Progress",
            lives_label: "Lives",
            lives_tooltip: "1 - 16777215", // TODO vals from struct
            cycle_lives_label: "Cycle",
            cycle_lives_tooltip: "Set Lives to 5, 99 or 9999",
            reset_label: "Reset",
            reset_tooltip: "New File",
            reload_label: "Reload",
            reload_tooltip: "Reload Values from File for this Group",
        };
        (
            Self {
                active_tab: Default::default(),
                about: About::new(about_strings),
                save: FileSelect::new(FileId::Save, save_strings),
                bfs: FileSelect::new(FileId::Bfs, bfs_strings),
                cheats: Cheats::new(cheats_strings),
                options: Options::new(opt_strings),
                files: Files::new(files_strings),
            },
            Command::none(),
        )
    }

    fn title(&self) -> String {
        "Whipseey Save Manager".into()
    }

    fn update(&mut self, message: Self::Message) -> Command<Self::Message> {
        match message {
            Message::TabSelected(id) => {
                self.active_tab = id;
                Command::none()
            }
            Message::About(message) => self.about.update(message),
            Message::FileSelect(id, message) => match id {
                FileId::Save => self.save.update(message),
                FileId::Bfs => self.bfs.update(message),
            },
            Message::Save(id, path) => match id {
                FileId::Save => {
                    let save = data::WhipseeySaveData {
                        options: self.options.get_state(),
                        files: self.files.get_state(),
                    };
                    let ini = save.into();
                    Command::perform(
                        async move { util::write_ini_file_padded(path, &ini).await },
                        |_| Message::Saved(FileId::Save),
                    )
                }
                FileId::Bfs => {
                    let bfs = data::BfsSettings {
                        cheats: self.cheats.get_state(),
                    };
                    let ini = bfs.into();
                    Command::perform(
                        async move { util::write_ini_file(path, &ini).await },
                        |_| Message::Saved(FileId::Bfs),
                    )
                }
            },
            Message::Saved(_) => Command::none(), // TODO?
            Message::Load(id, path) => match id {
                FileId::Save => Command::perform(
                    async { util::load_ini_file(path).await?.try_into() },
                    Message::LoadedSave,
                ),
                FileId::Bfs => Command::perform(
                    async { util::load_ini_file(path).await?.try_into() },
                    Message::LoadedBfs,
                ),
            },
            Message::LoadedBfs(result) => {
                self.cheats.set_state(result.unwrap().cheats); // TODO error handling (all unwraps)
                Command::none()
            }
            Message::LoadedSave(result) => {
                let save = result.unwrap();
                self.options.set_state(save.options);
                self.files.set_state(save.files);
                Command::none()
            }
            Message::Cheats(message) => self.cheats.update(message),
            Message::Options(message) => self.options.update(message),
            Message::Files(message) => self.files.update(message),
        }
    }

    fn view(&self) -> Element<'_, Self::Message, Self::Theme, Renderer> {
        Column::new()
            .push(self.save.view())
            .push(self.bfs.view())
            .push(
                Tabs::new(Message::TabSelected)
                    .push(TabId::Files, self.files.tab_label(), self.files.view())
                    .push(
                        TabId::Options,
                        self.options.tab_label(),
                        self.options.view(),
                    )
                    .push(TabId::Cheats, self.cheats.tab_label(), self.cheats.view())
                    .push(TabId::About, self.about.tab_label(), self.about.view())
                    .set_active_tab(&self.active_tab),
            )
            .into()
    }
}

trait Tab {
    type InMessage;

    fn title(&self) -> String;

    fn tab_label(&self) -> TabLabel;

    fn update(&mut self, message: Self::InMessage) -> Command<Message>;

    fn view(&self) -> Element<'_, Message, Theme, Renderer>;
}

trait TabState {
    type State;

    fn get_state(&self) -> Self::State;

    fn set_state(&mut self, state: Self::State);
}
