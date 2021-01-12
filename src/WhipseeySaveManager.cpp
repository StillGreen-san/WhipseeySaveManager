/**
 * @file WhipseeySaveManager.cpp
 * @brief this is the main file for WhispeeySaveManager, tying all functionality together
 */

#include <SimpleIni.h>

#include <iostream>

int main()
{
	// const std::string savePath = R"(%USERPROFILE%\AppData\Local\Whipseey\savedata\whipseey.sav)";
	const std::string savePath = R"(C:\Users\#####\AppData\Local\Whipseey\savedata\whipseey.sav)";

	CSimpleIniA ini;

	SI_Error rc = ini.LoadFile(savePath.data());
	if(rc != SI_Error::SI_OK)
	{
		std::cerr << "load save failed, not si-ok" << std::endl;
		return -1;
	}

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);
	for(auto& entryS : sections)
	{
		std::cout << "Line: " << entryS.nOrder << " Section: " << entryS.pItem << std::endl;
		
		CSimpleIniA::TNamesDepend keys;
		ini.GetAllKeys(entryS.pItem, keys);
		for(auto& entryK : keys)
		{
			std::cout << "  Line: " << entryK.nOrder << " Key: " << entryK.pItem << " Value: "
				<< ini.GetValue(entryS.pItem, entryK.pItem, nullptr, nullptr) << std::endl;
		}
	}

	return 0;
}
