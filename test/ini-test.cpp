#include "ini.hpp"
#include "testhelper.hpp"

#include <catch.hpp>

using namespace WhipseeySaveManager;
using WhipseeySaveManager::INI::IKey;

struct MinMaxStringInt final : public IKey
{
	MinMaxStringInt() : IKey(
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
struct EitherOrStringFloat final : public IKey
{
	EitherOrStringFloat() : IKey(
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
struct EitherOrInt final : public IKey
{
	EitherOrInt() : IKey(
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
		CHECK(key.key() == KeyType::name);
		CHECK(key.mMinOrA == KeyType::min);
		CHECK(key.mMaxOrB == KeyType::max);
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
	// bool loadFile(const std::filesystem::path& path);
	SECTION("loadFile")
	{
		INI::INI ini;
		const std::filesystem::path settingsDefault(L"data/settings-default.ini");//TODO move paths to testhelper?
		CHECK(ini.loadFile(settingsDefault));
		Types::Error error = ini.extractError();
		CHECK(error == Types::Error::Code::Nothing);

		CHECK_FALSE(ini.loadFile(Test::missing));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::FailedToLoadFile);
	}
	// bool has(const std::shared_ptr<ISection>& section);
	// bool read(const std::shared_ptr<ISection>& section);
	// bool read(const std::shared_ptr<IIni>& ini);
}
