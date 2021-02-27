#include "testhelper.hpp"

namespace WhipseeySaveManager::Test
{
	Types::Options makeInvalidOptions()
	{
		Types::Options options;
		options.language = maxEnumVal<Types::Language>();
		options.scale = maxEnumVal<Types::Scale>();
		options.fullScreen = maxEnumVal<Types::Toggle>();
		options.leftHanded = maxEnumVal<Types::Toggle>();
		options.sound.toggle = maxEnumVal<Types::Toggle>();
		options.sound.volume = maxEnumVal<Types::Volume>();
		options.music.toggle = maxEnumVal<Types::Toggle>();
		options.music.volume = maxEnumVal<Types::Volume>();
		return options;
	}

	Types::File makeInvalidFile()
	{
		Types::File file;
		file.noDamage = maxEnumVal<Types::BossNoDamage>();
		file.defeated = maxTypeVal(file.defeated);
		file.progress = maxEnumVal<Types::Level>();
		file.ending = maxEnumVal<Types::Toggle>();
		file.intro = maxEnumVal<Types::Toggle>();
		file.lives = maxTypeVal(file.lives);
		file.gems = maxTypeVal(file.gems);
		return file;
	}

	Types::Save makeInvalidSave()
	{
		Types::Save save;
		save.options = makeInvalidOptions();
		save.file1 = makeInvalidFile();
		save.file2 = makeInvalidFile();
		save.file3 = makeInvalidFile();
		return save;
	}

	Types::Options makeValidOptions()
	{
		Types::Options options;
		options.language = Types::Language::Japanese;
		options.scale = Types::Scale::R1152x648;
		options.fullScreen = Types::Toggle::Disabled;
		options.leftHanded = Types::Toggle::Enabled;
		options.sound.toggle = Types::Toggle::Disabled;
		options.sound.volume = Types::Volume::V30;
		options.music.toggle = Types::Toggle::Disabled;
		options.music.volume = Types::Volume::V70;
		return options;
	}

	Types::File makeValidFile()
	{
		Types::File file;
		file.noDamage = Types::BossNoDamage::All;
		file.defeated = maxTypeVal(file.defeated);
		file.progress = Types::Level::Castle;
		file.ending = Types::Toggle::Enabled;
		file.intro = Types::Toggle::Enabled;
		file.lives = 9999;
		file.gems = 99;//TODO use named constants
		return file;
	}

	Types::Save makeValidSave()
	{
		Types::Save save;
		save.options = makeValidOptions();
		save.file1 = makeValidFile();
		save.file2 = makeValidFile();
		save.file3 = makeValidFile();
		return save;
	}

	Types::Settings makeInvalidSettings()
	{
		Types::Settings settings;
		settings.cheats = maxEnumVal<Types::Toggle>();
		return settings;
	}

	Types::Settings makeValidSettings()
	{
		Types::Settings settings;
		settings.cheats = Types::Toggle::Enabled;
		return settings;
	}
} // namespace WhipseeySaveManager::Test
