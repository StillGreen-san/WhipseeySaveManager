#pragma once

#include <filesystem>
#include <set>

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
	enum class Fullscreen : uint8_t
	{
		Disabled = 0,
		Enabled = 1
	};
	enum class LeftHanded : uint8_t
	{
		Disabled = 0,
		Enabled = 1
	};
	enum class SoundToggle : uint8_t
	{
		Disabled = 0,
		Enabled = 1
	};
	enum class MusicToggle : uint8_t
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
		bool operator==(const Noise& other) const
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
		bool operator==(const Options& other) const
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
	 * @brief enemies, killed holds positive numbers <= 16777216
	 * 
	 */
	using Enemies = uint32_t;
	/**
	 * @brief lives, holds positive numbers <= 16777216
	 * 
	 */
	using Lives = uint32_t;
	/**
	 * @brief gems, holds positive numbers <= 99
	 * 
	 */
	using Gems = uint8_t;

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
		bool operator==(const File& other) const
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
	enum class FileIndex : size_t
	{
		File1 = 2,
		File2 = 1,
		File3 = 0
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
		bool operator==(const Save& other) const
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
		bool operator==(const Settings& other) const
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
	 * @brief may contain on or more error codes, converts to true if not empty
	 * 
	 */
	class Error
	{
	public:
		/**
		 * @brief the type of error
		 * 
		 */
		enum class Code
		{
			Nothing,// no error occurred
			Unknown,// an unknown error occurred
			ThemeDwordNotFound,
			ThemeKeyNotFound,
			ColorDwordNotFound,
			ColorKeyNotFound,
			DefaultSaveNotFound,
			SteamKeyNotFound,// an error occurred while opening the steam key in the registry
			SteamDwordNotFound,
			SteamLibrariesNotFound,
			GameNotFound,
			FailedToLoadSettings,
			CheatsSectionNotFound,
			CheatsKeyNotFound,
			CheatsKeyInvalid,
			FailedToLoadOptions,
			OptionsSectionNotFound,
			ScaleKeyNotFound,
			LanguageKeyNotFound,
			FullscreenKeyNotFound,
			LefthandedKeyNotFound,
			SoundvolumeKeyNotFound,
			SoundtoggleKeyNotFound,
			MusicvolumeKeyNotFound,
			MusictoggleKeyNotFound,
			FailedToLoadFile,
			FileSectionNotFound,
			NodamageKeyNotFound,
			DefeatedKeyNotFound,
			CastleKeyNotFound,
			MoonKeyNotFound,
			SnowKeyNotFound,
			DesertKeyNotFound,
			ForestKeyNotFound,
			EndingKeyNotFound,
			IntroKeyNotFound,
			LivesKeyNotFound,
			GemsKeyNotFound,

			InvalidFormat,
			InvalidValue
		};//TODO add comments for error codes

	private:
		std::set<Code> codes;//TODO use vector instead to keep code order

	public:
		Error() = default;
		Error(Code code) : codes{code} { }

		/**
		 * @brief conversion to bool
		 * 
		 * @return true if an error occurred, false if == Nowhere && Nothing
		 */
		operator bool() const
		{
			return codes.empty() == false;
		}

		/**
		 * @brief resets codes to only the one passed in
		 * 
		 * @param code the code to assign
		 * @return Error& reference to this object
		 */
		Error& operator=(const Error::Code& code)
		{
			codes.clear();
			codes.emplace(code);
			return *this;
		}

		/**
		 * @brief adds a code
		 * 
		 * @param code the code to add
		 * @return Error& reference to this object
		 */
		Error& operator+=(const Error::Code& code)
		{
			codes.emplace(code);
			return *this;
		}

		/**
		 * @brief checks if both contain the same errors (order does not matter)
		 * 
		 * @param other the error class to compare to
		 * @return true if both contain the same errors (order does not matter)
		 */
		bool operator==(const Error& other) const
		{
			if(codes.size() != other.codes.size()) return false;
			for(Code code : codes)
			{
				bool found = false;
				for(Code otherCode : other.codes)
				{
					if(code == otherCode)
					{
						found = true;
						break;
					}
				}
				if(found == false) return false;
			}
			return true;
		}

		/**
		 * @brief checks if this only conatins the code passed in
		 * 
		 * @param code the code to check
		 * @return true if this only conatins the code passed in
		 */
		bool operator==(const Error::Code& code) const
		{
			if(code == Code::Nothing) return codes.empty();
			if(codes.size() > 1) return false;
			return codes.find(code) != codes.cend();
		}
	};//TODO tests & more interface?

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
		operator bool() const
		{
			return error == false;
		}

		ErrDat& operator=(const Error& newError)
		{
			error = newError;
			return *this;
		}

		ErrDat& operator+=(const Error::Code& additionalCode)
		{
			error += additionalCode;
			return *this;
		}

		ErrDat& operator=(const Data& newData)
		{
			data = newData;
			return *this;
		}

		bool operator==(const Error& otherError) const
		{
			return error == otherError;
		}

		bool operator==(const Data& otherData) const
		{
			return data == otherData;
		}
	};//TODO more helpers?
} // namespace Types
} // namespace WhipseeySaveManager
