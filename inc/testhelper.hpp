#pragma once

#include "types.hpp"
#include <catch2/catch_test_macros.hpp>

namespace WhipseeySaveManager::Test
{
std::string stringify(float value);

namespace Data
{
const std::filesystem::path missing(L"data/__MISSING_FILE__.NONE");
const std::filesystem::path write(L"data/__WRITE_FILE__.TMP");
const std::filesystem::path settingsDefault(L"data/settings-default.ini");
const std::filesystem::path settingsValid(L"data/settings-valid.ini");
const std::filesystem::path settingsInvalid(L"data/settings-invalid.ini");
const std::filesystem::path settingsMissingSection(L"data/settings-missing-section.ini");
const std::filesystem::path settingsMissingKey(L"data/settings-missing-key.ini");
const std::filesystem::path optionsDefault(L"data/options-default.ini");
const std::filesystem::path optionsValid(L"data/options-valid.ini");
const std::filesystem::path optionsInvalid(L"data/options-invalid.ini");
const std::filesystem::path optionsMissingSection(L"data/options-missing-section.ini");
const std::filesystem::path optionsMissingKey(L"data/options-missing-key.ini");
const std::filesystem::path fileMissingBossnodamage(L"data/file-missing-bossnodamage.ini");
} // namespace Data

#define REQUIRE_EXISTS(path) \
	if(!std::filesystem::exists(path)) \
	FAIL(path.relative_path().string().append(" missing!"))
#define REQUIRE_MISSING(path) \
	if(std::filesystem::exists(path)) \
	FAIL(path.relative_path().string().append(" exists!"))
#define REMOVE(path) \
	if(std::filesystem::exists(path)) \
		if(!std::filesystem::remove(path)) \
	FAIL(path.relative_path().string().append(" could not be deleted!"))
} // namespace WhipseeySaveManager::Test
