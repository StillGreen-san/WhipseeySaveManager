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
	}

	std::filesystem::path PathControls::getPath()
	{
		return filePath.caption_native();
	}

	void PathControls::setPath(const std::filesystem::path& path)
	{
		filePath.caption(path.native());
		if(std::filesystem::exists(path))//TODO check when typing, long path field height
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

	nana::basic_event<nana::arg_click>& PathControls::onSave()
	{
		return saveFile.events().click;
	}

	nana::basic_event<nana::arg_click>& PathControls::onReload()
	{
		return reloadFile.events().click;
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