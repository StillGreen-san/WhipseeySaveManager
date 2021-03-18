#pragma once

#include "core.hpp"

#include <filesystem>
#include <optional>

namespace WhipseeySaveManager::Types
{
	/**
	 * @brief stringly typed toggle associated with there savegame values
	 * 
	 */
	enum class Fullscreen : uint8_t
	{
		Disabled = 0,
		Enabled = 1
	};

	/**
	 * @brief stringly typed toggle associated with there savegame values
	 * 
	 */
	enum class LeftHanded : uint8_t
	{
		Disabled = 0,
		Enabled = 1
	};

	/**
	 * @brief stringly typed toggle associated with there savegame values
	 * 
	 */
	enum class SoundToggle : uint8_t
	{
		Disabled = 0,
		Enabled = 1
	};

	/**
	 * @brief stringly typed toggle associated with there savegame values
	 * 
	 */
	enum class MusicToggle : uint8_t
	{
		Disabled = 0,
		Enabled = 1
	};

	/**
	 * @brief stringly typed toggle associated with there savegame values
	 * 
	 */
	enum class Ending : uint8_t
	{
		Unwatched = 0,
		Watched = 1
	};

	/**
	 * @brief stringly typed toggle associated with there savegame values
	 * 
	 */
	enum class Intro : uint8_t
	{
		Unwatched = 0,
		Watched = 1
	};

	/**
	 * @brief stringly typed toggle associated with there savegame values
	 * 
	 */
	enum class CheatsEnabled : uint8_t
	{
		Disabled = 0,
		Enabled = 1
	};

	/**
	 * @brief stringly typed toggle associated with there savegame values
	 * 
	 */
	enum class Darkmode : uint8_t
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
	enum class SoundVolume : uint8_t
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
	 * @brief selectable volume steps roughly associated with there savegame values (div by 10.f to get real)
	 * 
	 */
	enum class MusicVolume : uint8_t
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
	};//TODO something

	/**
	 * @brief individual level progress associated with there savegame values
	 * 
	 */
	enum class Castle : uint8_t
	{
		Remaining = 0,
		Cleared = 32
	};//TODO change to un/locked
	
	/**
	 * @brief individual level progress associated with there savegame values
	 * 
	 */
	enum class Moon : uint8_t
	{
		Remaining = 0,
		Cleared = 16
	};
	
	/**
	 * @brief individual level progress associated with there savegame values
	 * 
	 */
	enum class Snow : uint8_t
	{
		Remaining = 0,
		Cleared = 8
	};
	
	/**
	 * @brief individual level progress associated with there savegame values
	 * 
	 */
	enum class Desert : uint8_t
	{
		Remaining = 0,
		Cleared = 4
	};
	
	/**
	 * @brief individual level progress associated with there savegame values
	 * 
	 */
	enum class Forest : uint8_t
	{
		Remaining = 0,
		Cleared = 2
	};

	using Gems = uint8_t;
	using Lives = uint32_t;

	/**
	 * @brief contains basic info on a color theme (accent = rgb)
	 * 
	 */
	struct Theme
	{
		Darkmode darkmode = Darkmode::Disabled;
		uint32_t accent = 0x0078d7;
	};

	/**
	 * @brief may contain on or more error codes, converts to true if not empty
	 * 
	 */
	class Error
	{//TODO tests
	public:
		/**
		 * @brief the type of error
		 * 
		 */
		enum class Code
		{
			Nothing,// no error occurred
			Unknown,// an unknown error occurred
			InvalidFormat,// the value of the key was in the wrong format, default value used
			InvalidValue,// the value read was not valid for this key, default value used
			SectionNotFound,// the section was not found in the file, default values used
			KeyNotFound,// the key was not found in the file, default value used
			FailedToLoadFile,// the file could not be read, check existence and permissions
			FailedToWriteFile,// the file could not be written, check existence or permissions
			MissingCallback// one or more gui callback where not specified, gui not initialized
		};

	private:
		std::vector<Code> codes;

	public:
		Error() = default;
		Error(Code code) : codes{code}
		{
			if(codes.front() == Types::Error::Code::Nothing)
			{
				codes.clear();
			}
		}

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
			if(code == Code::Nothing)
			{
				codes.clear();
				return *this;
			}
			codes.assign(1, code);
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
			if(code == Code::Nothing) return *this;
			codes.emplace_back(code);
			return *this;
		}

		/**
		 * @brief appends all codes contained in the other error
		 * 
		 * @param error the Error object to copy codes from
		 * @return Error& *this
		 */
		Error& operator+=(const Error& error)
		{
			codes.insert(codes.end(), error.codes.cbegin(), error.codes.cend());
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
			return codes[0] == code;
		}
	};
} // namespace WhipseeySaveManager::Types
