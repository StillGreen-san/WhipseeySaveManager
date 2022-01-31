#include "gui-impl-common.hpp"

#include <nana/gui/filebox.hpp>

#include <set>

namespace WhipseeySaveManager::GUI
{
PathControls::PathControls(
    nana::window wd, std::vector<std::pair<std::string, std::string>>&& filters, std::filesystem::path path) :
    nana::panel<false>(wd),
    additionalFilters{std::move(filters)}
{
	place.div("this fit gap=5");
	place["this"] << filePath << openFile << saveFile << reloadFile;

	setPath(path);
	filePath.events().text_changed.connect_front(
	    [&]([[maybe_unused]] const nana::arg_textbox& textbox)
	    {
		    verifyPath(getPath());
	    });

	openFile.tooltip("Open and Load a new File");
	openFile.events().click.connect_front(
	    [&]([[maybe_unused]] const nana::arg_click& click)
	    {
		    open();
	    });

	saveFile.tooltip("Save current Values to File");
	reloadFile.tooltip("Reload Values from File");
}

std::filesystem::path PathControls::getPath() const
{
	return filePath.caption_native();
}

constexpr nana::colors validBG = nana::colors::white;
constexpr nana::colors invalidBG = nana::colors::firebrick;

void PathControls::verifyPath(const std::filesystem::path& path)
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
}

nana::basic_event<nana::arg_click>& PathControls::onReload()
{
	return reloadFile.events().click;
}

nana::basic_event<nana::arg_click>& PathControls::onSave()
{
	return saveFile.events().click;
}

void PathControls::open()
{
	nana::filebox ofd(*this, true);
	if(auto path = filePath.getline(0))
	{
		ofd.init_file(*path);
	}
	ofd.add_filter(additionalFilters);
	ofd.add_filter("All (*.*)", "*.*");

	auto paths = ofd.show();

	if(paths.size() == 1)
	{
		setPath(paths[0]);
	}
}

void showErrorMsg(const Types::Error& error)
{
	if(!error)
	{
		return;
	}

	std::set<Types::Error::Code> uniqueCodes(error.begin(), error.end());

	nana::msgbox msgbox("Error!");
	msgbox << "The following errors occurred at least once:\n\n";

	for(Types::Error::Code code : uniqueCodes)
	{
		switch(code)
		{
		case Types::Error::Code::InvalidFormat:
			msgbox << "InvalidFormat: a value of a key was in the wrong format, default value used\n";
			break;
		case Types::Error::Code::InvalidValue:
			msgbox << "InvalidValue: a value read was not valid for a key, default value used\n";
			break;
		case Types::Error::Code::SectionNotFound:
			msgbox << "SectionNotFound: a section was not found in the file, default values used\n";
			break;
		case Types::Error::Code::KeyNotFound:
			msgbox << "KeyNotFound: a key was not found in the file, default value used\n";
			break;
		case Types::Error::Code::FailedToLoadFile:
			msgbox << "FailedToLoadFile: The file could not be read, check existence and permissions\n";
			break;
		case Types::Error::Code::FailedToWriteFile:
			msgbox << "FailedToWriteFile: The file could not be written, check permissions\n";
			break;
		case Types::Error::Code::MissingCallback:
			msgbox << "MissingCallback: Yell at the developer if this happens in a shipping build!\n";
			break;
		default:
			msgbox << "Unknown: Yell at the developer if this happens in a shipping build!\n";
		}
	}
	msgbox << "\nMake sure you have selected the right file!"
	          "\nSaving here will return the file to a valid state. Saving in game will only work for the save game.";

	msgbox.show();
}
} // namespace WhipseeySaveManager::GUI
