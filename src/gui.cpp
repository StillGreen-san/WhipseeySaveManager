#include "gui.hpp"

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/label.hpp>

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

		PathControls(nana::window wd) : nana::panel<false>(wd)
		{
			place.div("this fit gap=5");
			place["this"] << filePath << openFile << saveFile << reloadFile;
			filePath.events().click.connect([&](nana::arg_click click){
				open();
			});
		}

		void open()
		{
			nana::filebox ofd(*this, true);
			if(auto path = filePath.getline(0))
			{
				ofd.init_file(*path);
			}
			ofd.add_filter({
				{"Save", "*.sav"},
				{"All", "*.*"}
			});
			auto paths = ofd.show();
			if(paths.size() == 1)
			{
				filePath.caption(paths[0].native());
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
				.append(std::to_string(
					unsigned(Types::BossNoDamage(file.getBossNoDamageProgress()))//! not so nice
				))
				.append(" - ")
				.append(std::to_string(file.getEnemiesDefeated()));
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
				 if(file.getCastle() == Types::Castle::Cleared)
			{
				option_check(0, true);
			}
			else if(file.getMoon() == Types::Moon::Cleared)
			{
				option_check(1, true);
			}
			else if(file.getSnow() == Types::Snow::Cleared)
			{
				option_check(2, true);
			}
			else if(file.getDesert() == Types::Desert::Cleared)
			{
				option_check(3, true);
			}
			else if(file.getForest() == Types::Forest::Cleared)
			{
				option_check(4, true);
			}
			else //TODO add Types::Level conversion to FileBase?
			{
				option_check(5, true);
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
		nana::radio_group pgroup;

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
	
	Types::Error GUI::run()
	{
		nana::form mainForm(nana::api::make_center(615, 255));
		mainForm.caption("Whipseey Save Manager");

		PathControls pcsave(mainForm);
		if(callbacks.onDefaultSavePath)
		{
			auto path = callbacks.onDefaultSavePath();
			if(path)
			{
				pcsave.filePath.caption(path->native());
			}
		}
		// pcsave.openFile.events().click.connect([&](nana::arg_click){
		// 	nana::filebox fb(mainForm, true);
		// 	fb.show();
		// });
		PathControls pcset(mainForm);
		if(callbacks.onDefaultSettingsPath)
		{
			auto path = callbacks.onDefaultSettingsPath();
			if(path)
			{
				pcset.filePath.caption(path->native());
			}
		}

		FileBox fb1(mainForm);
		FileBox fb2(mainForm);
		FileBox fb3(mainForm);

		auto save = std::make_shared<INI::Save>();
		if(auto path = pcsave.filePath.getline(0))
		{
			Types::Error error = callbacks.onReadIni(save, *path);
			if(!error)
			{
				fb1.update(save->getFile1());
				fb2.update(save->getFile2());
				fb3.update(save->getFile3());
			}
		}

		mainForm.div("vertical<vertical paths gap=5 margin=5 weight=70><files gap=3 margin=5>");
		mainForm["paths"] << pcsave << pcset;
		mainForm["files"] << fb1 << fb2 << fb3;
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
