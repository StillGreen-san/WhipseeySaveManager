#include "ini.hpp"
#include "testhelper.hpp"

#include <catch.hpp>

using namespace WhipseeySaveManager;

TEST_CASE("Language:IKey", "[INI]")
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

TEST_CASE("Scale:IKey", "[INI]")
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

TEST_CASE("Fullscreen:IKey", "[INI]")
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

TEST_CASE("LeftHanded:IKey", "[INI]")
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

TEST_CASE("SoundVolume:IKey", "[INI]")
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

TEST_CASE("SoundToggle:IKey", "[INI]")
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

TEST_CASE("MusicVolume:IKey", "[INI]")
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

TEST_CASE("MusicToggle:IKey", "[INI]")
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

TEST_CASE("Options:ISection", "[INI]")
{
	INI::Options options;

	CHECK(options.section() == "options");

	CHECK(options.getLanguage().key() == "language");
	CHECK(options.getScale().key() == "scale");
	CHECK(options.getFullscreen().key() == "fullscreen");
	CHECK(options.getLeftHanded().key() == "left_handed");
	CHECK(options.getSoundVolume().key() == "sound_volume");
	CHECK(options.getSoundToggle().key() == "sound_toggle");
	CHECK(options.getMusicVolume().key() == "music_volume");
	CHECK(options.getMusicToggle().key() == "music_toggle");

	const auto& ikeys = options.keys();

	CHECK(ikeys[0]->key() == "language");
	CHECK(ikeys[1]->key() == "scale");
	CHECK(ikeys[2]->key() == "fullscreen");
	CHECK(ikeys[3]->key() == "left_handed");
	CHECK(ikeys[4]->key() == "sound_volume");
	CHECK(ikeys[5]->key() == "sound_toggle");
	CHECK(ikeys[6]->key() == "music_volume");
	CHECK(ikeys[7]->key() == "music_toggle");
}


TEST_CASE("BossNoDamageProgress:IKey", "[INI]")
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

TEST_CASE("Castle:IKey", "[INI]")
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

TEST_CASE("Moon:IKey", "[INI]")
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

TEST_CASE("Snow:IKey", "[INI]")
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

TEST_CASE("Desert:IKey", "[INI]")
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

TEST_CASE("Forest:IKey", "[INI]")
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

TEST_CASE("Ending:IKey", "[INI]")
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

TEST_CASE("Intro:IKey", "[INI]")
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

TEST_CASE("FileBase:ISection", "[INI]")
{
	struct FileBaseMock : public INI::FileBase
	{
		FileBaseMock() : INI::FileBase("TEST") {}
	};
	FileBaseMock file;

	CHECK(file.section() == "TEST");

	CHECK(file.getBossNoDamageProgress().key() == "boss_no_damage_progress");
	CHECK(file.getEnemiesDefeated().key() == "enemies_defeated");
	CHECK(file.getCastle().key() == "castle");
	CHECK(file.getMoon().key() == "moon");
	CHECK(file.getSnow().key() == "snow");
	CHECK(file.getDesert().key() == "desert");
	CHECK(file.getForest().key() == "forest");
	CHECK(file.getEnding().key() == "ending");
	CHECK(file.getIntro().key() == "intro");
	CHECK(file.getLives().key() == "lives");
	CHECK(file.getGems().key() == "gems");

	const auto& ikeys = file.keys();

	CHECK(ikeys[0]->key() == "boss_no_damage_progress");
	CHECK(ikeys[1]->key() == "enemies_defeated");
	CHECK(ikeys[2]->key() == "castle");
	CHECK(ikeys[3]->key() == "moon");
	CHECK(ikeys[4]->key() == "snow");
	CHECK(ikeys[5]->key() == "desert");
	CHECK(ikeys[6]->key() == "forest");
	CHECK(ikeys[7]->key() == "ending");
	CHECK(ikeys[8]->key() == "intro");
	CHECK(ikeys[9]->key() == "lives");
	CHECK(ikeys[10]->key() == "gems");
}

TEST_CASE("FileX:ISection", "[INI]")
{
	SECTION("File1")
	{
		INI::File1 file1;
		CHECK(file1.section() == "file1");
	}

	SECTION("File2")
	{
		INI::File2 file2;
		CHECK(file2.section() == "file2");
	}

	SECTION("File3")
	{
		INI::File3 file3;
		CHECK(file3.section() == "file3");
	}
}

TEST_CASE("Save:IIni", "[INI]")
{
	INI::Save save;

	CHECK(save.getOptions()->section() == "options");
	CHECK(save.getFile1()->section() == "file1");
	CHECK(save.getFile2()->section() == "file2");
	CHECK(save.getFile3()->section() == "file3");

	const auto& ikeys = save.sections();

	CHECK(ikeys[0]->section() == "options");
	CHECK(ikeys[1]->section() == "file1");
	CHECK(ikeys[2]->section() == "file2");
	CHECK(ikeys[3]->section() == "file3");
}


TEST_CASE("CheatsEnabled:IKey", "[INI]")
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

TEST_CASE("Cheats:ISection", "[INI]")
{
	INI::Cheats cheats;

	CHECK(cheats.section() == "Cheats");

	CHECK(cheats.getCheatsEnabled().key() == "cheats_enabled");

	const auto& ikeys = cheats.keys();

	CHECK(ikeys[0]->key() == "cheats_enabled");
}

TEST_CASE("Cheats:IIni", "[INI]")
{
	INI::Settings settings;

	CHECK(settings.getCheats().section() == "Cheats");

	const auto& ikeys = settings.sections();

	CHECK(ikeys[0]->section() == "Cheats");
}
