#include "gui-impl-cheats.hpp"
#include "gui-impl-common.hpp"
#include "gui-impl-files.hpp"
#include "gui-impl-options.hpp"

#include <nana/gui/widgets/tabbar.hpp>

namespace WhipseeySaveManager::GUI
{
Types::Error GUI::run()
{
	if(!callbacks.onDefaultSavePath || !callbacks.onDefaultSettingsPath || !callbacks.onReadIni ||
	   !callbacks.onReadSection || !callbacks.onSystemTheme || !callbacks.onWriteIni || !callbacks.onWriteSection)
	{
		showErrorMsg(Types::Error::Code::MissingCallback);
		return Types::Error::Code::MissingCallback;
	}

	constexpr unsigned WINDOW_HIGHT = 615;
	constexpr unsigned WINDOW_WIDTH = 615;
	nana::form mainForm(
	    nana::API::make_center(WINDOW_HIGHT, WINDOW_WIDTH),
	    nana::appear::decorate<nana::appear::minimize, nana::appear::taskbar>());
	mainForm.caption("Whipseey Save Manager");

	auto save = std::make_shared<INI::Save>();
	auto settings = std::make_shared<INI::Settings>();

	TabFiles files(mainForm, save, callbacks);
	TabOptions options(mainForm, save, callbacks);
	TabCheats cheats(mainForm, settings, callbacks);

	nana::tabbar<size_t> tabs(mainForm);
	tabs.append("Files", files);
	tabs.append("Options", options);
	tabs.append("Cheats", cheats);
	tabs.activated(0);

	mainForm.div("vertical<tabbar weight=28><tabframe>");
	mainForm["tabbar"] << tabs;
	mainForm["tabframe"].fasten(files).fasten(options).fasten(cheats);
	mainForm.collocate();

	mainForm.show();
	nana::exec();

	return Types::Error::Code::Nothing;
}

void GUI::connectOnSystemTheme(std::function<GUI::ThemeSignature> func)
{
	callbacks.onSystemTheme = std::move(func);
}

void GUI::connectOnDefaultSavePath(std::function<GUI::PathSignature> func)
{
	callbacks.onDefaultSavePath = std::move(func);
}

void GUI::connectOnDefaultSettingsPath(std::function<GUI::PathSignature> func)
{
	callbacks.onDefaultSettingsPath = std::move(func);
}

void GUI::connectOnReadSection(std::function<GUI::SectionSignature> func)
{
	callbacks.onReadSection = std::move(func);
}

void GUI::connectOnReadIni(std::function<GUI::IniSignature> func)
{
	callbacks.onReadIni = std::move(func);
}

void GUI::connectOnWriteSection(std::function<GUI::SectionSignature> func)
{
	callbacks.onWriteSection = std::move(func);
}

void GUI::connectOnWriteIni(std::function<GUI::IniSignature> func)
{
	callbacks.onWriteIni = std::move(func);
}
} // namespace WhipseeySaveManager::GUI
