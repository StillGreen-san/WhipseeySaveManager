#include "gui-impl-files.hpp"


namespace WhipseeySaveManager::GUI
{
	void FileGroup::update(INI::FileBase& file)
	{
		std::string title(file.section());
		title.append("  ")
			.append(std::to_string(file.getBossNoDamageProgress().asInt()))
			.append(" - ")
			.append(std::to_string(file.getEnemiesDefeated().asInt()));
		caption(title);
	}

	ProgressGroup::ProgressGroup(nana::window wd) :
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

	size_t ProgressGroup::levelToIndex(Types::Level level)
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

	void ProgressGroup::update(INI::FileBase& file)
	{
		option_check(levelToIndex(file.getLevel()), true);
	}

	void ProgressGroup::option_check(Types::Level level)
	{
		option_check(levelToIndex(level), true);
	}

	FileBox::FileBox(nana::window wd) : nana::panel<false>(wd)
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

	void FileBox::update(INI::FileBase& file)
	{
		group.update(file);
		progress.update(file);
		intro.check(file.getIntro() == Types::Intro::Watched);
		ending.check(file.getEnding() == Types::Ending::Watched);
		tgems.caption(std::to_string(file.getGems()));
		tlives.caption(std::to_string(file.getLives()));
	}
	
	nana::basic_event<nana::arg_click>& FileBox::onSave() 
	{
		return save.events().click;
	}
	
	nana::basic_event<nana::arg_click>& FileBox::onReload() 
	{
		return reload.events().click;
	}

	TabFiles::TabFiles(nana::window wd, const std::shared_ptr<INI::Save>& , const GUI::FunctionStore& ) :
		nana::panel<false>(wd)
	{
		place.div("vert <path gap=5 margin=5 weight=35><files gap=3 margin=5>");
		place["path"] << path;
		place["files"] << file1 << file2 << file3;
		// file1.onReload().connect_front([&](nana::arg_click){
		// 	callbacks.onReadSection(save->getFile1(), path.getPath());
		// 	file1.update(*save->getFile1());
		// });
		// file1.onSave().connect_front([&](nana::arg_click){
		// 	file1.update(*save->getFile1());
		// });
	}

	void TabFiles::update(INI::Save& save)
	{
		file1.update(*save.getFile1());
		file2.update(*save.getFile2());
		file3.update(*save.getFile3());
	}
}
