#include "system.hpp"

#include <catch.hpp>
#include <sstream>
#include <ios>

using namespace WhipseeySaveManager;
//TODO use new error stuff
TEST_CASE("System::systemTheme", "[.][System][Manual]")
{
	Types::ErrDat<Types::Theme> ret = System::systemTheme();
	if(ret)
	{
		SUCCEED("theme retrieved");
	}
	else
	{
		CHECK((ret == Types::Error::Code::ColorDwordNotFound
			|| ret == Types::Error::Code::ColorKeyNotFound
			|| ret == Types::Error::Code::ThemeDwordNotFound
			|| ret == Types::Error::Code::ThemeKeyNotFound));
		FAIL("theme not retrieved");
	}
	std::stringstream msg;
	msg << "Theme status must be check manually!"
		<< "\nDarkmode: " << (ret.data.darkmode == Types::Toggle::Enabled ? "Enabled" : "Disabled")
		<< "\nAccent  : 0x" << std::hex << ret.data.accent;
	WARN(msg.str());
}

TEST_CASE("System::defaultSavePath", "[.][System][Save]")
{
	Types::ErrDat<std::filesystem::path> ret = System::defaultSavePath();
	if(ret)
	{
		REQUIRE(std::filesystem::exists(ret.data));
		CHECK(ret.data.filename() == "whipseey.sav");
		SUCCEED("path retrieved");
	}
	else
	{
		CHECK(ret == Types::Error::Code::DefaultSaveNotFound);
		FAIL("path not retrieved");
	}
}

TEST_CASE("System::defaultSettingsPath", "[.][System][Game]")
{
	Types::ErrDat<std::filesystem::path> ret = System::defaultSettingsPath();
	if(ret)
	{
		REQUIRE(std::filesystem::exists(ret.data));
		CHECK(ret.data.filename() == "bfs_settings.ini");
		SUCCEED("path retrieved");
	}
	else
	{
		CHECK((ret == Types::Error::Code::GameNotFound
			|| ret == Types::Error::Code::SteamDwordNotFound
			|| ret == Types::Error::Code::SteamKeyNotFound
			|| ret == Types::Error::Code::SteamLibrariesNotFound));
		FAIL("path not retrieved");
	}
}

TEST_CASE("System::readSettings", "[System]")
{
	const std::filesystem::path default(L"data/settings-default.ini");
	const std::filesystem::path invalid(L"data/settings-invalid.ini");
	const std::filesystem::path missing(L"data/settings-missing.ini");
	const std::filesystem::path   valid(L"data/settings-valid.ini");

	if(std::filesystem::exists(default) == false) FAIL("default test file missing!");
	if(std::filesystem::exists(invalid) == false) FAIL("invalid test file missing!");
	if(std::filesystem::exists(missing) == false) FAIL("missing test file missing!");
	if(std::filesystem::exists(  valid) == false) FAIL("  valid test file missing!");

	SECTION("default")
	{
		Types::ErrDat<Types::Settings> ret = System::readSettings(default);
		CHECK(ret);
		CHECK(ret.data.cheats == Types::Toggle::Disabled);
	}

	SECTION("valid")
	{
		Types::ErrDat<Types::Settings> ret = System::readSettings(valid);
		CHECK(ret);
		CHECK(ret.data.cheats == Types::Toggle::Enabled);
	}

	SECTION("invalid")
	{
		Types::ErrDat<Types::Settings> ret = System::readSettings(invalid);
		CHECK_FALSE(ret);
		CHECK(ret == Types::Error::Code::CheatsKeyInvalid);
		CHECK(ret.data.cheats == Types::Settings().cheats);
	}

	SECTION("missing")
	{
		Types::ErrDat<Types::Settings> ret = System::readSettings(missing);
		CHECK_FALSE(ret);
		CHECK((ret == Types::Error::Code::CheatsSectionNotFound
			|| ret == Types::Error::Code::CheatsKeyNotFound));
		CHECK(ret.data.cheats == Types::Settings().cheats);
	}
}
