#pragma once

#include "gui.hpp"

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/panel.hpp>

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
		);
		std::filesystem::path getPath();
		void setPath(const std::filesystem::path& path);
		nana::basic_event<nana::arg_click>& onSave();
		nana::basic_event<nana::arg_click>& onReload();
		void open();
	};
}
