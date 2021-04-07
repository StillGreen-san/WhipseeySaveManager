#pragma once

#include "gui-impl-common.hpp"

#include <nana/gui/widgets/checkbox.hpp>

namespace WhipseeySaveManager::GUI
{
/**
 * @brief control containing a checkbox and (long) description label for cheats
 *
 */
class CheatsBox : public nana::panel<false>
{
	nana::place place{*this};
	nana::checkbox cheatsEnabled{*this, "cheats enabled"};
	nana::label description{
	    *this, "checking \"cheats enabled\" will enable some hotkeys in game:"
	           "\nR  : restart room"
	           "\nN  : next room"
	           "\nP  : toggle fullscreen"
	           "\n, . , .  : infinite flight"
	           "\n, . , ,  : unlock all levels"
	           "\n, , , .  : disable hud"
	           "\n, , , ,  : invincibility"};

	void update(INI::Cheats& cheats);

public:
	/**
	 * @brief returns event reference to checkbox changed
	 *
	 * @return nana::basic_event<nana::arg_checkbox>& the event ref
	 */
	nana::basic_event<nana::arg_checkbox>& onEnabledChanged();

	CheatsBox(nana::window wd);

	friend class TabCheats;
};

/**
 * @brief tab control with PathControls & CheatsBox
 *
 */
class TabCheats : public nana::panel<false>
{
	nana::place place{*this};
	PathControls path{*this, {{"INI (*.ini)", "*.ini"}}};
	CheatsBox cheats{*this};

public:
	TabCheats(nana::window wd, const std::shared_ptr<INI::Settings>& sttngs, const GUI::FunctionStore& callbacks);
};
} // namespace WhipseeySaveManager::GUI
