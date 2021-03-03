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

TEST_CASE("System::readSettings", "[System]")
{
	SECTION("default")
	{
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		Types::ErrDat<Types::Settings> ret = System::readSettings(Test::Data::settingsDefault);
		CHECK(ret);
		CHECK(ret.data.cheats == Types::Toggle::Disabled);
	}

	SECTION("valid")
	{
		REQUIRE_EXISTS(Test::Data::settingsValid);
		Types::ErrDat<Types::Settings> ret = System::readSettings(Test::Data::settingsValid);
		CHECK(ret);
		CHECK(ret.data.cheats == Types::Toggle::Enabled);
	}

	SECTION("invalid")
	{
		REQUIRE_EXISTS(Test::Data::settingsInvalid);
		Types::ErrDat<Types::Settings> ret = System::readSettings(Test::Data::settingsInvalid);
		CHECK_FALSE(ret);
		CHECK(ret == Types::Error::Code::CheatsKeyInvalid);
		CHECK(ret.data.cheats == Types::Settings().cheats);
	}

	SECTION("missingSection")
	{
		REQUIRE_EXISTS(Test::Data::settingsMissingSection);
		Types::ErrDat<Types::Settings> ret = System::readSettings(Test::Data::settingsMissingSection);
		CHECK_FALSE(ret);
		CHECK((ret == Types::Error::Code::CheatsSectionNotFound
			|| ret == Types::Error::Code::CheatsKeyNotFound));
		CHECK(ret.data.cheats == Types::Settings().cheats);
	}

	SECTION("missingKey")
	{
		REQUIRE_EXISTS(Test::Data::settingsMissingKey);
		Types::ErrDat<Types::Settings> ret = System::readSettings(Test::Data::settingsMissingKey);
		CHECK_FALSE(ret);
		CHECK((ret == Types::Error::Code::CheatsSectionNotFound
			|| ret == Types::Error::Code::CheatsKeyNotFound));
		CHECK(ret.data.cheats == Types::Settings().cheats);
	}

	SECTION("missing")
	{
		REQUIRE_EXISTS(Test::Data::missing);
		Types::ErrDat<Types::Settings> ret = System::readSettings(Test::Data::missing);
		CHECK_FALSE(ret);
		CHECK(ret == Types::Error::Code::FailedToLoadSettings);
		CHECK(ret.data.cheats == Types::Settings().cheats);
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
		CHECK_THAT(settings->getCheatsEnabled(), Test::EQUALS(Types::CheatsEnabled::Disabled));
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
		REQUIRE_EXISTS(Test::Data::missing);
		Types::Error error = System::read(settings, Test::Data::missing);
		CHECK(error);
		CHECK(error == Types::Error::Code::KeyNotFound);
		CHECK(settings->getCheatsEnabled() == INI::CheatsEnabled());
	}
}

TEST_CASE("System::readOptions", "[System]")
{
	SECTION("default")
	{
		REQUIRE_EXISTS(Test::Data::optionsDefault);
		const Types::ErrDat<Types::Options> ret = System::readOptions(Test::Data::optionsDefault);
		CHECK(ret);
		CHECK(ret.data.language == Types::Language::English);
		CHECK(ret.data.scale == Types::Scale::R768x432);
		CHECK(ret.data.fullScreen == Types::Toggle::Enabled);
		CHECK(ret.data.leftHanded == Types::Toggle::Disabled);
		CHECK(ret.data.sound.volume == Types::Volume::V100);
		CHECK(ret.data.sound.toggle == Types::Toggle::Enabled);
		CHECK(ret.data.music.volume == Types::Volume::V100);
		CHECK(ret.data.music.toggle == Types::Toggle::Enabled);
	}

	SECTION("valid")
	{
		REQUIRE_EXISTS(Test::Data::optionsValid);
		const Types::ErrDat<Types::Options> ret = System::readOptions(Test::Data::optionsValid);
		CHECK(ret);
		CHECK(ret.data.language == Types::Language::Japanese);
		CHECK(ret.data.scale == Types::Scale::R1536x864);
		CHECK(ret.data.fullScreen == Types::Toggle::Disabled);
		CHECK(ret.data.leftHanded == Types::Toggle::Enabled);
		CHECK(ret.data.sound.volume == Types::Volume::V30);
		CHECK(ret.data.sound.toggle == Types::Toggle::Enabled);
		CHECK(ret.data.music.volume == Types::Volume::V0);
		CHECK(ret.data.music.toggle == Types::Toggle::Disabled);
	}

	SECTION("invalid")
	{
		FAIL("validation not implemented");
	}

	SECTION("missingSection")
	{
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		REQUIRE_EXISTS(Test::Data::optionsMissingSection);
		const Types::ErrDat<Types::Options> ret = System::readOptions(Test::Data::optionsMissingSection);
		CHECK_FALSE(ret);
		CHECK(ret == Types::Error::Code::OptionsSectionNotFound);
		CHECK(ret.data.language == Types::Language::English);
		CHECK(ret.data.scale == Types::Scale::R768x432);
		CHECK(ret.data.fullScreen == Types::Toggle::Enabled);
		CHECK(ret.data.leftHanded == Types::Toggle::Disabled);
		CHECK(ret.data.sound.volume == Types::Volume::V100);
		CHECK(ret.data.sound.toggle == Types::Toggle::Enabled);
		CHECK(ret.data.music.volume == Types::Volume::V100);
		CHECK(ret.data.music.toggle == Types::Toggle::Enabled);
	}

	SECTION("missingKey")
	{
		REQUIRE_EXISTS(Test::Data::optionsMissingKey);
		const Types::ErrDat<Types::Options> ret = System::readOptions(Test::Data::optionsMissingKey);
		CHECK_FALSE(ret);
		CHECK(ret.data.language == Types::Language::English);
		CHECK(ret.data.scale == Types::Scale::R768x432);
		CHECK(ret.data.fullScreen == Types::Toggle::Enabled);
		CHECK(ret.data.leftHanded == Types::Toggle::Disabled);
		CHECK(ret.data.sound.volume == Types::Volume::V100);
		CHECK(ret.data.sound.toggle == Types::Toggle::Enabled);
		CHECK(ret.data.music.volume == Types::Volume::V100);
		CHECK(ret.data.music.toggle == Types::Toggle::Enabled);
	}

	SECTION("missing")
	{
		REQUIRE_EXISTS(Test::Data::missing);
		const Types::ErrDat<Types::Options> ret = System::readOptions(Test::Data::missing);
		CHECK_FALSE(ret);
		CHECK(ret == Types::Error::Code::FailedToLoadOptions);
		CHECK(ret.data.language == Types::Language::English);
		CHECK(ret.data.scale == Types::Scale::R768x432);
		CHECK(ret.data.fullScreen == Types::Toggle::Enabled);
		CHECK(ret.data.leftHanded == Types::Toggle::Disabled);
		CHECK(ret.data.sound.volume == Types::Volume::V100);
		CHECK(ret.data.sound.toggle == Types::Toggle::Enabled);
		CHECK(ret.data.music.volume == Types::Volume::V100);
		CHECK(ret.data.music.toggle == Types::Toggle::Enabled);
	}
}
