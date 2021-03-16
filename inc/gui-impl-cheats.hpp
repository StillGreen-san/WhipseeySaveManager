#pragma once

#include "gui-impl-common.hpp"

#include <nana/gui/widgets/checkbox.hpp>

namespace WhipseeySaveManager::GUI
{
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

		CheatsBox(nana::window wd);
		nana::basic_event<nana::arg_checkbox>& onEnabledChanged();
		void update(INI::Cheats& cheats);
	};

	class TabCheats : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		PathControls path{*this, {{"INI (*.ini)", "*.ini"}}};
		CheatsBox cheats{*this};
		TabCheats(nana::window wd, const std::shared_ptr<INI::Settings>& sttngs, const GUI::FunctionStore& callbacks);
		nana::basic_event<nana::arg_click>& onReload();
		nana::basic_event<nana::arg_click>& onSave();
	};
}
