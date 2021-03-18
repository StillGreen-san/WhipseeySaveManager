#include "testhelper.hpp"

using namespace WhipseeySaveManager;

TEST_CASE("Error")
{
	SECTION("bool")
	{
		CHECK_FALSE(Types::Error(Types::Error::Code::Nothing));
		CHECK(Types::Error(Types::Error::Code::Unknown));
	}

	SECTION("== Code")
	{
		CHECK(Types::Error(Types::Error::Code::Nothing) == Types::Error::Code::Nothing);
		CHECK(Types::Error(Types::Error::Code::Unknown) == Types::Error::Code::Unknown);
	}

	SECTION("== Error")
	{
		CHECK(Types::Error(Types::Error::Code::Nothing) == Types::Error(Types::Error::Code::Nothing));
		CHECK(Types::Error(Types::Error::Code::Unknown) == Types::Error(Types::Error::Code::Unknown));

		Types::Error errorA(Types::Error::Code::Unknown);
		errorA += Types::Error::Code::Unknown;
		Types::Error errorB(Types::Error::Code::Unknown);
		errorB += Types::Error::Code::Unknown;

		CHECK(errorA == errorB);

		errorB += Types::Error::Code::Unknown;
		CHECK_FALSE(errorA == errorB);
	}

	SECTION("=")
	{
		Types::Error error;

		error = Types::Error::Code::Unknown;
		CHECK(error);

		error = Types::Error::Code::Nothing;
		CHECK_FALSE(error);

		error += Types::Error::Code::Unknown;
		error = Types::Error::Code::Nothing;
		CHECK_FALSE(error);
	}

	SECTION("+= Code")
	{
		Types::Error error;

		error += Types::Error::Code::Nothing;
		CHECK_FALSE(error);

		error += Types::Error::Code::Nothing;
		CHECK_FALSE(error);

		error += Types::Error::Code::Unknown;
		CHECK(error);

		error += Types::Error::Code::Unknown;
		Types::Error expected(Types::Error::Code::Unknown);
		expected += Types::Error::Code::Unknown;
		CHECK(error == expected);
	}

	SECTION("+= Error")
	{
		Types::Error error(Types::Error::Code::Nothing);
		Types::Error unknowns(Types::Error::Code::Unknown);
		unknowns += Types::Error::Code::Unknown;

		error += unknowns;
		CHECK(error == unknowns);

		unknowns += unknowns;
		CHECK_FALSE(error == unknowns);
	}
}