/**
 * @file WhipseeySaveManager.cpp
 * @brief this is the main file for WhispeeySaveManager, tying all functionality together
 */

#include <nana/gui.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/label.hpp>

#include "ini.hpp"

using namespace WhipseeySaveManager;

int WinMain()
{
	nana::form fm(nana::api::make_center(625, 255));
	fm.caption("Whipseey Save Manager");

	nana::textbox tbs(fm, "save file location");
	nana::button btso(fm, "...");
	nana::button btss(fm, "save");
	nana::button btse(fm, "reload");

	nana::textbox tbi(fm, "ini file location");
	nana::button btio(fm, "...");
	nana::button btis(fm, "save");
	nana::button btie(fm, "reload");

	nana::group grf1(fm, "File1 7-42069");
	nana::group grf1p(grf1, "Progress");
	grf1p.radio_mode(true);
	grf1p.add_option("Castle");
	grf1p.add_option("Moon");
	grf1p.add_option("Snow");
	grf1p.add_option("Desert");
	grf1p.add_option("Forest");
	grf1p.add_option("Beach");
	nana::checkbox grf1lc(grf1, "Intro");
	nana::checkbox grf1rc(grf1, "Ending");
	nana::label grf1ll(grf1, "Gems");
	nana::label grf1rl(grf1, "Lives");
	nana::textbox grf1lt(grf1, "99");
	nana::textbox grf1rt(grf1, "9999");
	nana::button grf1lbc(grf1, "cycle");
	nana::button grf1rbc(grf1, "cycle");
	nana::button grf1lbm(grf1, "max");
	nana::button grf1rbd(grf1, "delete");
	nana::button grf1lbs(grf1, "save");
	nana::button grf1rbr(grf1, "reload");
	
	grf1.div(
		"<progress margin=[5,5,5,5] weight=80 gap=5>"
		"<vert gems margin=[10,0,5,0] weight=45 gap=5>"
		"<vert life margin=[10,5,5,5] weight=70 gap=5>");
	grf1["progress"] << grf1p;
	grf1["gems"] << grf1lc << grf1ll << grf1lt << grf1lbc << grf1lbm << grf1lbs;
	grf1["life"] << grf1rc << grf1rl << grf1rt << grf1rbc << grf1rbd << grf1rbr;
	grf1.collocate();
	nana::group grf2(fm, "File2 7-42069");
	nana::group grf3(fm, "File3 7-42069");
	grf3.add_option(std::to_string(INI::test()));

	fm.div(
		"vert"
		"<save	margin=5 gap=5 weight=35 fit>"
		"<ini	margin=5 gap=5 weight=35 fit>"
		"<files	margin=5 gap=5>");
	fm["save"] << tbs << btso << btss << btse;
	fm["ini"] << tbi << btio << btis << btie;
	fm["files"] << grf1 << grf2 << grf3;
	fm.collocate();

	fm.show();
	nana::exec();
}

// #include "system.hpp"
// #include "gui.hpp"

// void testings()
// {
// 	using namespace WhipseeySaveManager;
// 	GUI::GUI gui;
// 	gui.connectOnDefaultSavePath(System::defaultSavePath);
// 	gui.connectOnDefaultSettingsPath(System::defaultSettingsPath);
// 	gui.connectOnReadFile(System::readFile);
// 	gui.connectOnReadOptions(System::readOptions);
// 	gui.connectOnReadSave(System::readSave);
// 	gui.connectOnReadSettings(System::readSettings);
// 	gui.connectOnSelectSavePath(System::selectSavePath);
// 	gui.connectOnSelectSettingsPath(System::selectSettingsPath);
// 	gui.connectOnWriteFile(System::writeFile);
// 	gui.connectOnWriteOptions(System::writeOptions);
// 	gui.connectOnWriteSave(System::writeSave);
// 	gui.connectOnWriteSettings(System::writeSettings);
// 	gui.run();
// }
