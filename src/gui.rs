use crate::gui::about::About;
use crate::gui::cheats::Cheats;
use crate::{data, system};
use iced::widget::Column;
use iced::{Application, Command, Element, Renderer};
use iced_aw::{TabLabel, Tabs};
use std::path::PathBuf;

pub mod about;
pub mod cheats;
pub mod file;
pub mod files;
pub mod options;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum TabId {
    #[default]
    About,
    Cheats,
}

#[derive(Debug, Clone, Copy)]
pub enum FileId {
    Save,
    Bfs,
}

#[derive(Debug, Clone)]
pub enum Message {
    TabSelected(TabId),
    About(about::Message),
    File(FileId, file::Message),
    Save(FileId, PathBuf),
    Load(FileId, PathBuf),
    LoadedBfs(data::Result<data::BfsSettings>),
    Saved(FileId),
    Cheats(cheats::Message),
}

pub struct Gui {
    save: file::File,
    bfs: file::File,
    active_tab: TabId,
    about: About,
    cheats: Cheats,
}

type Theme = iced::Theme;

impl Application for Gui {
    type Executor = iced::executor::Default;
    type Message = Message;
    type Theme = Theme;
    type Flags = ();

    fn new(_flags: Self::Flags) -> (Self, Command<Self::Message>) {
        let save_strings = file::DisplayStrings {
            placeholder: "save game",
            open: "...",
            save: "save",
            reload: "reload",
            dialog_title: "save game",
            dialog_filter_file: "save",
            dialog_filter_ext: "sav",
            dialog_filter_all: "all",
        }; // TODO localize
        let bfs_strings = file::DisplayStrings {
            placeholder: "bfs settings",
            dialog_title: "bfs settings",
            dialog_filter_file: "ini",
            dialog_filter_ext: "ini",
            ..save_strings
        };
        let cheats_strings = cheats::DisplayStrings {
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
        (
            Self {
                active_tab: Default::default(),
                about: Default::default(),
                save: file::File::new(FileId::Save, save_strings),
                bfs: file::File::new(FileId::Bfs, bfs_strings),
                cheats: Cheats::new(cheats_strings),
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
            Message::File(id, message) => match id {
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
                        async move { system::file::write_ini_file(path, &ini).await },
                        |_| Message::Saved(FileId::Bfs),
                    )
                }
            },
            Message::Saved(_) => Command::none(), // TODO?
            Message::Load(id, path) => match id {
                FileId::Save => todo!(),
                FileId::Bfs => Command::perform(
                    async { system::file::load_ini_file(path).await?.try_into() },
                    Message::LoadedBfs,
                ),
            },
            Message::LoadedBfs(result) => {
                self.cheats.set_state(result.unwrap().cheats); // TODO error handling
                Command::none()
            }
            Message::Cheats(message) => self.cheats.update(message),
        }
    }

    fn view(&self) -> Element<'_, Self::Message, Self::Theme, Renderer> {
        Column::new()
            .push(self.save.view())
            .push(self.bfs.view())
            .push(
                Tabs::new(Message::TabSelected)
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
