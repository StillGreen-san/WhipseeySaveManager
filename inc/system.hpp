#pragma once

#include "ini.hpp"

namespace WhipseeySaveManager::System
{
	/**
	 * @brief queries the os for theme info
	 * 
	 * @return Types::ErrDat<Types::Theme>: Theme always valid, 
	 * on error: code = ColorDwordNotFound, ColorKeyNotFound, ThemeDwordNotFound or ThemeKeyNotFound
	 */
	Types::ErrDat<Types::Theme> systemTheme();//TODO simplify

	/**
	 * @brief finds the default save path
	 * 
	 * @return Types::ErrDat<std::filesystem::path> if error path is empty and code = DefaultSaveNotFound
	 */
	Types::ErrDat<std::filesystem::path> defaultSavePath();//TODO simplify

	/**
	 * @brief finds the default bfs_settings.ini path
	 * 
	 * @return Types::ErrDat<std::filesystem::path> if error path is empty and code = 
	 * SteamKeyNotFound, SteamDwordNotFound, SteamLibrariesNotFound or GameNotFound
	 */
	Types::ErrDat<std::filesystem::path> defaultSettingsPath();//TODO simplify

	/**
	 * @brief reads all keys defined in section from file into ini
	 * 
	 * @param section INI::ISection object to read into
	 * @param file path to file to read from
	 * @return Types::Error if failed one or more FailedToLoadFile, KeyNotFound, SectionNotFound
	 */
	Types::Error read(std::shared_ptr<INI::ISection> section, std::filesystem::path file);

	/**
	 * @brief reads all keys defined in ini from file into ini
	 * 
	 * @param ini INI::IIni object to read into
	 * @param file path to file to read from
	 * @return Types::Error if failed one or more FailedToLoadFile, KeyNotFound, SectionNotFound
	 */
	Types::Error read(std::shared_ptr<INI::IIni> ini, std::filesystem::path file);

	Types::Error write(std::shared_ptr<INI::ISection> section, std::filesystem::path file);
	Types::Error write(std::shared_ptr<INI::IIni> ini, std::filesystem::path file);
} // namespace WhipseeySaveManager::System
