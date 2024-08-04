use iced::{Application, Command, Element, Renderer};
use iced_aw::{TabLabel, Tabs};

use crate::gui::about::About;

pub mod about;
pub mod cheats;
pub mod file;
pub mod files;
pub mod options;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
enum TabId {
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
}

#[derive(Default)]
pub struct Gui {
    active_tab: TabId,
    about: About,
}

type Theme = iced::Theme;

impl Application for Gui {
    type Executor = iced::executor::Default;
    type Message = Message;
    type Theme = Theme;
    type Flags = ();

    fn new(_flags: Self::Flags) -> (Self, Command<Self::Message>) {
        (Default::default(), Command::none())
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
            Message::File(id, message) => todo!(),
        }
    }

    fn view(&self) -> Element<'_, Self::Message, Self::Theme, Renderer> {
        Tabs::new(Message::TabSelected)
            .set_active_tab(&self.active_tab)
            .push(TabId::About, self.about.tab_label(), self.about.view())
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
