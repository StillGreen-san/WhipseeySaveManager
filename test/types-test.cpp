#include "testhelper.hpp"

using namespace WhipseeySaveManager;

TEST_CASE("Error", "[Types]")
{
	SECTION("Error()")
	{
		Types::Error error;
		REQUIRE_FALSE(error);
	}

	SECTION("Error(Code)")
	{
		CHECK_FALSE(Types::Error(Types::Error::Code::Nothing));

		CHECK(Types::Error(Types::Error::Code::KeyNotFound));
		CHECK(Types::Error(Types::Error::Code::KeyNotFound) == Types::Error::Code::KeyNotFound);
	}

	SECTION("Error(std::initializer_list<Code>)")
	{
		{
			Types::Error error({Types::Error::Code::Nothing, Types::Error::Code::Nothing});
			CHECK_FALSE(error);
		}
		{
			Types::Error error({Types::Error::Code::Nothing, Types::Error::Code::InvalidFormat});
			CHECK(error);
			CHECK(error == Types::Error::Code::InvalidFormat);
		}
		{
			Types::Error error({Types::Error::Code::FailedToLoadFile, Types::Error::Code::Nothing});
			CHECK(error);
			CHECK(error == Types::Error::Code::FailedToLoadFile);
		}
		{
			Types::Error error(
			    {Types::Error::Code::FailedToLoadFile, Types::Error::Code::Nothing, Types::Error::Code::Unknown});
			CHECK(error);
			CHECK(error == Types::Error({Types::Error::Code::FailedToLoadFile, Types::Error::Code::Unknown}));
		}
	}

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
		Types::Error unknowns({Types::Error::Code::Unknown, Types::Error::Code::Unknown});

		error += unknowns;
		CHECK(error == unknowns);

		unknowns += unknowns;
		CHECK_FALSE(error == unknowns);
		CHECK(
		    unknowns == Types::Error(
		                    {Types::Error::Code::Unknown, Types::Error::Code::Unknown, Types::Error::Code::Unknown,
		                     Types::Error::Code::Unknown}));
	}
}
