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
		std::shared_ptr<INI::Settings> settings;
		std::function<GUI::IniSignature> onReadIni;
		std::function<GUI::IniSignature> onWriteIni;
		TabCheats(nana::window wd, std::shared_ptr<INI::Settings> sttngs,
			std::function<GUI::IniSignature> cbReadIni, std::function<GUI::IniSignature> cbWriteIni
		);
		nana::basic_event<nana::arg_click>& onReload();
		nana::basic_event<nana::arg_click>& onSave();
	};
}
