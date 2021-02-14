#pragma once

#include <filesystem>

namespace WhipseeySaveManager
{
namespace Types
{
	enum class Toggle : uint8_t
	{
		Disabled = false,
		Enabled = true
	};

	enum class Language : uint8_t
	{
		English = 0,
		Spanish = 1,
		French = 2,
		Italian = 3,
		German = 4,
		Russian = 5,
		Swedish = 6,
		Japanese = 7,
		Chinese = 8,
		Portogese = 9
	};

	enum class Scale : uint8_t
	{
		R768x432 = 2,
		R1152x648 = 3,
		R1536x864 = 4
	};

	enum class Volume : uint8_t
	{
		V0 = 0,
		V10 = 1,
		V20 = 2,
		V30 = 3,
		V40 = 4,
		V50 = 5,
		V60 = 6,
		V70 = 7,
		V80 = 8,
		V90 = 9,
		V100 = 10
	};

	enum class BossNoDamage : uint8_t
	{
		None = 0,
		Forest = 1,
		Desert = 2,
		Castle = 4,
		All = 7
	};

	enum class Level : uint8_t
	{
		Beach = 1,
		Forest = 2,
		Desert = 4,
		Snow = 8,
		Moon = 16,
		Castle = 32
	};

	struct Noise
	{
		Volume volume = Volume::V100;
		Toggle toggle = Toggle::Enabled;
		bool operator==(const Noise& other)
		{
			return volume == other.volume
				&& toggle == other.toggle;
		}
	};

	struct Options
	{
		Language language = Language::English;
		Scale scale = Scale::R768x432;
		Toggle fullScreen = Toggle::Enabled;
		Toggle leftHanded = Toggle::Disabled;
		Noise sound;
		Noise music;
		bool operator==(const Options& other)
		{
			return language == other.language
				&& scale == other.scale
				&& fullScreen == other.fullScreen
				&& leftHanded == other.leftHanded
				&& sound == other.sound
				&& music == other.music;
		}
	};

	template<typename Base, size_t Min, size_t Max>
	struct ClampedNumber
	{
		Base value;
		ClampedNumber(Base number)
		{
			value = number;
		}
		operator Base()
		{
			return value;
		}
		ClampedNumber& operator=(size_t number)
		{
			if(number > Max) value = Max;
			else if(number < Min) value = Min;
			else value = number;
			return *this;
		}
		bool operator==(ClampedNumber other)
		{
			return value == other.value;
		}
	};
	using Enemies = ClampedNumber<uint32_t, 0, 16777216>;
	using Lives = ClampedNumber<uint32_t, 0, 16777216>;
	using Gems = ClampedNumber<uint8_t, 0, 99>;

	struct File
	{
		BossNoDamage noDamage = BossNoDamage::None;
		Enemies defeated = 0;
		Level progress = Level::Beach;
		Toggle ending = Toggle::Disabled;
		Toggle intro = Toggle::Disabled;
		Lives lives = 5;
		Gems gems = 0;
		bool operator==(const File& other)
		{
			return noDamage == other.noDamage
				&& defeated == other.defeated
				&& progress == other.progress
				&& ending == other.ending
				&& intro == other.intro
				&& lives == other.lives
				&& gems == other.gems;
		}
	};

	enum class FileIndex
	{
		File1 = 0,
		File2 = 1,
		File3 = 2
	};

	struct Save
	{
		Options options;
		File file1;
		File file2;
		File file3;
		bool operator==(const Save& other)
		{
			return options == other.options
				&& file1 == other.file1
				&& file2 == other.file2
				&& file3 == other.file3;
		}
	};

	struct Settings
	{
		Toggle cheats = Toggle::Disabled;
		bool operator==(const Settings& other)
		{
			return cheats == other.cheats;
		}
	};

	struct Error
	{
		enum class Where
		{
			Nowhere,
			Unknown,
			Save,
			Settings,
			Options,
			File,
			Data,
			GUI
		} where = Where::Unknown;
		enum class What
		{
			Nothing,
			Unknown,
			IO,
			Syntax,
			Value
		} what = What::Unknown;
		operator bool() const
		{
			return where != Where::Nowhere || what != What::Nothing;
		}
		bool operator==(const Error& other)
		{
			return where == other.where
				&& what == other.what;
		}
	};

	template<typename Data>
	struct Return
	{
		Error error;
		Data data;
	};

	struct Return<void>
	{
		Error error;
	};

	struct Return<Save>
	{
		Error error;
		Save save;
	};

	struct Return<Settings>
	{
		Error error;
		Settings settings;
	};

	struct Return<std::filesystem::path>
	{
		Error error;
		std::filesystem::path path;
	};

	struct Return<File>
	{
		Error error;
		File file;
	};

	struct Return<Options>
	{
		Error error;
		Options options;
	};
} // namespace Types
} // namespace WhipseeySaveManager
