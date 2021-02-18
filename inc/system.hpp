#pragma once

#include "types.hpp"

namespace WhipseeySaveManager
{
/**
 * @brief contain functions that interact with the OS
 * 
 */
namespace System
{
	/**
	 * @brief queries the os for theme info
	 * 
	 * @return Types::ErrDat<Types::Theme>: theme always valid, 
	 * on error: code = ColorDwordNotFound, ColorKeyNotFound, ThemeDwordNotFound or ThemeKeyNotFound
	 */
	Types::ErrDat<Types::Theme> systemTheme();

	/**
	 * @brief finds the default save path
	 * 
	 * @return Types::ErrDat<std::filesystem::path> if error path is empty and code = DefaultSaveNotFound
	 */
	Types::ErrDat<std::filesystem::path> defaultSavePath();

	/**
	 * @brief finds the default settings path
	 * 
	 * @return Types::ErrDat<std::filesystem::path> if error path is empty and code = 
	 * SteamKeyNotFound, SteamDwordNotFound, SteamLibrariesNotFound or GameNotFound
	 */
	Types::ErrDat<std::filesystem::path> defaultSettingsPath();

	Types::ErrDat<Types::Save> readSave(std::filesystem::path);
	Types::ErrDat<Types::Settings> readSettings(std::filesystem::path);
	Types::ErrDat<Types::Options> readOptions(std::filesystem::path);
	Types::ErrDat<Types::File> readFile(std::filesystem::path, Types::FileIndex);
	Types::Error writeSave(std::filesystem::path, Types::Save);
	Types::Error writeSettings(std::filesystem::path, Types::Settings);
	Types::Error writeOptions(std::filesystem::path,Types::Options);
	Types::Error writeFile(std::filesystem::path, Types::FileIndex, Types::File);
} // namespace System
} // namespace WhipseeySaveManager
