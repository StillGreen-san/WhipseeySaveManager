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
		CHECK(key.asFloat() == KeyType::max);

		valStr = Test::stringify(KeyType::min);
		Types::Error fromStringMin = key.fromString(valStr);
		CHECK(fromStringMin == Types::Error::Code::Nothing);
		CHECK(key.asFloat() == KeyType::min);

		valStr = Test::stringify(KeyType::min+1);
		Types::Error fromStringBetween = key.fromString(valStr);
		CHECK(fromStringBetween == Types::Error::Code::Nothing);
		CHECK(key.asFloat() == KeyType::min+1);

		valStr = Test::stringify(KeyType::max+1);
		Types::Error fromStringLarger = key.fromString(valStr);
		CHECK(fromStringLarger == Types::Error::Code::InvalidValue);
		CHECK(key.asFloat() == KeyType::default);

		valStr = Test::stringify(KeyType::min-1);
		Types::Error fromStringSmaller = key.fromString(valStr);
		CHECK(fromStringSmaller == Types::Error::Code::InvalidValue);
		CHECK(key.asFloat() == KeyType::default);
	}
	
	SECTION("EitherOr")
	{
		using KeyType = EitherOrStringFloat;
		KeyType key;

		auto valStr = Test::stringify(KeyType::or);
		Types::Error fromStringOr = key.fromString(valStr);
		CHECK(fromStringOr == Types::Error::Code::Nothing);
		CHECK(key.asFloat() == KeyType::or);

		valStr = Test::stringify(KeyType::either);
		Types::Error fromStringEither = key.fromString(valStr);
		CHECK(fromStringEither == Types::Error::Code::Nothing);
		CHECK(key.asFloat() == KeyType::either);

		valStr = Test::stringify(KeyType::either+1);
		Types::Error fromStringWrong = key.fromString(valStr);
		CHECK(fromStringWrong == Types::Error::Code::InvalidValue);
		CHECK(key.asFloat() == KeyType::default);
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
		valStr = std::to_string(static_cast<int>(KeyType::default));
		CHECK(key.toString() == valStr);

		valStr = Test::stringify(KeyType::default);
		Types::Error fromStringString = key.fromString(valStr);
		CHECK(fromStringString == Types::Error::Code::InvalidFormat);
		valStr = std::to_string(static_cast<int>(KeyType::default));
		CHECK(key.toString() == valStr);
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
		valStr = Test::stringify(KeyType::default);
		CHECK(key.toString() == valStr);

		valStr = "\"0.000\"";
		Types::Error fromStringFractionalShort = key.fromString(valStr);
		CHECK(fromStringFractionalShort == Types::Error::Code::InvalidFormat);
		valStr = Test::stringify(KeyType::default);
		CHECK(key.toString() == valStr);
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
		CHECK(key.toString() == valStr);

		valStr = Test::stringify(KeyType::default+.3f);
		Types::Error fromStringFloat = key.fromString(valStr);
		CHECK(fromStringFloat == Types::Error::Code::InvalidValue);
		valStr = Test::stringify(KeyType::default);
		CHECK(key.toString() == valStr);

		valStr = std::to_string(KeyType::default);
		Types::Error fromStringString = key.fromString(valStr);
		CHECK(fromStringString == Types::Error::Code::InvalidFormat);
		valStr = Test::stringify(KeyType::default);
		CHECK(key.toString() == valStr);

		valStr = "\"0.00000000\"";
		Types::Error fromStringFractionalLong = key.fromString(valStr);
		CHECK(fromStringFractionalLong == Types::Error::Code::InvalidFormat);
		valStr = Test::stringify(KeyType::default);
		CHECK(key.toString() == valStr);
	}
}

TEST_CASE("INI::INI", "[INI]")
{
	INI::INI ini;

	SECTION("loadFile")
	{
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		CHECK(ini.loadFile(Test::Data::settingsDefault));
		Types::Error error = ini.extractError();
		CHECK(error == Types::Error::Code::Nothing);
		auto cheats = std::make_shared<INI::Cheats>();
		CHECK(ini.has(cheats));

		CHECK_FALSE(ini.loadFile(Test::Data::missing));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::FailedToLoadFile);
		CHECK_FALSE(ini.has(cheats));
	}

	SECTION("writeFile")
	{
		REQUIRE_MISSING(Test::Data::write);
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		REQUIRE(ini.loadFile(Test::Data::settingsDefault));
		auto cheatsDefault = std::make_shared<INI::Cheats>();
		REQUIRE(ini.read(cheatsDefault));

		CHECK(ini.writeFile(Test::Data::write));
		Types::Error error = ini.extractError();
		REQUIRE(error == Types::Error::Code::Nothing);

		REQUIRE(ini.loadFile(Test::Data::write));
		auto cheatsWrite = std::make_shared<INI::Cheats>();
		REQUIRE(ini.read(cheatsWrite));

		CHECK(cheatsDefault->getCheatsEnabled() == cheatsWrite->getCheatsEnabled());

		REMOVE(Test::Data::write);
	}

	SECTION("extractError")
	{
		REQUIRE_MISSING(Test::Data::missing);
		ini.loadFile(Test::Data::missing);
		Types::Error error = ini.extractError();
		CHECK(error == Types::Error::Code::FailedToLoadFile);
		error = ini.extractError();
		CHECK(error == Types::Error::Code::Nothing);
	}

	SECTION("has")
	{
		REQUIRE_EXISTS(Test::Data::settingsDefault);
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
		REQUIRE(ini.loadFile(Test::Data::settingsValid));

		auto cheats = std::make_shared<INI::Cheats>();

		CHECK(ini.read(cheats));
		Types::Error error = ini.extractError();
		CHECK(error == Types::Error::Code::Nothing);
		CHECK(cheats->getCheatsEnabled() == Types::CheatsEnabled::Enabled);

		REQUIRE_EXISTS(Test::Data::settingsMissingSection);
		REQUIRE(ini.loadFile(Test::Data::settingsMissingSection));

		CHECK_FALSE(ini.read(cheats));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::SectionNotFound);
		CHECK(cheats->getCheatsEnabled() == INI::CheatsEnabled());

		REQUIRE_EXISTS(Test::Data::settingsMissingKey);
		REQUIRE(ini.loadFile(Test::Data::settingsMissingKey));

		CHECK_FALSE(ini.read(cheats));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::KeyNotFound);
		CHECK(cheats->getCheatsEnabled() == INI::CheatsEnabled());
	}

	SECTION("read IIni")
	{
		REQUIRE_EXISTS(Test::Data::settingsValid);
		REQUIRE(ini.loadFile(Test::Data::settingsValid));

		auto settings = std::make_shared<INI::Settings>();

		CHECK(ini.read(settings));
		Types::Error error = ini.extractError();
		CHECK(error == Types::Error::Code::Nothing);
		CHECK(settings->getCheats().getCheatsEnabled() == Types::CheatsEnabled::Enabled);

		REQUIRE_EXISTS(Test::Data::settingsMissingSection);
		REQUIRE(ini.loadFile(Test::Data::settingsMissingSection));

		CHECK_FALSE(ini.read(settings));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::SectionNotFound);
		CHECK(settings->getCheats().getCheatsEnabled() == INI::CheatsEnabled());

		REQUIRE_EXISTS(Test::Data::settingsMissingKey);
		REQUIRE(ini.loadFile(Test::Data::settingsMissingKey));

		CHECK_FALSE(ini.read(settings));
		error = ini.extractError();
		CHECK(error == Types::Error::Code::KeyNotFound);
		CHECK(settings->getCheats().getCheatsEnabled() == INI::CheatsEnabled());
	}

	SECTION("write ISection")
	{
		REQUIRE_EXISTS(Test::Data::settingsValid);
		REQUIRE(ini.loadFile(Test::Data::settingsValid));

		auto cheatsValid = std::make_shared<INI::Cheats>();

		REQUIRE(ini.read(cheatsValid));
		
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		REQUIRE(ini.loadFile(Test::Data::settingsDefault));

		auto cheatsDefault = std::make_shared<INI::Cheats>();

		REQUIRE(ini.read(cheatsDefault));

		ini.write(cheatsValid);

		REQUIRE(ini.read(cheatsDefault));

		CHECK(cheatsDefault->getCheatsEnabled() == cheatsValid->getCheatsEnabled());
	}

	SECTION("write IIni")
	{
		REQUIRE_EXISTS(Test::Data::settingsValid);
		REQUIRE(ini.loadFile(Test::Data::settingsValid));

		auto cheatsValid = std::make_shared<INI::Settings>();

		REQUIRE(ini.read(cheatsValid));
		
		REQUIRE_EXISTS(Test::Data::settingsDefault);
		REQUIRE(ini.loadFile(Test::Data::settingsDefault));

		auto cheatsDefault = std::make_shared<INI::Settings>();

		REQUIRE(ini.read(cheatsDefault));

		ini.write(cheatsValid);

		REQUIRE(ini.read(cheatsDefault));

		CHECK(cheatsDefault->getCheats().getCheatsEnabled() == cheatsValid->getCheats().getCheatsEnabled());
	}
}

TEST_CASE("FileBase")
{
	INI::File1 file;
	file.getCastle() = Types::Castle::Locked;
	file.getMoon() = Types::Moon::Locked;
	file.getSnow() = Types::Snow::Locked;
	file.getDesert() = Types::Desert::Locked;
	file.getForest() = Types::Forest::Locked;

	SECTION("getLevel")
	{
		REQUIRE(file.getLevel() == Types::Level::Beach);

		file.getDesert() = Types::Desert::Unlocked;
		file.getForest() = Types::Forest::Unlocked;

		REQUIRE(file.getLevel() == Types::Level::Desert);

		file.getCastle() = Types::Castle::Unlocked;
		file.getMoon() = Types::Moon::Unlocked;
		file.getSnow() = Types::Snow::Unlocked;

		REQUIRE(file.getLevel() == Types::Level::Castle);
	}

	SECTION("setLevel")
	{
		file.setLevel(Types::Level::Desert);
		REQUIRE(file.getDesert() == Types::Desert::Unlocked);
		REQUIRE(file.getForest() == Types::Forest::Unlocked);

		file.setLevel(Types::Level::Moon);
		REQUIRE(file.getDesert() == Types::Desert::Unlocked);
		REQUIRE(file.getForest() == Types::Forest::Unlocked);
		REQUIRE(file.getSnow() == Types::Snow::Unlocked);
		REQUIRE(file.getMoon() == Types::Moon::Unlocked);

		file.setLevel(Types::Level::Forest);
		REQUIRE(file.getSnow() == Types::Snow::Locked);
		REQUIRE(file.getMoon() == Types::Moon::Locked);
		REQUIRE(file.getDesert() == Types::Desert::Locked);
	}
}

TEST_CASE("BossNoDamageProgress")
{
	REQUIRE_EXISTS(Test::Data::fileMissingBossnodamage);
	INI::INI ini;
	REQUIRE(ini.loadFile(Test::Data::fileMissingBossnodamage));

	auto file = std::make_shared<INI::File1>();

	REQUIRE(ini.read(file));

	REQUIRE(file->getBossNoDamageProgress() == Types::BossNoDamage::None);
}

// ##########

TEST_CASE("Language:IKey")
{
	INI::Language language;

	CHECK(language == Types::Language::English);
	CHECK(language.key() == "language");
	
	language = Types::Language::Japanese;
	language.applyDefaults();
	CHECK(static_cast<Types::Language>(language) == Types::Language::English);
	CHECK(language.asFloat() == static_cast<float>(Types::Language::English));

	const auto LANGUAGES = {
		Types::Language::Chinese,Types::Language::English,Types::Language::French,Types::Language::German,
		Types::Language::Italian,Types::Language::Japanese,Types::Language::Portogese,
		Types::Language::Russian,Types::Language::Spanish,Types::Language::Swedish
	};

	for(Types::Language LANG : LANGUAGES)
	{
		language = LANG;
		CHECK(static_cast<Types::Language>(language) == LANG);
		CHECK(language.asFloat() == static_cast<float>(LANG));
	}
}

TEST_CASE("Scale:IKey")
{
	INI::Scale scale;

	CHECK(scale == Types::Scale::R768x432);
	CHECK(scale.key() == "scale");
	
	scale = Types::Scale::R1536x864;
	scale.applyDefaults();
	CHECK(static_cast<Types::Scale>(scale) == Types::Scale::R768x432);
	CHECK(scale.asFloat() == static_cast<float>(Types::Scale::R768x432));

	const auto SCALES = {
		Types::Scale::R1152x648,Types::Scale::R1536x864,Types::Scale::R768x432
	};

	for(Types::Scale SCALE : SCALES)
	{
		scale = SCALE;
		CHECK(static_cast<Types::Scale>(scale) == SCALE);
		CHECK(scale.asFloat() == static_cast<float>(SCALE));
	}
}

TEST_CASE("Fullscreen:IKey")
{
	INI::Fullscreen fullscreen;

	CHECK(fullscreen == Types::Fullscreen::Enabled);
	CHECK(fullscreen.key() == "fullscreen");
	
	fullscreen = Types::Fullscreen::Disabled;
	fullscreen.applyDefaults();
	CHECK(static_cast<Types::Fullscreen>(fullscreen) == Types::Fullscreen::Enabled);
	CHECK(fullscreen.asFloat() == static_cast<float>(Types::Fullscreen::Enabled));

	const auto FULLSCREENS = {
		Types::Fullscreen::Disabled,Types::Fullscreen::Enabled
	};

	for(Types::Fullscreen FS : FULLSCREENS)
	{
		fullscreen = FS;
		CHECK(static_cast<Types::Fullscreen>(fullscreen) == FS);
		CHECK(fullscreen.asFloat() == static_cast<float>(FS));
	}
}
