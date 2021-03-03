#include "ini.hpp"
#include "testhelper.hpp"

#include <catch.hpp>

using namespace WhipseeySaveManager;
using WhipseeySaveManager::INI::IKey;

struct TestKey : public IKey
{
	struct InternalData
	{
		float minOrA;
		float maxOrB;
		float default;
		float value;
	};
	InternalData getInternalData()
	{
		return {
			mMinOrA,
			mMaxOrB,
			mDefault,
			mValue
		};
	}
	using IKey::IKey;
};

struct MinMaxStringInt final : public TestKey
{
	MinMaxStringInt() : TestKey(
		name, default,
		Limits::MinMax,
		Number::StringInt,
		min, max
	) { }
	static constexpr std::string_view name = "keynameMinMaxStringInt";
	static constexpr float default = 42.f;
	static constexpr float min = 5.f;
	static constexpr float max = 99.f;
};
struct EitherOrStringFloat final : public TestKey
{
	EitherOrStringFloat() : TestKey(
		name, default,
		Limits::EitherOr,
		Number::StringFloat,
		either, or
	) { }
	static constexpr std::string_view name = "keynameEitherOrStringFloat";
	static constexpr float default = 5.5f;
	static constexpr float either = 5.5f;
	static constexpr float or = 99.9f;
};
struct EitherOrInt final : public TestKey
{
	EitherOrInt() : TestKey(
		name, default,
		Limits::EitherOr,
		Number::Int,
		either, or
	) { }
	static constexpr std::string_view name = "keynameEitherOrInt";
	static constexpr float default = 5.f;
	static constexpr float either = 5.f;
	static constexpr float or = 99.f;
};

TEST_CASE("INI::IKey", "[INI]")
{
	SECTION("Common")
	{
		using KeyType = MinMaxStringInt;
		KeyType key;
		auto const iData = key.getInternalData();
		CHECK(key.key() == KeyType::name);
		CHECK(iData.minOrA == KeyType::min);
		CHECK(iData.maxOrB == KeyType::max);
		CHECK(iData.default == KeyType::default);
		CHECK(iData.value == KeyType::default);
	}
	
	SECTION("MinMax")
	{
		using KeyType = MinMaxStringInt;
		KeyType key;

		auto valStr = Test::stringify(KeyType::max);
		Types::Error fromStringMax = key.fromString(valStr);
		CHECK(fromStringMax == Types::Error::Code::Nothing);

		valStr = Test::stringify(KeyType::min);
		Types::Error fromStringMin = key.fromString(valStr);
		CHECK(fromStringMin == Types::Error::Code::Nothing);

		valStr = Test::stringify(KeyType::min+1);
		Types::Error fromStringBetween = key.fromString(valStr);
		CHECK(fromStringBetween == Types::Error::Code::Nothing);

		valStr = Test::stringify(KeyType::max+1);
		Types::Error fromStringLarger = key.fromString(valStr);
		CHECK(fromStringLarger == Types::Error::Code::InvalidValue);

		valStr = Test::stringify(KeyType::min-1);
		Types::Error fromStringSmaller = key.fromString(valStr);
		CHECK(fromStringSmaller == Types::Error::Code::InvalidValue);
	}
	
	SECTION("EitherOr")
	{
		using KeyType = EitherOrStringFloat;
		KeyType key;

		auto valStr = Test::stringify(KeyType::or);
		Types::Error fromStringOr = key.fromString(valStr);
		CHECK(fromStringOr == Types::Error::Code::Nothing);

		valStr = Test::stringify(KeyType::either);
		Types::Error fromStringEither = key.fromString(valStr);
		CHECK(fromStringEither == Types::Error::Code::Nothing);

		valStr = Test::stringify(KeyType::either+1);
		Types::Error fromStringWrong = key.fromString(valStr);
		CHECK(fromStringWrong == Types::Error::Code::InvalidValue);
	}

	SECTION("Int")
	{
		using KeyType = EitherOrInt;
		KeyType key;

		auto valStr = std::to_string(static_cast<int>(KeyType::default));
		Types::Error fromStringDefault = key.fromString(valStr);
		CHECK(fromStringDefault == Types::Error::Code::Nothing);
		CHECK(key.toString() == valStr);

		valStr = std::to_string(KeyType::default+.3f);
		Types::Error fromStringFloat = key.fromString(valStr);
		CHECK(fromStringFloat == Types::Error::Code::InvalidFormat);

		valStr = Test::stringify(KeyType::default);
		Types::Error fromStringString = key.fromString(valStr);
		CHECK(fromStringString == Types::Error::Code::InvalidFormat);
	}

	SECTION("StringFloat")
	{
		using KeyType = EitherOrStringFloat;
		KeyType key;

		auto valStr = Test::stringify(KeyType::default);
		Types::Error fromStringFloat = key.fromString(valStr);
		CHECK(fromStringFloat == Types::Error::Code::Nothing);
		CHECK(key.toString() == valStr);

		valStr = std::to_string(KeyType::default);
		Types::Error fromStringWrong = key.fromString(valStr);
		CHECK(fromStringWrong == Types::Error::Code::InvalidFormat);
	}

	SECTION("StringInt")
	{
		using KeyType = MinMaxStringInt;
		KeyType key;

		auto valStr = Test::stringify(KeyType::default);
		Types::Error fromStringDefault = key.fromString(valStr);
		CHECK(fromStringDefault == Types::Error::Code::Nothing);
		CHECK(key.toString() == valStr);

		valStr = Test::stringify(KeyType::default+1);
		Types::Error fromStringInt = key.fromString(valStr);
		CHECK(fromStringInt == Types::Error::Code::Nothing);

		valStr = Test::stringify(KeyType::default+.3f);
		Types::Error fromStringFloat = key.fromString(valStr);
		CHECK(fromStringFloat == Types::Error::Code::InvalidValue);

		valStr = std::to_string(KeyType::default);
		Types::Error fromStringString = key.fromString(valStr);
		CHECK(fromStringString == Types::Error::Code::InvalidFormat);
	}
}

TEST_CASE("INI::INI", "[INI]")
{
	SECTION("loadFile")
	{
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		INI::INI ini;
		CHECK(ini.loadFile(Test::Data::settingsDefault));
		Types::Error error = ini.extractError();
		CHECK(error == Types::Error::Code::Nothing);

		CHECK_FALSE(ini.loadFile(Test::Data::missing));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::FailedToLoadFile);
	}//TODO check if internal data has been replaced

	SECTION("has")
	{
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		INI::INI ini;
		REQUIRE(ini.loadFile(Test::Data::settingsDefault));

		auto cheats = std::make_shared<INI::Cheats>();
		CHECK(ini.has(cheats));
		Types::Error error = ini.extractError();
		CHECK(error == Types::Error::Code::Nothing);

		auto options = std::make_shared<INI::Options>();
		CHECK_FALSE(ini.has(options));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::SectionNotFound);
	}

	SECTION("read ISection")
	{
		REQUIRE_EXISTS(Test::Data::settingsValid);
		INI::INI ini;
		REQUIRE(ini.loadFile(Test::Data::settingsValid));

		auto cheats = std::make_shared<INI::Cheats>();

		CHECK(ini.read(cheats));
		Types::Error error = ini.extractError();
		CHECK(error == Types::Error::Code::Nothing);
		bool hasNewValue = cheats->getCheatsEnabled() == Types::CheatsEnabled::Enabled;
		CHECK(hasNewValue);

		REQUIRE_EXISTS(Test::Data::settingsMissingSection);
		REQUIRE(ini.loadFile(Test::Data::settingsMissingSection));

		CHECK_FALSE(ini.read(cheats));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::SectionNotFound);
		bool hasDefaultValue = cheats->getCheatsEnabled() == INI::CheatsEnabled();
		CHECK(hasDefaultValue);

		REQUIRE_EXISTS(Test::Data::settingsMissingKey);
		REQUIRE(ini.loadFile(Test::Data::settingsMissingKey));

		CHECK_FALSE(ini.read(cheats));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::KeyNotFound);
		hasDefaultValue = cheats->getCheatsEnabled() == INI::CheatsEnabled();
		CHECK(hasDefaultValue);
	}

	SECTION("read IIni")
	{
		REQUIRE_EXISTS(Test::Data::settingsValid);
		INI::INI ini;
		REQUIRE(ini.loadFile(Test::Data::settingsValid));

		auto settings = std::make_shared<INI::Settings>();

		CHECK(ini.read(settings));
		Types::Error error = ini.extractError();
		CHECK(error == Types::Error::Code::Nothing);
		bool hasNewValue = settings->getCheats().getCheatsEnabled() == Types::CheatsEnabled::Enabled;
		CHECK(hasNewValue);

		REQUIRE_EXISTS(Test::Data::settingsMissingSection);
		REQUIRE(ini.loadFile(Test::Data::settingsMissingSection));

		CHECK_FALSE(ini.read(settings));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::SectionNotFound);
		bool hasDefaultValue = settings->getCheats().getCheatsEnabled() == INI::CheatsEnabled();
		CHECK(hasDefaultValue);

		REQUIRE_EXISTS(Test::Data::settingsMissingKey);
		REQUIRE(ini.loadFile(Test::Data::settingsMissingKey));

		CHECK_FALSE(ini.read(settings));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::KeyNotFound);
		hasDefaultValue = settings->getCheats().getCheatsEnabled() == INI::CheatsEnabled();
		CHECK(hasDefaultValue);
	}
}
