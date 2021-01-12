/**
 * @file WhipseeySaveManager.cpp
 * @brief this is the main file for WhispeeySaveManager, tying all functionality together
 */

#include <SimpleIni.h>
#include <winreg/WinReg.hpp>

#include <ShlObj.h>

#include <iostream>

int main()
{
	PWSTR ppszPath;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, KNOWN_FOLDER_FLAG::KF_FLAG_DEFAULT, NULL, &ppszPath);
	if(hr != S_OK)
	{
		std::cerr << "error getting appdata path" << std::endl;
		return -1;
	}

	std::wstring savePath(ppszPath);
	savePath.append(LR"(\Whipseey\savedata\whipseey.sav)");

	CSimpleIniA ini;

	SI_Error rc = ini.LoadFile(savePath.data());
	if(rc != SI_Error::SI_OK)
	{
		std::cerr << "load save failed, not si-ok" << std::endl;
		return -1;
	}

	// CSimpleIniA::TNamesDepend sections;
	// ini.GetAllSections(sections);
	// for(auto& entryS : sections)
	// {
	// 	std::cout << "Line: " << entryS.nOrder << " Section: " << entryS.pItem << std::endl;
	//
	// 	CSimpleIniA::TNamesDepend keys;
	// 	ini.GetAllKeys(entryS.pItem, keys);
	// 	for(auto& entryK : keys)
	// 	{
	// 		std::cout << "  Line: " << entryK.nOrder << " Key: " << entryK.pItem << " Value: "
	// 			<< ini.GetValue(entryS.pItem, entryK.pItem, nullptr, nullptr) << std::endl;
	// 	}
	// }

	//! does not work with "" double moon = ini.GetDoubleValue("file1", "moon");

	using namespace winreg;

	RegKey key{HKEY_LOCAL_MACHINE, LR"(SOFTWARE\WOW6432Node\Valve\Steam)"};
	

	return 0;
}
