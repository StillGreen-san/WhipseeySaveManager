#include <catch.hpp>

#include "testhelper.hpp"

using namespace WhipseeySaveManager;

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
	Data::Data data;
	const std::filesystem::path default("data/settings-default.ini");
	const std::filesystem::path valid("data/settings-valid.ini");
	const std::filesystem::path invalid("data/settings-invalid.ini");
	const std::filesystem::path missing("data/settings-missing.ini");
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
		REQUIRE(save == data.getSave());
	}

	SECTION("valid save")
	{
		save = Test::makeValidSave();
		Error::Error error = data.setSave(save);
		REQUIRE_FALSE(error);
		REQUIRE(save == data.getSave());
	}

	SECTION("invalid save")
	{
		save = Test::makeInvalidSave();
		Error::Error expected = Error::makeError(Error::Where::Save, Error::What::Value);
		Error::Error error = data.setSave(save);
		REQUIRE(error == expected);
		REQUIRE(Data::Save() == data.getSave());
	}

	SECTION("invalid options")
	{
		save.options = Test::makeInvalidOptions();
		Error::Error expected = Error::makeError(Error::Where::Options, Error::What::Value);
		Error::Error error = data.setSave(save);
		REQUIRE(error == expected);
		REQUIRE(Data::Save() == data.getSave());
	}

	SECTION("invalid file")
	{
		save.file1 = Test::makeInvalidFile();
		Error::Error expected = Error::makeError(Error::Where::File, Error::What::Value);
		Error::Error error = data.setSave(save);
		REQUIRE(error == expected);
		REQUIRE(Data::Save() == data.getSave());
	}
}

TEST_CASE("Data::setSettings")
{
	Data::Data data;
	Data::Settings settings;

	SECTION("default settings")
	{
		Error::Error error = data.setSettings(settings);
		REQUIRE_FALSE(error);
		REQUIRE(settings == data.getSettings());
	}

	SECTION("valid settings")
	{
		settings = Test::makeValidSettings();
		Error::Error error = data.setSettings(settings);
		REQUIRE_FALSE(error);
		REQUIRE(settings == data.getSettings());
	}

	SECTION("invalid settings")
	{
		settings = Test::makeInvalidSettings();
		Error::Error expected = Error::makeError(Error::Where::Settings, Error::What::Value);
		Error::Error error = data.setSettings(settings);
		REQUIRE(error == expected);
		REQUIRE(Data::Settings() == data.getSettings());
	}
}

TEST_CASE("Data::setFile")
{
	Data::Data data;
	Data::File file;
	Data::FileIndex index;

	SECTION("default file")
	{
		Error::Error error = data.setFile(index, file);
		REQUIRE_FALSE(error);
		REQUIRE(file == data.getFile(index));
	}

	SECTION("valid file")
	{
		file = Test::makeValidFile();
		Error::Error error = data.setFile(index, file);
		REQUIRE_FALSE(error);
		REQUIRE(file == data.getFile(index));
	}

	SECTION("invalid file")
	{
		file = Test::makeInvalidFile();
		Error::Error expected = Error::makeError(Error::Where::File, Error::What::Value);
		Error::Error error = data.setFile(index, file);
		REQUIRE(error == expected);
		REQUIRE(Data::File() == data.getFile(index));
	}

	SECTION("invalid file fields")
	{
		Data::File invalid = Test::makeInvalidFile();
		Error::Error expected = Error::makeError(Error::Where::File, Error::What::Value);

		std::swap(file.noDamage, invalid.noDamage);
		Error::Error error = data.setFile(index, file);
		REQUIRE(error == expected);
		std::swap(file.noDamage, invalid.noDamage);
		REQUIRE(file == data.getFile(index));

		std::swap(file.progress, invalid.progress);
		error = data.setFile(index, file);
		REQUIRE(error == expected);
		std::swap(file.progress, invalid.progress);
		REQUIRE(file == data.getFile(index));

		std::swap(file.ending, invalid.ending);
		error = data.setFile(index, file);
		REQUIRE(error == expected);
		std::swap(file.ending, invalid.ending);
		REQUIRE(file == data.getFile(index));

		std::swap(file.intro, invalid.intro);
		error = data.setFile(index, file);
		REQUIRE(error == expected);
		std::swap(file.intro, invalid.intro);
		REQUIRE(file == data.getFile(index));

		std::swap(file.lives, invalid.lives);
		error = data.setFile(index, file);
		REQUIRE(error == expected);
		std::swap(file.lives, invalid.lives);
		REQUIRE(file == data.getFile(index));

		std::swap(file.gems, invalid.gems);
		error = data.setFile(index, file);
		REQUIRE(error == expected);
		std::swap(file.gems, invalid.gems);
		REQUIRE(file == data.getFile(index));
	}
}
