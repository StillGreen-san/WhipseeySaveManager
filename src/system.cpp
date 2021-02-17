#include "system.hpp"

#define UNICODE
#include <winreg/WinReg.hpp>
#include <ShlObj.h>

#include <vdf_parser.hpp>

namespace WhipseeySaveManager
{
namespace System
{
	Types::ErrDat<Types::Theme> systemTheme() 
	{
		Types::ErrDat<Types::Theme> errTheme;
		const std::wstring themeKey = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\Personalize)";
		const std::wstring themeDW = LR"(AppsUseLightTheme)";
		const std::wstring colorKey = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\History\Colors)";
		const std::wstring colorDW = LR"(ColorHistory0)";

		winreg::RegKey regHandler;

		if(regHandler.TryOpen(HKEY_CURRENT_USER, themeKey, KEY_READ))
		{
			std::optional<DWORD> lightTheme = regHandler.TryGetDwordValue(themeDW);
			if(lightTheme.has_value())
			{
				errTheme.data.darkmode = lightTheme.value() ? Types::Toggle::Disabled : Types::Toggle::Enabled;
			}
			else
			{
				errTheme.error.code = Types::Error::Code::ThemeDwordNotFound;
			}
		}
		else
		{
			errTheme.error.code = Types::Error::Code::ThemeKeyNotFound;
		}
		
		if(regHandler.TryOpen(HKEY_CURRENT_USER, colorKey, KEY_READ))
		{
			std::optional<DWORD> color = regHandler.TryGetDwordValue(colorDW);
			if(color.has_value())
			{
				errTheme.data.accent = color.value();
			}
			else
			{
				errTheme.error.code = Types::Error::Code::ColorDwordNotFound;
			}
		}
		else
		{
			errTheme.error.code = Types::Error::Code::ColorKeyNotFound;
		}

		return errTheme;
	}
	
	Types::ErrDat<std::filesystem::path> defaultSavePath() 
	{
		Types::ErrDat<std::filesystem::path> errPath;
		PWSTR szPath = nullptr;
		HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, NULL, &szPath);//TODO create wrapper
		if(result == S_OK)
		{
			errPath.data.assign(szPath);
			CoTaskMemFree(szPath);
		}
		else
		{
			errPath.error.code = Types::Error::Code::DefaultSaveNotFound;
			CoTaskMemFree(szPath);
			return errPath;
		}

		errPath.data += R"(\Whipseey\savedata\whipseey.sav)";
		if(std::filesystem::exists(errPath.data) == false)
		{
			errPath.error.code = Types::Error::Code::DefaultSaveNotFound;
			errPath.data.clear();
		}

		return errPath;
	}
	
	Types::ErrDat<std::filesystem::path> defaultSettingsPath() 
	{
		Types::ErrDat<std::filesystem::path> errPath;
		const std::wstring steamKey = LR"(SOFTWARE\WOW6432Node\Valve\Steam)";//TODO make bit independent
		const std::wstring steamSZ = LR"(InstallPath)";

		std::filesystem::path steamPath;
		winreg::RegKey regHandler;

		if(regHandler.TryOpen(HKEY_LOCAL_MACHINE, steamKey, KEY_READ))
		{
			std::optional<std::wstring> installPath = regHandler.TryGetStringValue(steamSZ);
			if(installPath.has_value())
			{
				steamPath.assign(installPath.value());
			}
			else
			{
				errPath.error.code = Types::Error::Code::SteamDwordNotFound;
				return errPath;
			}
		}
		else
		{
			errPath.error.code = Types::Error::Code::SteamKeyNotFound;
			return errPath;
		}

		std::filesystem::path settingsRelativePath("steamapps/common/Whipseey and the Lost Atlas/bfs_settings.ini");
		std::filesystem::path settingsPath(steamPath / settingsRelativePath);

		if(std::filesystem::exists(settingsPath))
		{
			errPath.data.assign(std::move(settingsPath));
			return errPath;
		}

		std::filesystem::path librariesRelativePath("steamapps/libraryfolders.vdf");
		tyti::vdf::object librariesVDF;
		{
			std::ifstream librariesFile(steamPath / librariesRelativePath);
			tyti::vdf::object librariesVDF = tyti::vdf::read(librariesFile);
			librariesFile.close();
		}

		if(librariesVDF.name != "")
		{
			errPath.error.code = Types::Error::Code::SteamLibrariesNotFound;
			return errPath;
		}

		for(auto& attribute : librariesVDF.attribs)
		{
			if(std::isdigit(attribute.first.front()))
			{
				settingsPath.assign(steamPath / attribute.second);
				if(std::filesystem::exists(settingsPath))
				{
					errPath.data.assign(std::move(settingsPath));
					return errPath;
				}
			}
		}

		errPath.error.code = Types::Error::Code::GameNotFound;
		return errPath;
	}
}
}
