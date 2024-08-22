use crate::{data, system};
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
use options::Options;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
enum TabId {
    #[default]
    About,
    Cheats,
    Options,
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
            open: "...",
            save: "save",
            reload: "reload",
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
        (
            Self {
                active_tab: Default::default(),
                about: About::new(about_strings),
                save: FileSelect::new(FileId::Save, save_strings),
                bfs: FileSelect::new(FileId::Bfs, bfs_strings),
                cheats: Cheats::new(cheats_strings),
                options: Options::new(opt_strings),
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
                FileId::Save => todo!(),
                FileId::Bfs => {
                    let bfs = data::BfsSettings {
                        cheats: self.cheats.get_state(),
                    };
                    let ini = bfs.into();
                    Command::perform(
                        async move { system::write_ini_file(path, &ini).await },
                        |_| Message::Saved(FileId::Bfs),
                    )
                }
            },
            Message::Saved(_) => Command::none(), // TODO?
            Message::Load(id, path) => match id {
                FileId::Save => todo!(),
                FileId::Bfs => Command::perform(
                    async { system::load_ini_file(path).await?.try_into() },
                    Message::LoadedBfs,
                ),
            },
            Message::LoadedBfs(result) => {
                self.cheats.set_state(result.unwrap().cheats); // TODO error handling
                Command::none()
            }
            Message::Cheats(message) => self.cheats.update(message),
            Message::Options(message) => self.options.update(message),
            Message::Files(_message) => todo!(),
        }
    }

    fn view(&self) -> Element<'_, Self::Message, Self::Theme, Renderer> {
        Column::new()
            .push(self.save.view())
            .push(self.bfs.view())
            .push(
                Tabs::new(Message::TabSelected)
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
