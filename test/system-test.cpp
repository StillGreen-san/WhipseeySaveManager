#include "system.hpp"
#include "testhelper.hpp"

#include <ios>
#include <sstream>

using namespace WhipseeySaveManager;

TEST_CASE("System::systemTheme", "[.][System][Manual]")
{
	std::optional<Types::Theme> ret = System::systemTheme();
	if(ret)
	{
		SUCCEED("theme retrieved");
	}
	else
	{
		FAIL("theme not retrieved");
	}
	std::stringstream msg;
	msg << "Theme status must be check manually!"
	    << "\nDarkmode: " << (ret->darkmode == Types::Darkmode::Enabled ? "Enabled" : "Disabled") << "\nAccent  : 0x"
	    << std::hex << ret->accent;
	WARN(msg.str());
}

TEST_CASE("System::defaultSavePath", "[.][System][Save]")
{
	std::optional<std::filesystem::path> ret = System::defaultSavePath();
	if(ret)
	{
		REQUIRE(std::filesystem::exists(*ret));
		CHECK(ret->filename() == "whipseey.sav");
		SUCCEED("path retrieved");
	}
	else
	{
		FAIL("path not retrieved");
	}
}

TEST_CASE("System::defaultSettingsPath", "[.][System][Game]")
{
	std::optional<std::filesystem::path> ret = System::defaultSettingsPath();
	if(ret)
	{
		REQUIRE(std::filesystem::exists(*ret));
		CHECK(ret->filename() == "bfs_settings.ini");
		SUCCEED("path retrieved");
	}
	else
	{
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

TEST_CASE("System::write ISection", "[System]")
{
	SECTION("write new")
	{
		REQUIRE_MISSING(Test::Data::write);
		auto cheatsWrite = std::make_shared<INI::Cheats>();
		Types::Error error = System::write(cheatsWrite, Test::Data::write);
		CHECK_FALSE(error);

		auto cheatsDefault = std::make_shared<INI::Cheats>();
		System::read(cheatsDefault, Test::Data::write);
		CHECK(cheatsDefault->getCheatsEnabled() == cheatsWrite->getCheatsEnabled());

		REMOVE(Test::Data::write);
	}

	SECTION("write existing")
	{
		REQUIRE_MISSING(Test::Data::write);
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		REQUIRE(std::filesystem::copy_file(Test::Data::settingsDefault, Test::Data::write));

		auto optionsWrite = std::make_shared<INI::Options>();
		Types::Error error = System::write(optionsWrite, Test::Data::write);
		CHECK_FALSE(error);

		auto cheatsDefault = std::make_shared<INI::Cheats>();
		error = System::read(cheatsDefault, Test::Data::write);
		CHECK_FALSE(error);
		CHECK(cheatsDefault->getCheatsEnabled() == INI::Cheats().getCheatsEnabled());

		auto optionsDefault = std::make_shared<INI::Options>();
		error = System::read(optionsDefault, Test::Data::write);
		CHECK_FALSE(error);
		CHECK(optionsDefault->getLanguage() == optionsWrite->getLanguage());

		REMOVE(Test::Data::write);
	}
}

TEST_CASE("System::write IIni", "[System]")
{
	REQUIRE_MISSING(Test::Data::write);
	auto settingsWrite = std::make_shared<INI::Settings>();
	settingsWrite->getCheats().getCheatsEnabled() = Types::CheatsEnabled::Enabled;
	Types::Error error = System::write(settingsWrite, Test::Data::write);
	CHECK_FALSE(error);

	auto settingsCheck = std::make_shared<INI::Settings>();
	error = System::read(settingsCheck, Test::Data::write);
	CHECK_FALSE(error);
	CHECK(settingsCheck->getCheats().getCheatsEnabled() == settingsWrite->getCheats().getCheatsEnabled());

	REMOVE(Test::Data::write);
}
