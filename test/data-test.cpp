#include <catch.hpp>

#include "data.hpp"

using namespace WhipseeySaveManager;

template<typename ENUM>
constexpr ENUM maxEnumVal() noexcept
{
	return static_cast<ENUM>(std::numeric_limits<ENUM>::max());
}

template<typename TYPE>
constexpr TYPE maxTypeVal(const TYPE&)
{
	return std::numeric_limits<TYPE>::max();
}

Data::Options makeInvalidOptions()
{
	Data::Options options;
	options.language = maxEnumVal<Data::Language>();
	options.scale = maxEnumVal<Data::Scale>();
	options.fullScreen = maxEnumVal<Data::Toggle>();
	options.leftHanded = maxEnumVal<Data::Toggle>();
	options.sound.toggle = maxEnumVal<Data::Toggle>();
	options.sound.volume = maxEnumVal<Data::Volume>();
	options.music.toggle = maxEnumVal<Data::Toggle>();
	options.music.volume = maxEnumVal<Data::Volume>();
	return options;
}

Data::File makeInvalidFile()
{
	Data::File file;
	file.noDamage = maxEnumVal<Data::BossNoDamage>();
	file.defeated = maxTypeVal(file.defeated);
	file.progress = maxEnumVal<Data::Level>();
	file.ending = maxEnumVal<Data::Toggle>();
	file.intro = maxEnumVal<Data::Toggle>();
	file.lives = maxTypeVal(file.lives);
	file.gems = maxTypeVal(file.gems);
	return file;
}

Data::Save makeInvalidSave()
{
	Data::Save save;
	save.options = makeInvalidOptions();
	save.file1 = makeInvalidFile();
	save.file2 = makeInvalidFile();
	save.file3 = makeInvalidFile();
	return save;
}

Data::Options makeValidOptions()
{
	Data::Options options;
	options.language = Data::Language::Japanese;
	options.scale = Data::Scale::R1152x648;
	options.fullScreen = Data::Toggle::Disabled;
	options.leftHanded = Data::Toggle::Enabled;
	options.sound.toggle = Data::Toggle::Disabled;
	options.sound.volume = Data::Volume::V30;
	options.music.toggle = Data::Toggle::Disabled;
	options.music.volume = Data::Volume::V70;
	return options;
}

Data::File makeValidFile()
{
	Data::File file;
	file.noDamage = Data::BossNoDamage::All;
	file.defeated = maxTypeVal(file.defeated);
	file.progress = Data::Level::Castle;
	file.ending = Data::Toggle::Enabled;
	file.intro = Data::Toggle::Enabled;
	file.lives = 9999;
	file.gems = 99;//TODO use named constants
	return file;
}

Data::Save makeValidSave()
{
	Data::Save save;
	save.options = makeValidOptions();
	save.file1 = makeValidFile();
	save.file2 = makeValidFile();
	save.file3 = makeValidFile();
	return save;
}

TEST_CASE("Data::readSave", "[IO]")
{
	FAIL("TEST NOT IMPLEMENTED");
}

TEST_CASE("Data::writeSave", "[IO]")
{
	SECTION("path")
	{
		FAIL("TEST NOT IMPLEMENTED");
	}

	SECTION("void")
	{
		FAIL("TEST NOT IMPLEMENTED");
	}
}

TEST_CASE("Data::readSettings", "[IO]")
{
	FAIL("TEST NOT IMPLEMENTED");
}

TEST_CASE("Data::writeSettings", "[IO]")
{
	SECTION("path")
	{
		FAIL("TEST NOT IMPLEMENTED");
	}

	SECTION("void")
	{
		FAIL("TEST NOT IMPLEMENTED");
	}
}

TEST_CASE("Data::setSave")
{
	Data::Data data;
	Data::Save save;

	SECTION("default save")
	{
		Error::Error error = data.setSave(save);
		REQUIRE_FALSE(error);
	}

	SECTION("valid save")
	{
		save = makeValidSave();
		Error::Error error = data.setSave(save);
		REQUIRE_FALSE(error);
	}

	SECTION("invalid save")
	{
		save = makeInvalidSave();
		Error::Error expected = Error::makeError(Error::Where::Save, Error::What::Value);
		Error::Error error = data.setSave(save);
		REQUIRE(error == expected);
	}

	SECTION("invalid options")
	{
		save.options = makeInvalidOptions();
		Error::Error expected = Error::makeError(Error::Where::Options, Error::What::Value);
		Error::Error error = data.setSave(save);
		REQUIRE(error == expected);
	}

	SECTION("invalid file")
	{
		save.file1 = makeInvalidFile();
		Error::Error expected = Error::makeError(Error::Where::File, Error::What::Value);
		Error::Error error = data.setSave(save);
		REQUIRE(error == expected);
	}
}

TEST_CASE("Data::setSettings")
{
	FAIL("TEST NOT IMPLEMENTED");
}

TEST_CASE("Data::setFile")
{
	FAIL("TEST NOT IMPLEMENTED");
}
