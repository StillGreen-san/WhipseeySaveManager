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
	 * @return Types::ErrDat<Types::Theme>: Theme always valid, 
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
	 * @brief finds the default bfs_settings.ini path
	 * 
	 * @return Types::ErrDat<std::filesystem::path> if error path is empty and code = 
	 * SteamKeyNotFound, SteamDwordNotFound, SteamLibrariesNotFound or GameNotFound
	 */
	Types::ErrDat<std::filesystem::path> defaultSettingsPath();

	Types::ErrDat<Types::Save> readSave(std::filesystem::path);

	/**
	 * @brief reads a bfs_settings file
	 * 
	 * @param settings path to a bfs_settings file
	 * @return Types::ErrDat<Types::Settings> Settings always valid,
	 * on error: code = FailedToLoadSettings, CheatsSectionNotFound, CheatsKeyNotFound, CheatsKeyInvalid
	 */
	Types::ErrDat<Types::Settings> readSettings(const std::filesystem::path& settings);

	/**
	 * @brief reads the options section of a whipseey save file
	 * 
	 * @param options path to whipseey save file
	 * @return Types::ErrDat<Types::Options> Options always valid,
	 * on error: code = FailedToLoadOptions, OptionsSectionNotFound, ScaleKeyNotFound,
	 * LanguageKeyNotFound, FullscreenKeyNotFound, LefthandedKeyNotFound, SoundvolumeKeyNotFound,
	 * SoundtoggleKeyNotFound, MusicvolumeKeyNotFound, MusictoggleKeyNotFound
	 */
	Types::ErrDat<Types::Options> readOptions(const std::filesystem::path& save);

	/**
	 * @brief reads the options section of a whipseey save file
	 * 
	 * @param options path to whipseey save file
	 * @return Types::ErrDat<Types::File> File always valid,
	 * on error: code = FailedToLoadFile, FileSectionNotFound, NodamageKeyNotFound, DefeatedKeyNotFound,
	 * CastleKeyNotFound, MoonKeyNotFound, SnowKeyNotFound, DesertKeyNotFound, ForestKeyNotFound,
	 * EndingKeyNotFound, IntroKeyNotFound, LivesKeyNotFound, GemsKeyNotFound
	 */
	Types::ErrDat<Types::File> readFile(const std::filesystem::path& save, Types::FileIndex index);

	Types::Error writeSave(std::filesystem::path, Types::Save);
	Types::Error writeSettings(std::filesystem::path, Types::Settings);
	Types::Error writeOptions(std::filesystem::path,Types::Options);
	Types::Error writeFile(std::filesystem::path, Types::FileIndex, Types::File);
} // namespace System
	void func();
} // namespace WhipseeySaveManager
