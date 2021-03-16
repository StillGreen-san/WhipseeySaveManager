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

//TODO delegate reset&max to INI types
//TODO connect vs connect_front

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

		static constexpr nana::colors validBG = nana::colors::white;
		static constexpr nana::colors invalidBG = nana::colors::firebrick;

		PathControls(
			nana::window wd,
			std::vector<std::pair<std::string,std::string>>&& filters,
			std::filesystem::path path = {}
		) :
			nana::panel<false>(wd),
			additianalFilters{std::move(filters)}
		{
			place.div("this fit gap=5");
			place["this"] << filePath << openFile << saveFile << reloadFile;
			openFile.events().click.connect_front([&](nana::arg_click click){
				open();
			});
			setPath(path);
		}
		std::filesystem::path getPath()
		{
			return filePath.caption_native();
		}
		void setPath(const std::filesystem::path& path)
		{
			filePath.caption(path.native());
			if(std::filesystem::exists(path))//TODO check when typing, long path field height
			{
				filePath.scheme().background = validBG;
				saveFile.enabled(true);
				reloadFile.enabled(true);
			}
			else
			{
				filePath.scheme().background = invalidBG;
				saveFile.enabled(false);
				reloadFile.enabled(false);
			}
		}
		nana::basic_event<nana::arg_click>& onSave()
		{
			return saveFile.events().click;
		}
		nana::basic_event<nana::arg_click>& onReload()
		{
			return reloadFile.events().click;
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
			reloadFile.events().click.emit({}, *this);
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
				.append(std::to_string(file.getBossNoDamageProgress().asInt()))
				.append(" - ")
				.append(std::to_string(file.getEnemiesDefeated().asInt()));
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
		size_t levelToIndex(Types::Level level)
		{
			switch(level)
			{
			case Types::Level::Castle : return 0;
			case Types::Level::Moon : return 1;
			case Types::Level::Snow : return 2;
			case Types::Level::Desert : return 3;
			case Types::Level::Forest : return 4;
			case Types::Level::Beach :
			default : return 5;
			}
		}
		void update(INI::FileBase& file)
		{
			option_check(levelToIndex(file.getLevel()), true);
		}
		using nana::group::option_check;
		void option_check(Types::Level level)
		{
			option_check(levelToIndex(level), true);
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
		nana::button reset{group, "reset"};
		nana::button save{group, "save"};
		nana::button reload{group, "reload"};

		FileBox(nana::window wd) : nana::panel<false>(wd)
		{
			group.div(
			"<progress weight=80 margin=[5,5,5,5]>"
			"<vert gems weight=45 margin=[14,0,5,0] gap=5 arrange=[14,12,21,21,21,21]>"
			"<vert lives weight=70 margin=[14,5,5,5] gap=5 arrange=[14,12,21,21,21,21]>");
			group["progress"] << progress;
			group["gems"] << intro << lgems << tgems << bgems << max << save;
			group["lives"] << ending << llives << tlives << blives << reset << reload;
			group.collocate();
			place.div("this");
			place["this"] << group;
			bgems.events().click.connect([&](nana::arg_click){
				if(tgems.caption() == "99")
				{
					tgems.caption("0");
				}
				else
				{
					tgems.caption("99");
				}
			});
			blives.events().click.connect([&](nana::arg_click){
				const std::string lives = tlives.caption();
				if(lives == "9999")
				{
					tlives.caption("5");
				}
				else if(lives == "99")
				{
					tlives.caption("9999");
				}
				else
				{
					tlives.caption("99");
				}
			});
			reset.events().click.connect([&](nana::arg_click){
				tgems.caption("0");
				tlives.caption("5");
				intro.check(false);
				ending.check(false);
				progress.option_check(Types::Level::Beach);
			});
			max.events().click.connect([&](nana::arg_click){
				tgems.caption("99");
				tlives.caption("9999");
				intro.check(true);
				ending.check(true);
				progress.option_check(Types::Level::Castle);
			});
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
			place.div("things gap=8 arrange=[190,300]");
			place[FIELD] << label << combo;
		}
		static constexpr char* FIELD = "things";
		nana::basic_event<nana::arg_combox>& onChanged()
		{
			return combo.events().text_changed;
		}
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
			combo.option(options.getLanguage().asInt());
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
			combo.option(options.getScale().asInt() - 2);
		}
	};

	class OptionFullscreen : public OptionBase
	{
	public:
		OptionFullscreen(nana::window wd) : OptionBase(wd, "Fullscreen")
		{
			combo
				.push_back("Disabled")
				.push_back("Enabled");
		}
		void update(INI::Options& options)
		{
			combo.option(options.getFullscreen().asInt());
		}
	};

	class OptionLeftHanded : public OptionBase
	{
	public:
		OptionLeftHanded(nana::window wd) : OptionBase(wd, "LeftHanded")
		{
			combo
				.push_back("Disabled")
				.push_back("Enabled");
		}
		void update(INI::Options& options)
		{
			combo.option(options.getLeftHanded().asInt());
		}
	};

	class OptionSoundVolume : public OptionBase
	{
	public:
		OptionSoundVolume(nana::window wd) : OptionBase(wd, "SoundVolume")
		{
			combo
				.push_back("0%")
				.push_back("10%")
				.push_back("20%")
				.push_back("30%")
				.push_back("40%")
				.push_back("50%")
				.push_back("60%")
				.push_back("70%")
				.push_back("80%")
				.push_back("90%")
				.push_back("100%");
		}
		void update(INI::Options& options)
		{
			combo.option(options.getSoundVolume().asInt());
		}
	};

	class OptionSoundToggle : public OptionBase
	{
	public:
		OptionSoundToggle(nana::window wd) : OptionBase(wd, "SoundToggle")
		{
			combo
				.push_back("Disabled")
				.push_back("Enabled");
		}
		void update(INI::Options& options)
		{
			combo.option(options.getSoundToggle().asInt());
		}
	};

	class OptionMusicVolume : public OptionBase
	{
	public:
		OptionMusicVolume(nana::window wd) : OptionBase(wd, "MusicVolume")
		{
			combo
				.push_back("0%")
				.push_back("10%")
				.push_back("20%")
				.push_back("30%")
				.push_back("40%")
				.push_back("50%")
				.push_back("60%")
				.push_back("70%")
				.push_back("80%")
				.push_back("90%")
				.push_back("100%");
		}
		void update(INI::Options& options)
		{
			combo.option(options.getMusicVolume().asInt());
		}
	};

	class OptionMusicToggle : public OptionBase
	{
	public:
		OptionMusicToggle(nana::window wd) : OptionBase(wd, "MusicToggle")
		{
			combo
				.push_back("Disabled")
				.push_back("Enabled");
		}
		void update(INI::Options& options)
		{
			combo.option(options.getMusicToggle().asInt());
		}
	};

	class OptionsBox : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		OptionLanguage language{*this};
		OptionScale scale{*this};
		OptionFullscreen fullscreen{*this};
		OptionLeftHanded lefthanded{*this};
		OptionSoundVolume soundvolume{*this};
		OptionSoundToggle soundtoggle{*this};
		OptionMusicVolume musicvolume{*this};
		OptionMusicToggle musictoggle{*this};

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
			fullscreen.update(options);
			lefthanded.update(options);
			soundvolume.update(options);
			soundtoggle.update(options);
			musicvolume.update(options);
			musictoggle.update(options);
		}
	};

	class CheatsBox : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		nana::checkbox cheatsEnabled{*this, "cheats enabled"};
		nana::label description{*this,
			"checking \"cheats enabled\" will enable some hotkeys in game:"
			"\nR  : restart room"
			"\nN  : next room"
			"\nP  : toggle fullscreen"
			"\n, . , .  : infinite flight"
			"\n, . , ,  : unlock all levels"
			"\n, , , .  : disable hud"
			"\n, , , ,  : invincibility"};

		CheatsBox(nana::window wd) : nana::panel<false>(wd)
		{
			place.div("vert things margin=[0, 50] arrange=[40,variable]");
			place["things"] << cheatsEnabled << description;
		}
		nana::basic_event<nana::arg_checkbox>& onEnabledChanged()
		{
			return cheatsEnabled.events().checked;
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

		void update(INI::Save& save)
		{
			file1.update(save.getFile1());
			file2.update(save.getFile2());
			file3.update(save.getFile3());
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
		void update(INI::Save& save)
		{
			options.update(save.getOptions());
		}
	};

	class TabCheats : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		PathControls path{*this, {{"INI (*.ini)", "*.ini"}}};
		CheatsBox cheats{*this};
		std::shared_ptr<INI::Settings> settings;
		std::function<GUI::IniSignature> onReadIni;
		std::function<GUI::IniSignature> onWriteIni;
		TabCheats(nana::window wd, std::shared_ptr<INI::Settings> sttngs,
			std::function<GUI::IniSignature> cbReadIni, std::function<GUI::IniSignature> cbWriteIni
		) :
			nana::panel<false>(wd),
			settings{std::move(sttngs)},
			onReadIni{std::move(cbReadIni)},
			onWriteIni{std::move(cbWriteIni)}
		{
			place.div("vert <path gap=5 margin=5 weight=35><cheats margin=5>");
			place["path"] << path;
			place["cheats"] << cheats;
			cheats.onEnabledChanged().connect_front([&](nana::arg_checkbox cb){
				settings->getCheats().getCheatsEnabled() = static_cast<Types::CheatsEnabled>(cb.widget->checked());
			});
			if(onReadIni)
			{
				onReload().connect_front([&](nana::arg_click){
					Types::Error error = onReadIni(settings, path.getPath());
					//TODO show error
					cheats.update(settings->getCheats());
				});
			}
			if(onWriteIni)
			{
				onSave().connect_front([&](nana::arg_click){
					Types::Error error = onWriteIni(settings, path.getPath());
					//TODO show error
				});
			}
		}
		nana::basic_event<nana::arg_click>& onReload()
		{
			return path.reloadFile.events().click;
		}
		nana::basic_event<nana::arg_click>& onSave()
		{
			return path.saveFile.events().click;
		}
	};

	Types::Error GUI::run()
	{
		nana::form mainForm(nana::api::make_center(615, 255));
		mainForm.caption("Whipseey Save Manager");

		auto save = std::make_shared<INI::Save>();
		auto settings = std::make_shared<INI::Settings>();

		TabFiles files(mainForm);
		TabOptions options(mainForm);
		TabCheats cheats(mainForm, settings, callbacks.onReadIni, callbacks.onWriteIni);

		nana::tabbar<size_t> tabs(mainForm);
		tabs.append("Files", files);
		tabs.append("Options", options);
		tabs.append("Cheats", cheats);
		tabs.activated(0);

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
			options.update(*save);
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
