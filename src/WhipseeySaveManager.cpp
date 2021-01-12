/**
 * @file WhipseeySaveManager.cpp
 * @brief this is the main file for WhispeeySaveManager, tying all functionality together
 */

#include <SimpleIni.h>
#include <vdf_parser.hpp>

#include <ShlObj.h>
#include <winreg.h>

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
	
	std::wstring wchrBuffer;
	DWORD bufferSizeBytes = 60 * sizeof(wchar_t);
	wchrBuffer.reserve(bufferSizeBytes / sizeof(wchar_t));

	LSTATUS st = RegGetValueW(HKEY_LOCAL_MACHINE, LR"(SOFTWARE\WOW6432Node\Valve\Steam\)", L"InstallPath", RRF_RT_REG_SZ, NULL, wchrBuffer.data(), &bufferSizeBytes);
	if(st == ERROR_MORE_DATA)
	{
		wchrBuffer.reserve(bufferSizeBytes / sizeof(wchar_t));
		st = RegGetValueW(HKEY_LOCAL_MACHINE, LR"(SOFTWARE\WOW6432Node\Valve\Steam\)", L"InstallPath", RRF_RT_REG_SZ, NULL, wchrBuffer.data(), &bufferSizeBytes);
	}
	if(st != ERROR_SUCCESS)
	{
		std::cerr << "reg read failed" << std::endl;
		return -1;
	}

	std::wstring librariesVdf(wchrBuffer.data(), bufferSizeBytes / sizeof(wchar_t) - 1);

	using namespace tyti;

	librariesVdf.append(LR"(\steamapps\libraryfolders.vdf)");
	std::ifstream vdf(librariesVdf);
	auto root = vdf::read(vdf);
	if(root.name == "LibraryFolders")
	{
		for(auto& attrib : root.attribs)
		{
			if(std::isdigit(attrib.first.front()))
			{
				std::cout << attrib.first << " : " << attrib.second << std::endl;
			}
		}
	}

	return 0;
}
