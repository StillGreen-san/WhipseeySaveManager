#pragma once

#include "gui.hpp"

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/textbox.hpp>

namespace WhipseeySaveManager::GUI
{
/**
 * @brief a control containing textbox for a file path and buttons to open save & reload
 *
 */
class PathControls : public nana::panel<false>
{
	nana::place place{*this};
	nana::textbox filePath{*this};
	nana::button openFile{*this, "..."};
	nana::button saveFile{*this, "save"};
	nana::button reloadFile{*this, "reload"};
	std::vector<std::pair<std::string, std::string>> additionalFilters;

	void verifyPath(const std::filesystem::path& path);
	void open();

public:
	/**
	 * @brief Construct a new Path Controls object
	 *
	 * @param wd parent window
	 * @param filters filetype filter for file open, pair<name, filter>. to specify multiple filters, use a semicolon
	 * ("*.TXT;*.DOC;*.BAK"), filter for all file supplied by default
	 * @param path the initial path
	 */
	PathControls(
	    nana::window wd, std::vector<std::pair<std::string, std::string>>&& filters, std::filesystem::path path = {});

	/**
	 * @brief returns the path currently entered in the textbox
	 *
	 * @return std::filesystem::path the path
	 */
	std::filesystem::path getPath() const;

	/**
	 * @brief sets the path in the textbox
	 *
	 * @param path the path to set
	 */
	void setPath(const std::filesystem::path& path);

	/**
	 * @brief returns event reference to reload clicked
	 *
	 * @return nana::basic_event<nana::arg_click>& the event ref
	 */
	nana::basic_event<nana::arg_click>& onReload();

	/**
	 * @brief returns event reference to save clicked
	 *
	 * @return nana::basic_event<nana::arg_click>& the event ref
	 */
	nana::basic_event<nana::arg_click>& onSave();
};

/**
 * @brief displays a popup with information on the error(s) in error, if any
 *
 * @param error Error obj to show info on
 */
void showErrorMsg(const Types::Error& error);
} // namespace WhipseeySaveManager::GUI
