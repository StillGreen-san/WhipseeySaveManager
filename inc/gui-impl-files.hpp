#pragma once

#include "gui-impl-common.hpp"

#include <nana/gui/widgets/group.hpp>

namespace WhipseeySaveManager::GUI
{
/**
 * @brief group control that displays info of FileBase
 *
 */
class FileGroup : public nana::group
{
public:
	using nana::group::group;
	void update(INI::FileBase& file);
};

/**
 * @brief group control with radio boxes for all levels
 *
 */
class ProgressGroup : public nana::group
{
public:
	explicit ProgressGroup(nana::window wnd);
	static size_t levelToIndex(Types::Level level);
	static Types::Level indexToLevel(size_t index);
	void update(INI::FileBase& file);
	using nana::group::option_check;
	void option_check(Types::Level level);
};

/**
 * @brief textbox control that only accepts numbers and keeps its value between min & max
 *
 */
class NumericTextbox : public nana::textbox
{
public:
	int min;
	int max;
	NumericTextbox(nana::window parent, int minVal, int maxVal);
};

/**
 * @brief control with various setting controls for a 'saveslot'
 *
 */
class FileBox : public nana::panel<false>
{ // TODO use def,min,max values from ini.hpp or add other common defines
	nana::place place{*this};
	FileGroup group{*this, "FileX  0 - 0"}; //! placeholder text required
	ProgressGroup progress{group};
	nana::checkbox intro{group, "Intro"};
	nana::checkbox ending{group, "Ending"};
	nana::label lgems{group, "Gems"};
	nana::label llives{group, "Lives"};
	NumericTextbox tgems{group, 0, 99}; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
	NumericTextbox tlives{
	    group, 1, 16777215}; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
	nana::button bgems{group, "cycle"};
	nana::button blives{group, "cycle"};
	nana::button max{group, "max"};
	nana::button reset{group, "reset"};
	nana::button save{group, "save"};
	nana::button reload{group, "reload"};

	void update(INI::FileBase& file);
	void get(INI::FileBase& file) const;
	nana::basic_event<nana::arg_click>& onSave();
	nana::basic_event<nana::arg_click>& onReload();

public:
	explicit FileBox(nana::window wnd);

	friend class TabFiles;
};

/**
 * @brief tab control with PathControls & 3 FileBox
 *
 */
class TabFiles : public nana::panel<false>
{
	nana::place place{*this};
	PathControls path{*this, {{"Save (*.sav)", "*.sav"}}};
	FileBox file1{*this};
	FileBox file2{*this};
	FileBox file3{*this};

	void update(INI::Save& save);

public:
	TabFiles(nana::window wnd, const std::shared_ptr<INI::Save>& save, const GUI::FunctionStore& callbacks);
};
} // namespace WhipseeySaveManager::GUI
