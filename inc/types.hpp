#pragma once

#include <filesystem>

/**
 * @brief General Project Namespace
 * 
 */
namespace WhipseeySaveManager
{
/**
 * @brief Contains all Types used in WhipseeySaveManager
 * 
 */
namespace Types
{
	/**
	 * @brief named boolean associated with there savegame values
	 * 
	 */
	enum class Toggle : uint8_t
	{
		Disabled = 0,
		Enabled = 1
	};

	/**
	 * @brief selectable languages associated with there savegame values
	 * 
	 */
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

	/**
	 * @brief resolutions in windowed mode associated with there savegame values
	 * 
	 */
	enum class Scale : uint8_t
	{
		R768x432 = 2,
		R1152x648 = 3,
		R1536x864 = 4
	};

	/**
	 * @brief selectable volume steps roughly associated with there savegame values (div by 10.f to get real)
	 * 
	 */
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

	/**
	 * @brief boss no damage progress bitfield associated with there savegame values
	 * 
	 */
	enum class BossNoDamage : uint8_t
	{
		None = 0,
		Forest = 1,
		Desert = 2,
		Castle = 4,
		All = 7
	};

	/**
	 * @brief level progress bitfield represents multiple savegame values
	 * 
	 */
	enum class Level : uint8_t
	{
		Beach = 1,
		Forest = 2,
		Desert = 4,
		Snow = 8,
		Moon = 16,
		Castle = 32
	};

	/**
	 * @brief wrapper struct for Volume and Toggle
	 * 
	 */
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

	/**
	 * @brief holds all values of the options section in the savegame
	 * 
	 */
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

	/**
	 * @brief a number of type Base automatically clamped between Min and Max
	 * 
	 * @tparam Base the type to store internally
	 * @tparam Min the minimum value to clamp to
	 * @tparam Max the maximum value to clamp to
	 */
	template<typename Base, Base Min, Base Max>
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
		ClampedNumber& operator=(Base number)
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
	/**
	 * @brief enemies, killed holds positive numbers <= 16777216
	 * 
	 */
	using Enemies = ClampedNumber<uint32_t, 0, 16777216>;
	/**
	 * @brief lives, holds positive numbers <= 16777216
	 * 
	 */
	using Lives = ClampedNumber<uint32_t, 0, 16777216>;
	/**
	 * @brief gems, holds positive numbers <= 99
	 * 
	 */
	using Gems = ClampedNumber<uint8_t, 0, 99>;

	/**
	 * @brief holds all values of a file section in the savegame
	 * 
	 */
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

	/**
	 * @brief identifies a file section in a savegame
	 * 
	 */
	enum class FileIndex
	{
		File1 = 0,
		File2 = 1,
		File3 = 2
	};

	/**
	 * @brief holds all sections of a savegame
	 * 
	 */
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

	/**
	 * @brief holds all sections of a settings file
	 * 
	 */
	struct Settings
	{
		Toggle cheats = Toggle::Disabled;
		bool operator==(const Settings& other)
		{
			return cheats == other.cheats;
		}
	};

	/**
	 * @brief contains basic info on a color theme (accent = rgb)
	 * 
	 */
	struct Theme
	{
		Toggle darkmode = Toggle::Disabled;
		uint32_t accent = 0x0078d7;
	};

	/**
	 * @brief contains Where & What info of an error, converts to true if an error occurred
	 * 
	 */
	struct Error
	{
		/**
		 * @brief the type of error
		 * 
		 */
		enum class Code
		{
			Nothing,
			Unknown,
			ThemeDwordNotFound,
			ThemeKeyNotFound,
			ColorDwordNotFound,
			ColorKeyNotFound,
			DefaultSaveNotFound,
			SteamKeyNotFound,
			SteamDwordNotFound,
			SteamLibrariesNotFound,
			GameNotFound
		} code = Code::Nothing;
		/**
		 * @brief conversion to bool
		 * 
		 * @return true if an error occurred, false if == Nowhere && Nothing
		 */
		operator bool() const
		{
			return code != Code::Nothing;
		}
		bool operator==(const Error& other)
		{
			return code == other.code;
		}
	};

	/**
	 * @brief holds Error and Data
	 * 
	 * @tparam Data the second type
	 */
	template<typename Data>
	struct ErrDat
	{
		Error error;
		Data data;
		/**
		 * @brief checks the error status
		 * 
		 * @return true if error == false
		 */
		operator bool()
		{
			return error == false;
		}
	};
} // namespace Types
} // namespace WhipseeySaveManager
