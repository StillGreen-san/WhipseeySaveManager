#include "gui.hpp"

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/combox.hpp>

namespace WhipseeySaveManager::GUI
{
	class PathControls : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		nana::textbox filePath{*this};
		nana::button openFile{*this, "..."};
		nana::button saveFile{*this, "save"};
		nana::button reloadFile{*this, "reload"};
		std::vector<std::pair<std::string,std::string>> additianalFilters;

		PathControls(nana::window wd, std::vector<std::pair<std::string,std::string>> filters) :
			nana::panel<false>(wd),
			additianalFilters{std::move(filters)}
		{
			place.div("this fit gap=5");
			place["this"] << filePath << openFile << saveFile << reloadFile;
			openFile.events().click.connect([&](nana::arg_click click){
				open();
			});
		}

		void setPath(const std::filesystem::path& path)
		{
			filePath.caption(path.native());
		}

		void open()
		{
			nana::filebox ofd(*this, true);
			if(auto path = filePath.getline(0))
			{
				ofd.init_file(*path);
			}
			ofd.add_filter(additianalFilters);
			ofd.add_filter("All (*.*)", "*.*");
			auto paths = ofd.show();
			if(paths.size() == 1)
			{
				setPath(paths[0]);
			}
		}
	};

	class FileGroup : public nana::group
	{
	public:
		using nana::group::group;
		void update(INI::FileBase& file)
		{
			std::string title(file.section());
			title.append("  ")
				.append(std::to_string(file.getBossNoDamageProgress().as<uint32_t>()))
				.append(" - ")
				.append(std::to_string(file.getEnemiesDefeated().as<uint32_t>()));
			caption(title);
		}
	};

	class ProgressGroup : public nana::group
	{
	public:
		ProgressGroup(nana::window wd) :
			nana::group::group(wd, "progress")
		{
			radio_mode(true);
			add_option("Castle");
			add_option("Moon");
			add_option("Snow");
			add_option("Desert");
			add_option("Forest");
			add_option("Beach").check(true);
		}

		void update(INI::FileBase& file)
		{
			Types::Level level = file.getLevel();
			switch(level)
			{
			case Types::Level::Castle :
				option_check(0, true); break;
			case Types::Level::Moon :
				option_check(1, true); break;
			case Types::Level::Snow :
				option_check(2, true); break;
			case Types::Level::Desert :
				option_check(3, true); break;
			case Types::Level::Forest :
				option_check(4, true); break;
			case Types::Level::Beach :
				option_check(5, true); break;
			}
		}
	};

	class FileBox : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		FileGroup group{*this, "FileX  0 - 07"};//! placeholder text needed
		ProgressGroup progress{group};
		nana::checkbox intro{group, "Intro"};
		nana::checkbox ending{group, "Ending"};
		nana::label lgems{group, "Gems"};
		nana::label llives{group, "Lives"};
		nana::textbox tgems{group, "99"};
		nana::textbox tlives{group, "9999"};
		nana::button bgems{group, "cycle"};
		nana::button blives{group, "cycle"};
		nana::button max{group, "max"};
		nana::button remove{group, "delete"};
		nana::button save{group, "save"};
		nana::button reload{group, "reload"};

		FileBox(nana::window wd) : nana::panel<false>(wd)
		{
			group.div(
			"<progress weight=80 margin=[5,5,5,5]>"
			"<vert gems weight=45 margin=[5,0,5,0] gap=5>"
			"<vert lives weight=70 margin=[5,5,5,5] gap=5>");
			group["progress"] << progress;
			group["gems"] << intro << lgems << tgems << bgems << max << save;
			group["lives"] << ending << llives << tlives << blives << remove << reload;
			group.collocate();
			place.div("this");
			place["this"] << group;
		}

		void update(INI::FileBase& file)
		{
			group.update(file);
			progress.update(file);
			intro.check(file.getIntro() == Types::Intro::Watched);
			ending.check(file.getEnding() == Types::Ending::Watched);
			tgems.caption(std::to_string(file.getGems()));
			tlives.caption(std::to_string(file.getLives()));
		}
	};

	class LabeledTextBox : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		nana::label label;
		nana::textbox textBox;
		LabeledTextBox(nana::window wd, std::string_view labelText, std::string_view textboxText) :
			nana::panel<false>(wd),
			label{*this, labelText},
			textBox{*this, textboxText}
		{
			label.text_align(nana::align::right, nana::align_v::center);
			place.div("things gap=8 arrange=[150,variable]");
			place["things"] << label << textBox;
		}
	};

	class OptionBase : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		nana::label label;
		nana::combox combo{*this};
		OptionBase(nana::window wd, std::string_view labelText) :
			nana::panel<false>(wd),
			label{*this, labelText}
		{
			label.text_align(nana::align::right, nana::align_v::center);
			place.div("things gap=8 arrange=[150,variable]");
			place[FIELD] << label << combo;
		}
		static constexpr char* FIELD = "things";
	};

	class OptionLanguage : public OptionBase
	{
	public:
		OptionLanguage(nana::window wd) : OptionBase(wd, "Language")
		{
			combo
				.push_back("English")
				.push_back("Spanish")
				.push_back("French")
				.push_back("Italian")
				.push_back("German")
				.push_back("Russian")
				.push_back("Swedish")
				.push_back("Japanese")
				.push_back("Chinese")
				.push_back("Portogese");
		}
		void update(INI::Options& options)
		{
			combo.option(options.getLanguage().as<uint32_t>());
		}
	};

	class OptionScale : public OptionBase
	{
	public:
		OptionScale(nana::window wd) : OptionBase(wd, "Scale")
		{
			combo
				.push_back("R768x432")
				.push_back("R1152x648")
				.push_back("R1536x864");
		}
		void update(INI::Options& options)
		{
			combo.option(options.getScale().as<uint32_t>() - 2);
		}
	};

	class OptionsBox : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		OptionLanguage language{*this};
		OptionScale scale{*this};
		LabeledTextBox fullscreen{*this, "fullscreen", ""};
		LabeledTextBox lefthanded{*this, "lefthaded", ""};
		LabeledTextBox soundvolume{*this, "soundvolume", ""};
		LabeledTextBox soundtoggle{*this, "soundtoggle", ""};
		LabeledTextBox musicvolume{*this, "musicvolume", ""};
		LabeledTextBox musictoggle{*this, "musictoggle", ""};

		OptionsBox(nana::window wd) : nana::panel<false>(wd)
		{
			place.div("vert things gap=2");
			place["things"] << language << scale << fullscreen << lefthanded
				<< soundvolume << soundtoggle << musicvolume << musictoggle;
		}

		void update(INI::Options& options)
		{
			language.update(options);
			scale.update(options);
			fullscreen.textBox.caption(options.getFullscreen().toString());
			lefthanded.textBox.caption(options.getLeftHanded().toString());
			soundvolume.textBox.caption(options.getSoundVolume().toString());
			soundtoggle.textBox.caption(options.getSoundToggle().toString());
			musicvolume.textBox.caption(options.getMusicVolume().toString());
			musictoggle.textBox.caption(options.getMusicToggle().toString());
		}
	};

	class CheatsBox : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		nana::checkbox cheatsEnabled{*this, "cheats enabled"};
		nana::label description{*this,
			"checking Cheats will enable some hotkeys in game"
			"\nR  : restart room"
			"\nN  : next room"
			"\nP  : toggle fullscreen"
			"\n, . , .  : infinite flight"
			"\n, . , ,  : unlock all levels"
			"\n, , , .  : disable hud"
			"\n, , , ,  : invincibility"};

		CheatsBox(nana::window wd) : nana::panel<false>(wd)
		{
			place.div("vert things arrange=[40,variable]");
			place["things"] << cheatsEnabled << description;
		}

		void update(INI::Cheats& cheats)
		{
			cheatsEnabled.check(cheats.getCheatsEnabled() == Types::CheatsEnabled::Enabled);
		}
	};

	class TabFiles : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		PathControls path{*this, {{"Save (*.sav)", "*.sav"}}};
		FileBox file1{*this};
		FileBox file2{*this};
		FileBox file3{*this};

		TabFiles(nana::window wd) : nana::panel<false>(wd)
		{
			place.div("vert <path gap=5 margin=5 weight=35><files gap=3 margin=5>");
			place["path"] << path;
			place["files"] << file1 << file2 << file3;
		}

		void update(INI::Save& file)
		{
			file1.update(file.getFile1());
			file2.update(file.getFile2());
			file3.update(file.getFile3());
		}
	};

	class TabOptions : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		PathControls path{*this, {{"Save (*.sav)", "*.sav"}}};
		OptionsBox options{*this};
		TabOptions(nana::window wd) : nana::panel<false>(wd)
		{
			place.div("vert <path gap=5 margin=5 weight=35><options margin=5>");
			place["path"] << path;
			place["options"] << options;
		}
	};

	class TabCheats : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		PathControls path{*this, {{"INI (*.ini)", "*.ini"}}};
		CheatsBox cheats{*this};
		TabCheats(nana::window wd) : nana::panel<false>(wd)
		{
			place.div("vert <path gap=5 margin=5 weight=35><cheats margin=5>");
			place["path"] << path;
			place["cheats"] << cheats;
		}
	};

	Types::Error GUI::run()
	{
		nana::form mainForm(nana::api::make_center(615, 255));
		mainForm.caption("Whipseey Save Manager");

		TabFiles files(mainForm);
		TabOptions options(mainForm);
		TabCheats cheats(mainForm);

		nana::tabbar<size_t> tabs(mainForm);
		tabs.append("Files", files);
		tabs.append("Options", options);
		tabs.append("Cheats", cheats);
		tabs.activated(0);

		auto save = std::make_shared<INI::Save>();
		auto settings = std::make_shared<INI::Settings>();

		if(callbacks.onDefaultSavePath)
		{
			std::optional<std::filesystem::path> path = callbacks.onDefaultSavePath();
			if(path)
			{
				files.path.setPath(*path);
				options.path.setPath(*path);

				if(callbacks.onReadIni)
				{
					/*Types::Error error = */callbacks.onReadIni(save, *path);
				}
			}
			files.update(*save);
			options.options.update(save->getOptions());
		}
		if(callbacks.onDefaultSettingsPath)
		{
			std::optional<std::filesystem::path> path = callbacks.onDefaultSettingsPath();
			if(path)
			{
				cheats.path.setPath(*path);
			}
		}

		mainForm.div("vertical<tabbar weight=28><tabframe>");
		mainForm["tabbar"] << tabs;
		mainForm["tabframe"].fasten(files).fasten(options).fasten(cheats);
		mainForm.collocate();

		mainForm.show();
		nana::exec();

		return {};
	}

	void GUI::connectOnSystemTheme(std::function<GUI::ThemeSignature> func)
	{
		callbacks.onSystemTheme = func;
	}

	void GUI::connectOnDefaultSavePath(std::function<GUI::PathSignature> func)
	{
		callbacks.onDefaultSavePath = func;
	}

	void GUI::connectOnDefaultSettingsPath(std::function<GUI::PathSignature> func)
	{
		callbacks.onDefaultSettingsPath = func;
	}

	void GUI::connectOnReadSection(std::function<GUI::SectionSignature> func)
	{
		callbacks.onReadSection = func;
	}

	void GUI::connectOnReadIni(std::function<GUI::IniSignature> func)
	{
		callbacks.onReadIni = func;
	}

	void GUI::connectOnWriteSection(std::function<GUI::SectionSignature> func)
	{
		callbacks.onWriteSection = func;
	}

	void GUI::connectOnWriteIni(std::function<GUI::IniSignature> func)
	{
		callbacks.onWriteIni = func;
	}
}
