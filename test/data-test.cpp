#include <catch.hpp>

#include "data.hpp"

using namespace WhipseeySaveManager;

Data::Options makeInvalidOptions()
{
	return {};
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
	FAIL("TEST NOT IMPLEMENTED");
	Data::Data data;

	SECTION("default save")
	{
		Data::Save save;
		Error::Error error = data.setSave(save);
		REQUIRE_FALSE(error);
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
