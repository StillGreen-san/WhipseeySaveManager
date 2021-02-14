#pragma once

#include "types.hpp"

namespace WhipseeySaveManager
{
namespace System
{
	Types::Return<std::filesystem::path> defaultSavePath();
	Types::Return<std::filesystem::path> defaultSettingsPath();
	Types::Return<std::filesystem::path> selectSavePath();
	Types::Return<std::filesystem::path> selectSettingsPath();
	Types::Return<Types::Save> readSave(std::filesystem::path);
	Types::Return<Types::Settings> readSettings(std::filesystem::path);
	Types::Return<Types::Options> readOptions(std::filesystem::path);
	Types::Return<Types::File> readFile(std::filesystem::path, Types::FileIndex);
	Types::Return<void> writeSave(std::filesystem::path, Types::Save);
	Types::Return<void> writeSettings(std::filesystem::path, Types::Settings);
	Types::Return<void> writeOptions(std::filesystem::path,Types::Options);
	Types::Return<void> writeFile(std::filesystem::path, Types::FileIndex, Types::File);
} // namespace System
} // namespace WhipseeySaveManager
