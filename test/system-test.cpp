#include "system.hpp"
#include "testhelper.hpp"

#include <sstream>
#include <ios>

using namespace WhipseeySaveManager;

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

TEST_CASE("System::read ISection", "[System]")
{
	auto settings = std::make_shared<INI::Cheats>();

	SECTION("default")
	{
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		Types::Error error = System::read(settings, Test::Data::settingsDefault);
		CHECK_FALSE(error);
		CHECK(settings->getCheatsEnabled() == Types::CheatsEnabled::Disabled);
	}

	SECTION("valid")
	{
		REQUIRE_EXISTS(Test::Data::settingsValid);
		Types::Error error = System::read(settings, Test::Data::settingsValid);
		CHECK_FALSE(error);
		CHECK(settings->getCheatsEnabled() == Types::CheatsEnabled::Enabled);
	}

	SECTION("invalid")
	{
		REQUIRE_EXISTS(Test::Data::settingsInvalid);
		Types::Error error = System::read(settings, Test::Data::settingsInvalid);
		CHECK(error);
		CHECK(error == Types::Error::Code::InvalidValue);
		CHECK(settings->getCheatsEnabled() == INI::CheatsEnabled());
	}

	SECTION("missingSection")
	{
		REQUIRE_EXISTS(Test::Data::settingsMissingSection);
		Types::Error error = System::read(settings, Test::Data::settingsMissingSection);
		CHECK(error);
		CHECK(error == Types::Error::Code::SectionNotFound);
		CHECK(settings->getCheatsEnabled() == INI::CheatsEnabled());
	}

	SECTION("missingKey")
	{
		REQUIRE_EXISTS(Test::Data::settingsMissingKey);
		Types::Error error = System::read(settings, Test::Data::settingsMissingKey);
		CHECK(error);
		CHECK(error == Types::Error::Code::KeyNotFound);
		CHECK(settings->getCheatsEnabled() == INI::CheatsEnabled());
	}

	SECTION("missing")
	{
		REQUIRE_MISSING(Test::Data::missing);
		Types::Error error = System::read(settings, Test::Data::missing);
		CHECK(error);
		CHECK(error == Types::Error::Code::FailedToLoadFile);
		CHECK(settings->getCheatsEnabled() == INI::CheatsEnabled());
	}
}

TEST_CASE("System::read IIni", "[System]")
{
	auto settings = std::make_shared<INI::Settings>();

	SECTION("default")
	{
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		Types::Error error = System::read(settings, Test::Data::settingsDefault);
		CHECK_FALSE(error);
		CHECK(settings->getCheats().getCheatsEnabled() == Types::CheatsEnabled::Disabled);
	}

	SECTION("valid")
	{
		REQUIRE_EXISTS(Test::Data::settingsValid);
		Types::Error error = System::read(settings, Test::Data::settingsValid);
		CHECK_FALSE(error);
		CHECK(settings->getCheats().getCheatsEnabled() == Types::CheatsEnabled::Enabled);
	}

	SECTION("invalid")
	{
		REQUIRE_EXISTS(Test::Data::settingsInvalid);
		Types::Error error = System::read(settings, Test::Data::settingsInvalid);
		CHECK(error);
		CHECK(error == Types::Error::Code::InvalidValue);
		CHECK(settings->getCheats().getCheatsEnabled() == INI::CheatsEnabled());
	}

	SECTION("missingSection")
	{
		REQUIRE_EXISTS(Test::Data::settingsMissingSection);
		Types::Error error = System::read(settings, Test::Data::settingsMissingSection);
		CHECK(error);
		CHECK(error == Types::Error::Code::SectionNotFound);
		CHECK(settings->getCheats().getCheatsEnabled() == INI::CheatsEnabled());
	}

	SECTION("missingKey")
	{
		REQUIRE_EXISTS(Test::Data::settingsMissingKey);
		Types::Error error = System::read(settings, Test::Data::settingsMissingKey);
		CHECK(error);
		CHECK(error == Types::Error::Code::KeyNotFound);
		CHECK(settings->getCheats().getCheatsEnabled() == INI::CheatsEnabled());
	}

	SECTION("missing")
	{
		REQUIRE_MISSING(Test::Data::missing);
		Types::Error error = System::read(settings, Test::Data::missing);
		CHECK(error);
		CHECK(error == Types::Error::Code::FailedToLoadFile);
		CHECK(settings->getCheats().getCheatsEnabled() == INI::CheatsEnabled());
	}
}
