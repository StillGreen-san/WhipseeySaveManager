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
				errTheme += Types::Error::Code::ThemeDwordNotFound;
			}
		}
		else
		{
			errTheme += Types::Error::Code::ThemeKeyNotFound;
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
				errTheme += Types::Error::Code::ColorDwordNotFound;
			}
		}
		else
		{
			errTheme += Types::Error::Code::ColorKeyNotFound;
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
			errPath = Types::Error::Code::DefaultSaveNotFound;
			CoTaskMemFree(szPath);
			return errPath;
		}

		errPath.data += R"(\Whipseey\savedata\whipseey.sav)";
		if(std::filesystem::exists(errPath.data) == false)
		{
			errPath = Types::Error::Code::DefaultSaveNotFound;
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
				errPath = Types::Error::Code::SteamDwordNotFound;
				return errPath;
			}
		}
		else
		{
			errPath = Types::Error::Code::SteamKeyNotFound;
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
			errPath = Types::Error::Code::SteamLibrariesNotFound;
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

		errPath = Types::Error::Code::GameNotFound;
		return errPath;
	}

	namespace sINI
	{
		constexpr int SIZE_NOT_FOUND = -1;
		constexpr char* VALUE_NOT_FOUND = nullptr;
		constexpr long LONG_NOT_FOUND = -1;
		constexpr double DOUBLE_NOT_FOUND = -1.0;

		namespace bfs_settings
		{
			constexpr char* Cheats = "Cheats";
			constexpr char* cheats_enabled = "cheats_enabled";
		} // namespace bfs_settings

		namespace whipseey
		{
			constexpr char* options = "options";
			constexpr char* language = "language";
			constexpr char* scale = "scale";
			constexpr char* fullscreen = "fullscreen";
			constexpr char* left_handed = "left_handed";
			constexpr char* sound_volume = "sound_volume";
			constexpr char* sound_toggle = "sound_toggle";
			constexpr char* music_volume = "music_volume";
			constexpr char* music_toggle = "music_toggle";

			constexpr char* file[3] = {"file3", "file2", "file1"};
			constexpr char* boss_no_damage_progress = "boss_no_damage_progress";
			constexpr char* enemies_defeated = "enemies_defeated";
			constexpr char* castle = "castle";
			constexpr char* moon = "moon";
			constexpr char* snow = "snow";
			constexpr char* desert = "desert";
			constexpr char* forest = "forest";
			constexpr char* ending = "ending";
			constexpr char* intro = "intro";
			constexpr char* lives = "lives";
			constexpr char* gems = "gems";
		} // namespace whipseey

		template<typename T>
		void parseSaveValue(const CSimpleIniA& ini, T& data, Types::Error& error, Types::Error::Code code,
			const char* section, const char* key)
		{
			const char* rawValue = ini.GetValue(section, key, VALUE_NOT_FOUND);
			if(rawValue == VALUE_NOT_FOUND)
			{
				error += code;
				return;
			}
			long value = std::strtol(rawValue+1, nullptr, 10);
			data = static_cast<T>(value);
		}//TODO validation

		template<>
		void parseSaveValue(const CSimpleIniA& ini, Types::Volume& data, Types::Error& error, Types::Error::Code code,
			const char* section, const char* key)
		{
			const char* rawValue = ini.GetValue(section, key, VALUE_NOT_FOUND);
			if(rawValue == VALUE_NOT_FOUND)
			{
				error += code;
				return;
			}
			double value = std::strtod(rawValue+1, nullptr) * 10.0;
			data = static_cast<Types::Volume>(value);
		}//TODO validation

		template<>
		void parseSaveValue(const CSimpleIniA& ini, Types::Level& data, Types::Error& error, Types::Error::Code code,
			const char* section, const char* key)
		{
			const char* rawValue = ini.GetValue(section, key, VALUE_NOT_FOUND);
			if(rawValue == VALUE_NOT_FOUND)
			{
				error += code;
				return;
			}
			long value = std::strtol(rawValue+1, nullptr, 10);
			data = static_cast<Types::Level>(value | static_cast<long>(data));
		}//TODO validation
	} // namespace sINI

	Types::ErrDat<Types::Settings> readSettings(const std::filesystem::path& settings) 
	{
		Types::ErrDat<Types::Settings> errSettings;

		CSimpleIniA ini;
		SI_Error er = ini.LoadFile(settings.c_str());
		if(er != SI_OK)
		{
			errSettings = Types::Error::Code::FailedToLoadSettings;
			return errSettings;
		}

		int section = ini.GetSectionSize(sINI::bfs_settings::Cheats);
		if(section == sINI::SIZE_NOT_FOUND)
		{
			errSettings = Types::Error::Code::CheatsSectionNotFound;
			return errSettings;
		}

		long cheats = ini.GetLongValue(
			sINI::bfs_settings::Cheats,
			sINI::bfs_settings::cheats_enabled,
			sINI::LONG_NOT_FOUND
		);
		if(cheats == sINI::LONG_NOT_FOUND)
		{
			errSettings = Types::Error::Code::CheatsKeyNotFound;
			return errSettings;
		}
		if(cheats < 0 || cheats > 1)//TODO add proper validation (for all types)
		{
			errSettings = Types::Error::Code::CheatsKeyInvalid;
			return errSettings;
		}

		errSettings.data.cheats = cheats == 0 ? Types::Toggle::Disabled : Types::Toggle::Enabled;
		return errSettings;
	}
	
	Types::Error read(std::shared_ptr<INI::ISection> section, const std::filesystem::path& file) 
	{
		INI::INI ini;
		if(!ini.loadFile(file))
		{
			return ini.extractError();//TODO defaults if early error?
		}
		if(!ini.has(section))
		{
			return ini.extractError();
		}
		ini.read(section);
		return ini.extractError();
	}
	
	Types::ErrDat<Types::Options> readOptions(const std::filesystem::path& save) 
	{
		Types::ErrDat<Types::Options> errOpt;

		CSimpleIniA ini;//TODO create ini wrapper
		SI_Error er = ini.LoadFile(save.c_str());
		if(er != SI_OK)
		{
			errOpt = Types::Error::Code::FailedToLoadOptions;
			return errOpt;
		}

		int section = ini.GetSectionSize(sINI::whipseey::options);
		if(section == sINI::SIZE_NOT_FOUND)
		{
			errOpt = Types::Error::Code::OptionsSectionNotFound;
			return errOpt;
		}

		auto parse = [&](auto& data, Types::Error::Code code, const char* key)
		{
			sINI::parseSaveValue(
				ini, data, errOpt.error, code,
				sINI::whipseey::options, key
			);
		};

		parse(errOpt.data.language, Types::Error::Code::LanguageKeyNotFound, sINI::whipseey::language);
		parse(errOpt.data.scale, Types::Error::Code::ScaleKeyNotFound, sINI::whipseey::scale);
		parse(errOpt.data.fullScreen, Types::Error::Code::FullscreenKeyNotFound, sINI::whipseey::fullscreen);
		parse(errOpt.data.leftHanded, Types::Error::Code::LefthandedKeyNotFound, sINI::whipseey::left_handed);
		parse(errOpt.data.sound.volume, Types::Error::Code::SoundvolumeKeyNotFound, sINI::whipseey::sound_volume);
		parse(errOpt.data.sound.toggle, Types::Error::Code::SoundtoggleKeyNotFound, sINI::whipseey::sound_toggle);
		parse(errOpt.data.music.volume, Types::Error::Code::MusicvolumeKeyNotFound, sINI::whipseey::music_volume);
		parse(errOpt.data.music.toggle, Types::Error::Code::MusictoggleKeyNotFound, sINI::whipseey::music_toggle);

		return errOpt;
	}

	Types::ErrDat<Types::File> readFile(const std::filesystem::path& save, Types::FileIndex index) 
	{
		Types::ErrDat<Types::File> errFile;

		CSimpleIniA ini;
		SI_Error er = ini.LoadFile(save.c_str());
		if(er != SI_OK)
		{
			errFile = Types::Error::Code::FailedToLoadFile;
			return errFile;
		}

		int section = ini.GetSectionSize(sINI::whipseey::options);
		if(section == sINI::SIZE_NOT_FOUND)
		{
			errFile = Types::Error::Code::FileSectionNotFound;
			return errFile;
		}

		auto parse = [&](auto& data, Types::Error::Code code, const char* key)
		{
			sINI::parseSaveValue(
				ini, data, errFile.error, code,
				sINI::whipseey::file[static_cast<size_t>(index)], key
			);
		};

		parse(errFile.data.noDamage, Types::Error::Code::NodamageKeyNotFound, sINI::whipseey::boss_no_damage_progress);
		parse(errFile.data.noDamage, Types::Error::Code::NodamageKeyNotFound, sINI::whipseey::boss_no_damage_progress);
		parse(errFile.data.defeated, Types::Error::Code::DefeatedKeyNotFound, sINI::whipseey::enemies_defeated);
		parse(errFile.data.progress, Types::Error::Code::CastleKeyNotFound, sINI::whipseey::castle);
		parse(errFile.data.progress, Types::Error::Code::MoonKeyNotFound, sINI::whipseey::moon);
		parse(errFile.data.progress, Types::Error::Code::SnowKeyNotFound, sINI::whipseey::snow);
		parse(errFile.data.progress, Types::Error::Code::DesertKeyNotFound, sINI::whipseey::desert);
		parse(errFile.data.progress, Types::Error::Code::ForestKeyNotFound, sINI::whipseey::forest);
		parse(errFile.data.ending, Types::Error::Code::EndingKeyNotFound, sINI::whipseey::ending);
		parse(errFile.data.ending, Types::Error::Code::IntroKeyNotFound, sINI::whipseey::intro);
		parse(errFile.data.lives, Types::Error::Code::LivesKeyNotFound, sINI::whipseey::lives);
		parse(errFile.data.gems, Types::Error::Code::GemsKeyNotFound, sINI::whipseey::gems);

		return errFile;
	}
} // WhipseeySaveManager::System