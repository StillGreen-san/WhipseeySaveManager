#include "gui-impl-files.hpp"

#include <cwctype>

//TODO delegate reset to INI types

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
		case Types::Level::Beach : default : return 5;
		}
	}
	
	Types::Level ProgressGroup::indexToLevel(size_t index) 
	{
		switch(index)
		{
		case 0 : return Types::Level::Castle;
		case 1 : return Types::Level::Moon;
		case 2 : return Types::Level::Snow;
		case 3 : return Types::Level::Desert;
		case 4 : return Types::Level::Forest;
		case 5 : default : return Types::Level::Beach;
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

	constexpr wchar_t BACKSPACE = 0x08;
	constexpr wchar_t DELETE = 0x7f;
	
	NumericTextbox::NumericTextbox(nana::window parent, int minVal, int maxVal) :
		nana::textbox{parent, ""},
		min{minVal},
		max{maxVal}
	{
		set_accept([&](wchar_t chr)->bool{
			if(!this->selected())
			{
				if(chr == BACKSPACE)
				{
					if(this->to_int() / 10 >= min) return true;
					this->from(min);
					return false;
				}
				if(chr == DELETE)
				{
					std::string value = this->text();
					if(value.size() > 1)
					{
						value.erase(this->caret_pos().x, 1);
						if(std::stoi(value) >= min) return true;
					}
					this->from(min);
					return false;
				}
				if(std::iswdigit(chr))
				{
					std::string value = this->text();
					value.insert(this->caret_pos().x, 1, static_cast<char>(chr));
					int newValue = std::stoi(value);
					if(newValue <= max)
					{
						if(value.front() == '0') {
							this->from(newValue);
							return false;
						}
						return true;
					}
					this->from(max);
					return false;
				}
			}
			else
			{
				std::pair<nana::upoint, nana::upoint> range = this->selection();
				std::string value = this->text();
				if(chr == DELETE || chr == BACKSPACE)
				{
					if(value.size() > 1)
					{
						value.erase(range.first.x, range.second.x - range.first.x);
						if(std::stoi(value) >= min) return true;
					}
					this->from(min);
					return false;
				}
				if(std::iswdigit(chr))
				{
					value.erase(range.first.x + 1, (range.second.x - range.first.x) - 1);
					value[range.first.x] = static_cast<char>(chr);
					int newValue = std::stoi(value);
					if(newValue > max)
					{
						this->from(max);
						return false;
					}
					if(newValue < min)
					{
						this->from(min);
						return false;
					}
					if(value.front() == '0') {
						this->from(newValue);
						return false;
					}
					return true;
				}
			}
			return false;
		});
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
		intro.tooltip("Will skip Intro if checked");
		tgems.tooltip(std::to_string(tgems.min).append(" - ").append(std::to_string(tgems.max)));
		bgems.tooltip("Set Gems to 0 or 99");
		bgems.events().click.connect_front([&](nana::arg_click){
			if(tgems.caption() == "99")
			{
				tgems.caption("0");
			}
			else
			{
				tgems.caption("99");
			}
		});
		ending.tooltip("Only used to calculate %Progress");
		tlives.tooltip(std::to_string(tlives.min).append(" - ").append(std::to_string(tlives.max)));
		blives.tooltip("Set Lives to 0, 99 or 9999");
		blives.events().click.connect_front([&](nana::arg_click){
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
		reset.tooltip("New File");
		reset.events().click.connect_front([&](nana::arg_click){
			tgems.caption("0");
			tlives.caption("5");
			intro.check(false);
			ending.check(false);
			progress.option_check(Types::Level::Beach);
		});
		max.tooltip("100% File with 9999 Lives");
		max.events().click.connect_front([&](nana::arg_click){
			tgems.caption("99");
			tlives.caption("9999");
			intro.check(true);
			ending.check(true);
			progress.option_check(Types::Level::Castle);
		});
		save.tooltip("Save current Values to File for this Group");
		reload.tooltip("Reload Values from File for this Group");
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
	
	void FileBox::get(INI::FileBase& file) 
	{
		file.getIntro() = static_cast<Types::Intro>(intro.checked());
		file.getEnding() = static_cast<Types::Ending>(ending.checked());
		file.getGems() = static_cast<Types::Gems>(tgems.to_int());
		file.getLives() = static_cast<Types::Lives>(tlives.to_int());
		file.setLevel(progress.indexToLevel(progress.option()));
	}
	
	nana::basic_event<nana::arg_click>& FileBox::onSave() 
	{
		return save.events().click;
	}
	
	nana::basic_event<nana::arg_click>& FileBox::onReload() 
	{
		return reload.events().click;
	}

	TabFiles::TabFiles(nana::window wd, const std::shared_ptr<INI::Save>& save, const GUI::FunctionStore& callbacks) :
		nana::panel<false>(wd)
	{
		place.div("vert <path gap=5 margin=5 weight=35><files gap=3 margin=5>");
		place["path"] << path;
		place["files"] << file1 << file2 << file3;
		file1.onReload().connect_front([&](nana::arg_click){
			callbacks.onReadSection(save->getFile1(), path.getPath());
			file1.update(*save->getFile1());
		});
		file1.onSave().connect_front([&](nana::arg_click){
			file1.get(*save->getFile1());
			callbacks.onWriteSection(save->getFile1(), path.getPath());
		});
		file2.onReload().connect_front([&](nana::arg_click click){
			callbacks.onReadSection(save->getFile2(), path.getPath());
			file2.update(*save->getFile2());
		});
		file2.onSave().connect_front([&](nana::arg_click){
			file2.get(*save->getFile2());
			callbacks.onWriteSection(save->getFile2(), path.getPath());
		});
		file3.onReload().connect_front([&](nana::arg_click click){
			callbacks.onReadSection(save->getFile3(), path.getPath());
			file3.update(*save->getFile3());
		});
		file3.onSave().connect_front([&](nana::arg_click){
			file3.get(*save->getFile3());
			callbacks.onWriteSection(save->getFile3(), path.getPath());
		});
		path.reloadFile.events().click.connect_front([&](nana::arg_click){
			showErrorMsg(callbacks.onReadIni(save, path.getPath()));
			update(*save);
		});
		path.saveFile.events().click.connect_front([&](nana::arg_click){
			file1.get(*save->getFile1());
			file2.get(*save->getFile2());
			file3.get(*save->getFile3());
			showErrorMsg(callbacks.onWriteIni(save, path.getPath()));
		});
		std::optional<std::filesystem::path> savePath = callbacks.onDefaultSavePath();
		if(savePath)
		{
			path.setPath(*savePath);
			/*Types::Error error = */callbacks.onReadIni(save, *savePath);
		}
		update(*save);
	}

	void TabFiles::update(INI::Save& save)
	{
		file1.update(*save.getFile1());
		file2.update(*save.getFile2());
		file3.update(*save.getFile3());
	}
}
