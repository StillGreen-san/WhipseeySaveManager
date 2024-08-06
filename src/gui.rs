use iced::widget::Column;
use iced::{Application, Command, Element, Renderer};
use iced_aw::{TabLabel, Tabs};
use std::path::PathBuf;

use crate::gui::about::About;
use crate::gui::file::DisplayStrings;

pub mod about;
pub mod cheats;
pub mod file;
pub mod files;
pub mod options;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum TabId {
    #[default]
    About,
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
    Open(FileId, PathBuf),
    Reload(FileId, PathBuf),
}

pub struct Gui {
    active_tab: TabId,
    about: About,
    save: file::File,
    bfs: file::File,
}

type Theme = iced::Theme;

impl Application for Gui {
    type Executor = iced::executor::Default;
    type Message = Message;
    type Theme = Theme;
    type Flags = ();

    fn new(_flags: Self::Flags) -> (Self, Command<Self::Message>) {
        let save_strings = DisplayStrings {
            placeholder: "save game",
            open: "...",
            save: "save",
            reload: "reload",
            dialog_title: "save game",
            dialog_filter_save: "save",
            dialog_filter_all: "all",
        }; // TODO localize
        let bfs_strings = DisplayStrings {
            placeholder: "bfs settings",
            dialog_title: "bfs settings",
            ..save_strings
        };
        (
            Self {
                active_tab: Default::default(),
                about: Default::default(),
                save: file::File::new(FileId::Save, save_strings),
                bfs: file::File::new(FileId::Bfs, bfs_strings),
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
            Message::Save(_, _) => todo!(),
            Message::Open(_, _) => todo!(),
            Message::Reload(_, _) => todo!(),
        }
    }

    fn view(&self) -> Element<'_, Self::Message, Self::Theme, Renderer> {
        Column::new()
            .push(self.save.view())
            .push(self.bfs.view())
            .push(
                Tabs::new(Message::TabSelected)
                    .set_active_tab(&self.active_tab)
                    .push(TabId::About, self.about.tab_label(), self.about.view()),
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
