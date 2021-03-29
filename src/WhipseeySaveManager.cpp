/**
 * @file WhipseeySaveManager.cpp
 * @brief this is the main file for WhispeeySaveManager, tying all functionality together
 */

#include "system.hpp"
#include "gui.hpp"

using namespace WhipseeySaveManager;

//TODO rethink shared_pointer usage

int WinMain()
{
	GUI::GUI gui;

	gui.connectOnSystemTheme(System::systemTheme);
	gui.connectOnDefaultSavePath(System::defaultSavePath);
	gui.connectOnDefaultSettingsPath(System::defaultSettingsPath);
	gui.connectOnReadSection(static_cast<GUI::GUI::SectionSignature*>(System::read));
	gui.connectOnReadIni(static_cast<GUI::GUI::IniSignature*>(System::read));
	gui.connectOnWriteSection(static_cast<GUI::GUI::SectionSignature*>(System::write));
	gui.connectOnWriteIni(static_cast<GUI::GUI::IniSignature*>(System::write));

	return gui.run();
}
