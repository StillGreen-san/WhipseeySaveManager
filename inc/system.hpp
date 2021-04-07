#pragma once

#include "ini.hpp"

namespace WhipseeySaveManager::System
{
/**
 * @brief trys to query the os for theme info
 *
 * @return std::optional<Types::Theme>: if successful contains Theme object
 */
std::optional<Types::Theme> systemTheme();

/**
 * @brief trys to find the default whipseey.sav path
 *
 * @return std::optional<std::filesystem::path>: if successful contains path
 */
std::optional<std::filesystem::path> defaultSavePath();

/**
 * @brief trys to find the default bfs_settings.ini path
 *
 * @return std::optional<std::filesystem::path>: if successful contains path
 */
std::optional<std::filesystem::path> defaultSettingsPath();

/**
 * @brief reads all keys defined in ISection from file into ini
 *
 * @param section INI::ISection object to read into
 * @param file path to file to read from
 * @return Types::Error if failed one or more FailedToLoadFile, KeyNotFound, SectionNotFound
 */
Types::Error read(std::shared_ptr<INI::ISection> section, std::filesystem::path file);

/**
 * @brief reads all keys defined in IIni from file into ini
 *
 * @param ini INI::IIni object to read into
 * @param file path to file to read from
 * @return Types::Error if failed one or more FailedToLoadFile, KeyNotFound, SectionNotFound
 */
Types::Error read(std::shared_ptr<INI::IIni> ini, std::filesystem::path file);

/**
 * @brief writes all keys defined in ISection to file, trys to load file first to keep other sections
 *
 * @param section INI::ISection object to write from
 * @param file path to file to write to
 * @return Types::Error if failed FailedToLoadFile, FailedToWriteFile
 */
Types::Error write(std::shared_ptr<INI::ISection> section, std::filesystem::path file);

/**
 * @brief writes all keys defined in IIni to file
 *
 * @param ini INI::IIni object to write from
 * @param file path to file to write to
 * @return Types::Error if failed FailedToWriteFile
 */
Types::Error write(std::shared_ptr<INI::IIni> ini, std::filesystem::path file);
} // namespace WhipseeySaveManager::System
