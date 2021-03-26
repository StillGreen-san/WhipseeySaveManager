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

	const auto VALUES = {
		Types::Language::Chinese,Types::Language::English,Types::Language::French,Types::Language::German,
		Types::Language::Italian,Types::Language::Japanese,Types::Language::Portogese,
		Types::Language::Russian,Types::Language::Spanish,Types::Language::Swedish
	};

	for(Types::Language LANG : VALUES)
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

	const auto VALUES = {
		Types::Scale::R1152x648,Types::Scale::R1536x864,Types::Scale::R768x432
	};

	for(Types::Scale SCALE : VALUES)
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

	const auto VALUES = {
		Types::Fullscreen::Disabled,Types::Fullscreen::Enabled
	};

	for(Types::Fullscreen FS : VALUES)
	{
		fullscreen = FS;
		CHECK(static_cast<Types::Fullscreen>(fullscreen) == FS);
		CHECK(fullscreen.asFloat() == static_cast<float>(FS));
	}
}

TEST_CASE("LeftHanded:IKey")
{
	INI::LeftHanded leftHanded;

	CHECK(leftHanded == Types::LeftHanded::Enabled);
	CHECK(leftHanded.key() == "left_handed");
	
	leftHanded = Types::LeftHanded::Disabled;
	leftHanded.applyDefaults();
	CHECK(static_cast<Types::LeftHanded>(leftHanded) == Types::LeftHanded::Enabled);
	CHECK(leftHanded.asFloat() == static_cast<float>(Types::LeftHanded::Enabled));

	const auto VALUES = {
		Types::LeftHanded::Disabled,Types::LeftHanded::Enabled
	};

	for(Types::LeftHanded HANDED : VALUES)
	{
		leftHanded = HANDED;
		CHECK(static_cast<Types::LeftHanded>(leftHanded) == HANDED);
		CHECK(leftHanded.asFloat() == static_cast<float>(HANDED));
	}
}

TEST_CASE("SoundVolume:IKey")
{
	INI::SoundVolume soundVolume;

	CHECK(soundVolume == Types::SoundVolume::V100);
	CHECK(soundVolume.key() == "sound_volume");
	
	soundVolume = Types::SoundVolume::V0;
	soundVolume.applyDefaults();
	CHECK(static_cast<Types::SoundVolume>(soundVolume) == Types::SoundVolume::V100);
	CHECK(soundVolume.asFloat() == static_cast<float>(Types::SoundVolume::V100) / 10);

	const auto VALUES = {
		Types::SoundVolume::V0,Types::SoundVolume::V10,Types::SoundVolume::V20,Types::SoundVolume::V30,
		Types::SoundVolume::V40,Types::SoundVolume::V50,Types::SoundVolume::V60,Types::SoundVolume::V70,
		Types::SoundVolume::V80,Types::SoundVolume::V90,Types::SoundVolume::V100
	};

	for(Types::SoundVolume SVOLUME : VALUES)
	{
		soundVolume = SVOLUME;
		CHECK(static_cast<Types::SoundVolume>(soundVolume) == SVOLUME);
		CHECK(soundVolume.asFloat() == static_cast<float>(SVOLUME) / 10);
	}
}

TEST_CASE("SoundToggle:IKey")
{
	INI::SoundToggle soundToggle;

	CHECK(soundToggle == Types::SoundToggle::Enabled);
	CHECK(soundToggle.key() == "sound_toggle");
	
	soundToggle = Types::SoundToggle::Disabled;
	soundToggle.applyDefaults();
	CHECK(static_cast<Types::SoundToggle>(soundToggle) == Types::SoundToggle::Enabled);
	CHECK(soundToggle.asFloat() == static_cast<float>(Types::SoundToggle::Enabled));

	const auto VALUES = {
		Types::SoundToggle::Disabled,Types::SoundToggle::Enabled
	};

	for(Types::SoundToggle STOGGLE : VALUES)
	{
		soundToggle = STOGGLE;
		CHECK(static_cast<Types::SoundToggle>(soundToggle) == STOGGLE);
		CHECK(soundToggle.asFloat() == static_cast<float>(STOGGLE));
	}
}

TEST_CASE("MusicVolume:IKey")
{
	INI::MusicVolume musicVolume;

	CHECK(musicVolume == Types::MusicVolume::V100);
	CHECK(musicVolume.key() == "music_volume");
	
	musicVolume = Types::MusicVolume::V0;
	musicVolume.applyDefaults();
	CHECK(static_cast<Types::MusicVolume>(musicVolume) == Types::MusicVolume::V100);
	CHECK(musicVolume.asFloat() == static_cast<float>(Types::MusicVolume::V100) / 10);

	const auto VALUES = {
		Types::MusicVolume::V0,Types::MusicVolume::V10,Types::MusicVolume::V20,Types::MusicVolume::V30,
		Types::MusicVolume::V40,Types::MusicVolume::V50,Types::MusicVolume::V60,Types::MusicVolume::V70,
		Types::MusicVolume::V80,Types::MusicVolume::V90,Types::MusicVolume::V100
	};

	for(Types::MusicVolume MVOLUME : VALUES)
	{
		musicVolume = MVOLUME;
		CHECK(static_cast<Types::MusicVolume>(musicVolume) == MVOLUME);
		CHECK(musicVolume.asFloat() == static_cast<float>(MVOLUME) / 10);
	}
}

TEST_CASE("MusicToggle:IKey")
{
	INI::MusicToggle musicToggle;

	CHECK(musicToggle == Types::MusicToggle::Enabled);
	CHECK(musicToggle.key() == "music_toggle");
	
	musicToggle = Types::MusicToggle::Disabled;
	musicToggle.applyDefaults();
	CHECK(static_cast<Types::MusicToggle>(musicToggle) == Types::MusicToggle::Enabled);
	CHECK(musicToggle.asFloat() == static_cast<float>(Types::MusicToggle::Enabled));

	const auto VALUES = {
		Types::MusicToggle::Disabled,Types::MusicToggle::Enabled
	};

	for(Types::MusicToggle MTOGGLE : VALUES)
	{
		musicToggle = MTOGGLE;
		CHECK(static_cast<Types::MusicToggle>(musicToggle) == MTOGGLE);
		CHECK(musicToggle.asFloat() == static_cast<float>(MTOGGLE));
	}
}


TEST_CASE("BossNoDamageProgress:IKey")
{
	SECTION("missing")
	{
		REQUIRE_EXISTS(Test::Data::fileMissingBossnodamage);
		INI::INI ini;
		REQUIRE(ini.loadFile(Test::Data::fileMissingBossnodamage));

		auto file = std::make_shared<INI::File1>();

		CHECK(ini.read(file));

		REQUIRE(file->getBossNoDamageProgress() == Types::BossNoDamage::None);
	}
	
	SECTION("IKey")
	{
		INI::BossNoDamageProgress bossProgress;

		CHECK(bossProgress == Types::BossNoDamage::None);
		CHECK(bossProgress.key() == "boss_no_damage_progress");
		
		bossProgress = Types::BossNoDamage::Castle;
		bossProgress.applyDefaults();
		CHECK(static_cast<Types::BossNoDamage>(bossProgress) == Types::BossNoDamage::None);
		CHECK(bossProgress.asFloat() == static_cast<float>(Types::BossNoDamage::None));

		const auto VALUES = {
			Types::BossNoDamage::All,Types::BossNoDamage::Castle,Types::BossNoDamage::Desert,
			Types::BossNoDamage::Forest,Types::BossNoDamage::None
		};

		for(Types::BossNoDamage PROGRESS : VALUES)
		{
			bossProgress = PROGRESS;
			CHECK(static_cast<Types::BossNoDamage>(bossProgress) == PROGRESS);
			CHECK(bossProgress.asFloat() == static_cast<float>(PROGRESS));
		}
	}
}

TEST_CASE("Castle:IKey")
{
	INI::Castle castle;

	CHECK(castle == Types::Castle::Locked);
	CHECK(castle.key() == "castle");
	
	castle = Types::Castle::Unlocked;
	castle.applyDefaults();
	CHECK(static_cast<Types::Castle>(castle) == Types::Castle::Locked);
	CHECK(castle.asFloat() == static_cast<float>(Types::Castle::Locked));

	const auto VALUES = {
		Types::Castle::Unlocked,Types::Castle::Locked
	};

	for(Types::Castle STATUS : VALUES)
	{
		castle = STATUS;
		CHECK(static_cast<Types::Castle>(castle) == STATUS);
		CHECK(castle.asFloat() == static_cast<float>(STATUS));
	}
}

TEST_CASE("Moon:IKey")
{
	INI::Moon moon;

	CHECK(moon == Types::Moon::Locked);
	CHECK(moon.key() == "moon");
	
	moon = Types::Moon::Unlocked;
	moon.applyDefaults();
	CHECK(static_cast<Types::Moon>(moon) == Types::Moon::Locked);
	CHECK(moon.asFloat() == static_cast<float>(Types::Moon::Locked));

	const auto VALUES = {
		Types::Moon::Unlocked,Types::Moon::Locked
	};

	for(Types::Moon STATUS : VALUES)
	{
		moon = STATUS;
		CHECK(static_cast<Types::Moon>(moon) == STATUS);
		CHECK(moon.asFloat() == static_cast<float>(STATUS));
	}
}

TEST_CASE("Snow:IKey")
{
	INI::Snow snow;

	CHECK(snow == Types::Snow::Locked);
	CHECK(snow.key() == "snow");
	
	snow = Types::Snow::Unlocked;
	snow.applyDefaults();
	CHECK(static_cast<Types::Snow>(snow) == Types::Snow::Locked);
	CHECK(snow.asFloat() == static_cast<float>(Types::Snow::Locked));

	const auto VALUES = {
		Types::Snow::Unlocked,Types::Snow::Locked
	};

	for(Types::Snow STATUS : VALUES)
	{
		snow = STATUS;
		CHECK(static_cast<Types::Snow>(snow) == STATUS);
		CHECK(snow.asFloat() == static_cast<float>(STATUS));
	}
}

TEST_CASE("Desert:IKey")
{
	INI::Desert desert;

	CHECK(desert == Types::Desert::Locked);
	CHECK(desert.key() == "desert");
	
	desert = Types::Desert::Unlocked;
	desert.applyDefaults();
	CHECK(static_cast<Types::Desert>(desert) == Types::Desert::Locked);
	CHECK(desert.asFloat() == static_cast<float>(Types::Desert::Locked));

	const auto VALUES = {
		Types::Desert::Unlocked,Types::Desert::Locked
	};

	for(Types::Desert STATUS : VALUES)
	{
		desert = STATUS;
		CHECK(static_cast<Types::Desert>(desert) == STATUS);
		CHECK(desert.asFloat() == static_cast<float>(STATUS));
	}
}

TEST_CASE("Forest:IKey")
{
	INI::Forest forest;

	CHECK(forest == Types::Forest::Locked);
	CHECK(forest.key() == "forest");
	
	forest = Types::Forest::Unlocked;
	forest.applyDefaults();
	CHECK(static_cast<Types::Forest>(forest) == Types::Forest::Locked);
	CHECK(forest.asFloat() == static_cast<float>(Types::Forest::Locked));

	const auto VALUES = {
		Types::Forest::Unlocked,Types::Forest::Locked
	};

	for(Types::Forest STATUS : VALUES)
	{
		forest = STATUS;
		CHECK(static_cast<Types::Forest>(forest) == STATUS);
		CHECK(forest.asFloat() == static_cast<float>(STATUS));
	}
}

TEST_CASE("Ending:IKey")
{
	INI::Ending ending;

	CHECK(ending == Types::Ending::Unwatched);
	CHECK(ending.key() == "ending");
	
	ending = Types::Ending::Watched;
	ending.applyDefaults();
	CHECK(static_cast<Types::Ending>(ending) == Types::Ending::Unwatched);
	CHECK(ending.asFloat() == static_cast<float>(Types::Ending::Unwatched));

	const auto VALUES = {
		Types::Ending::Watched,Types::Ending::Unwatched
	};

	for(Types::Ending STATUS : VALUES)
	{
		ending = STATUS;
		CHECK(static_cast<Types::Ending>(ending) == STATUS);
		CHECK(ending.asFloat() == static_cast<float>(STATUS));
	}
}

TEST_CASE("Intro:IKey")
{
	INI::Intro intro;

	CHECK(intro == Types::Intro::Unwatched);
	CHECK(intro.key() == "intro");
	
	intro = Types::Intro::Watched;
	intro.applyDefaults();
	CHECK(static_cast<Types::Intro>(intro) == Types::Intro::Unwatched);
	CHECK(intro.asFloat() == static_cast<float>(Types::Intro::Unwatched));

	const auto VALUES = {
		Types::Intro::Watched,Types::Intro::Unwatched
	};

	for(Types::Intro STATUS : VALUES)
	{
		intro = STATUS;
		CHECK(static_cast<Types::Intro>(intro) == STATUS);
		CHECK(intro.asFloat() == static_cast<float>(STATUS));
	}
}

TEST_CASE("CheatsEnabled:IKey")
{
	INI::CheatsEnabled intro;

	CHECK(intro == Types::CheatsEnabled::Disabled);
	CHECK(intro.key() == "cheats_enabled");
	
	intro = Types::CheatsEnabled::Enabled;
	intro.applyDefaults();
	CHECK(static_cast<Types::CheatsEnabled>(intro) == Types::CheatsEnabled::Disabled);
	CHECK(intro.asFloat() == static_cast<float>(Types::CheatsEnabled::Disabled));

	const auto VALUES = {
		Types::CheatsEnabled::Enabled,Types::CheatsEnabled::Disabled
	};

	for(Types::CheatsEnabled STATUS : VALUES)
	{
		intro = STATUS;
		CHECK(static_cast<Types::CheatsEnabled>(intro) == STATUS);
		CHECK(intro.asFloat() == static_cast<float>(STATUS));
	}
}
