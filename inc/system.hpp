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
	Types::ErrDat<Types::Theme> systemTheme();
	Types::ErrDat<std::filesystem::path> defaultSavePath();
	Types::ErrDat<std::filesystem::path> defaultSettingsPath();
	Types::ErrDat<std::filesystem::path> selectSavePath();
	Types::ErrDat<std::filesystem::path> selectSettingsPath();
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
