#pragma once

#include "gui-impl-common.hpp"

#include <nana/gui/widgets/group.hpp>

namespace WhipseeySaveManager::GUI
{
	class FileGroup : public nana::group
	{
	public:
		using nana::group::group;
		void update(INI::FileBase& file);
	};

	class ProgressGroup : public nana::group
	{
	public:
		ProgressGroup(nana::window wd);
		size_t levelToIndex(Types::Level level);
		void update(INI::FileBase& file);
		using nana::group::option_check;
		void option_check(Types::Level level);
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

		FileBox(nana::window wd);

		void update(INI::FileBase& file);
	};

	class TabFiles : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		PathControls path{*this, {{"Save (*.sav)", "*.sav"}}};
		FileBox file1{*this};
		FileBox file2{*this};
		FileBox file3{*this};

		TabFiles(nana::window wd);

		void update(INI::Save& save);
	};
}
