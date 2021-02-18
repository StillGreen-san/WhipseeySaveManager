#include "system.hpp"

#define UNICODE
#include <winreg/WinReg.hpp>
#include <ShlObj.h>

#include <fstream>
#include <regex>

#include <SimpleIni.h>

namespace WhipseeySaveManager::System
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
		const std::wstring steamKey = LR"(SOFTWARE\Valve\Steam)";
		const std::wstring steamSZ = LR"(InstallPath)";

		std::filesystem::path steamPath;
		winreg::RegKey regHandler;

		if(regHandler.TryOpen(HKEY_LOCAL_MACHINE, steamKey, KEY_READ | KEY_WOW64_32KEY))
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

		const std::filesystem::path settingsRelativePath("steamapps/common/Whipseey and the Lost Atlas/bfs_settings.ini");
		std::filesystem::path settingsPath(steamPath / settingsRelativePath);

		if(std::filesystem::exists(settingsPath))
		{
			errPath.data.assign(std::move(settingsPath));
			return errPath;
		}

		const std::filesystem::path librariesFilePath(steamPath / "steamapps/libraryfolders.vdf");

		if(std::filesystem::exists(librariesFilePath) == false)
		{
			errPath.error.code = Types::Error::Code::SteamLibrariesNotFound;
			return errPath;
		}

		std::ifstream librariesFile(librariesFilePath);
		const std::regex reg(R"#(.*"\d+".*"(.*)")#", std::regex_constants::optimize);
		std::smatch match;

		for(std::string line; std::getline(librariesFile, line);)
		{
			if(std::regex_match(line, match, reg) && match.size() == 2)
			{
				settingsPath.assign(match[1].str() / settingsRelativePath);
				if(std::filesystem::exists(settingsPath))
				{
					errPath.data.assign(std::move(settingsPath));
					return errPath;
				}
			}
		}
		librariesFile.close();

		errPath.error.code = Types::Error::Code::GameNotFound;
		return errPath;
	}

	namespace INI
	{
		constexpr int NOT_FOUND = -1;
		namespace bfs_settings
		{
		constexpr char* Cheats = "Cheats";
		constexpr char* cheats_enabled = "cheats_enabled";
		constexpr long NOT_FOUND = -1;
		} // namespace bfs_settings
	} // namespace INI::bfs_settings

	Types::ErrDat<Types::Settings> readSettings(const std::filesystem::path& settings) 
	{
		Types::ErrDat<Types::Settings> errSettings;
		
		CSimpleIniA ini;
		SI_Error er = ini.LoadFile(settings.c_str());
		if(er != SI_OK)
		{
			errSettings.error.code = Types::Error::Code::FailedToLoadSettings;
			return errSettings;
		}

		int section = ini.GetSectionSize(INI::bfs_settings::Cheats);
		if(section == INI::NOT_FOUND)
		{
			errSettings.error.code = Types::Error::Code::CheatsSectionNotFound;
			return errSettings;
		}

		long cheats = ini.GetLongValue(
			INI::bfs_settings::Cheats,
			INI::bfs_settings::cheats_enabled,
			INI::bfs_settings::NOT_FOUND
		);
		if(cheats == INI::bfs_settings::NOT_FOUND)
		{
			errSettings.error.code = Types::Error::Code::CheatsKeyNotFound;
			return errSettings;
		}

		errSettings.data.cheats = cheats == 0 ? Types::Toggle::Disabled : Types::Toggle::Enabled;
		
		return errSettings;
	}
}
