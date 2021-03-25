#include "gui-impl-common.hpp"

#include <nana/gui/filebox.hpp>

namespace WhipseeySaveManager::GUI
{
	PathControls::PathControls(
		nana::window wd,
		std::vector<std::pair<std::string,std::string>>&& filters,
		std::filesystem::path path
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
		filePath.events().text_changed.connect_front([&](nana::arg_textbox tb){
			varifyPath(getPath());
		});
	}

	std::filesystem::path PathControls::getPath() const
	{
		return filePath.caption_native();
	}

	void PathControls::varifyPath(const std::filesystem::path& path)
	{
		if(std::filesystem::exists(path))
		{
			filePath.scheme().background = validBG;
			saveFile.enabled(true);
			reloadFile.enabled(true);
			reloadFile.events().click.emit({}, *this);
		}
		else
		{
			filePath.scheme().background = invalidBG;
			saveFile.enabled(false);
			reloadFile.enabled(false);
		}
	}

	void PathControls::setPath(const std::filesystem::path& path)
	{
		filePath.caption(path.native());
		varifyPath(path);
	}

	void PathControls::open()
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
	}
}