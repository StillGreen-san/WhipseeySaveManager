#include "testhelper.hpp"

namespace WhipseeySaveManager
{
namespace Test
{
	Data::Options makeInvalidOptions()
	{
		Data::Options options;
		options.language = maxEnumVal<Data::Language>();
		options.scale = maxEnumVal<Data::Scale>();
		options.fullScreen = maxEnumVal<Data::Toggle>();
		options.leftHanded = maxEnumVal<Data::Toggle>();
		options.sound.toggle = maxEnumVal<Data::Toggle>();
		options.sound.volume = maxEnumVal<Data::Volume>();
		options.music.toggle = maxEnumVal<Data::Toggle>();
		options.music.volume = maxEnumVal<Data::Volume>();
		return options;
	}

	Data::File makeInvalidFile()
	{
		Data::File file;
		file.noDamage = maxEnumVal<Data::BossNoDamage>();
		file.defeated = maxTypeVal(file.defeated);
		file.progress = maxEnumVal<Data::Level>();
		file.ending = maxEnumVal<Data::Toggle>();
		file.intro = maxEnumVal<Data::Toggle>();
		file.lives = maxTypeVal(file.lives);
		file.gems = maxTypeVal(file.gems);
		return file;
	}

	Data::Save makeInvalidSave()
	{
		Data::Save save;
		save.options = makeInvalidOptions();
		save.file1 = makeInvalidFile();
		save.file2 = makeInvalidFile();
		save.file3 = makeInvalidFile();
		return save;
	}

	Data::Options makeValidOptions()
	{
		Data::Options options;
		options.language = Data::Language::Japanese;
		options.scale = Data::Scale::R1152x648;
		options.fullScreen = Data::Toggle::Disabled;
		options.leftHanded = Data::Toggle::Enabled;
		options.sound.toggle = Data::Toggle::Disabled;
		options.sound.volume = Data::Volume::V30;
		options.music.toggle = Data::Toggle::Disabled;
		options.music.volume = Data::Volume::V70;
		return options;
	}

	Data::File makeValidFile()
	{
		Data::File file;
		file.noDamage = Data::BossNoDamage::All;
		file.defeated = maxTypeVal(file.defeated);
		file.progress = Data::Level::Castle;
		file.ending = Data::Toggle::Enabled;
		file.intro = Data::Toggle::Enabled;
		file.lives = 9999;
		file.gems = 99;//TODO use named constants
		return file;
	}

	Data::Save makeValidSave()
	{
		Data::Save save;
		save.options = makeValidOptions();
		save.file1 = makeValidFile();
		save.file2 = makeValidFile();
		save.file3 = makeValidFile();
		return save;
	}

	Data::Settings makeInvalidSettings()
	{
		Data::Settings settings;
		settings.cheats = maxEnumVal<Data::Toggle>();
		return settings;
	}

	Data::Settings makeValidSettings()
	{
		Data::Settings settings;
		settings.cheats = Data::Toggle::Enabled;
		return settings;
	}

	bool prepareTetstdata()
	{
		std::filesystem::path dataPath("/data");
		std::error_code error;
		bool exists = std::filesystem::exists(dataPath, error);
		if(error || !exists) return false;
		
		return false;

		// std::filesystem::perms permissions;
		// std::filesystem::permissions(dataPath, , error);
	}
} // namespace Test
} // namespace WhipseeySaveManager
