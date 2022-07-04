#pragma once

#include "core.hpp"

#include <filesystem>
#include <map>
#include <optional>

namespace WhipseeySaveManager::Types
{
/**
 * @brief strongly typed toggle associated with their savegame values
 *
 */
enum class Fullscreen : uint8_t
{
	Disabled = 0,
	Enabled = 1
};

/**
 * @brief strongly typed toggle associated with their savegame values
 *
 */
enum class LeftHanded : uint8_t
{
	Disabled = 0,
	Enabled = 1
};

/**
 * @brief strongly typed toggle associated with their savegame values
 *
 */
enum class SoundToggle : uint8_t
{
	Disabled = 0,
	Enabled = 1
};

/**
 * @brief strongly typed toggle associated with their savegame values
 *
 */
enum class MusicToggle : uint8_t
{
	Disabled = 0,
	Enabled = 1
};

/**
 * @brief strongly typed toggle associated with their savegame values
 *
 */
enum class Ending : uint8_t
{
	Unwatched = 0,
	Watched = 1
};

/**
 * @brief strongly typed toggle associated with their savegame values
 *
 */
enum class Intro : uint8_t
{
	Unwatched = 0,
	Watched = 1
};

/**
 * @brief strongly typed toggle associated with their savegame values
 *
 */
enum class CheatsEnabled : uint8_t
{
	Disabled = 0,
	Enabled = 1
};

/**
 * @brief strongly typed toggle associated with their savegame values
 *
 */
enum class Darkmode : uint8_t
{
	Disabled = 0,
	Enabled = 1
};

/**
 * @brief selectable languages associated with their savegame values
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
 * @brief resolutions in windowed mode associated with their savegame values
 *
 */
enum class Scale : uint8_t
{
	R768x432 = 2,
	R1152x648 = 3,
	R1536x864 = 4
};

/**
 * @brief selectable volume steps roughly associated with their savegame values (div by 10.f to get real)
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
 * @brief selectable volume steps roughly associated with their savegame values (div by 10.f to get real)
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
 * @brief boss no damage progress bitfield associated with their savegame values
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
 * @brief individual level progress associated with their savegame values
 *
 */
enum class Castle : uint8_t
{
	Locked = 0,
	Unlocked = 32
};

/**
 * @brief individual level progress associated with their savegame values
 *
 */
enum class Moon : uint8_t
{
	Locked = 0,
	Unlocked = 16
};

/**
 * @brief individual level progress associated with their savegame values
 *
 */
enum class Snow : uint8_t
{
	Locked = 0,
	Unlocked = 8
};

/**
 * @brief individual level progress associated with their savegame values
 *
 */
enum class Desert : uint8_t
{
	Locked = 0,
	Unlocked = 4
};

/**
 * @brief individual level progress associated with their savegame values
 *
 */
enum class Forest : uint8_t
{
	Locked = 0,
	Unlocked = 2
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
	uint32_t accent = 0x0078d7; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
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
	enum class Code : unsigned
	{                           //! do not change code values, do not use 2147483648 as value
		Nothing = 0,            // no error occurred
		InvalidFormat = 1,      // the value of the key was in the wrong format, default value used
		InvalidValue = 2,       // the value read was not valid for this key, default value used
		SectionNotFound = 4,    // the section was not found in the file, default values used
		KeyNotFound = 8,        // the key was not found in the file, default value used
		FailedToLoadFile = 16,  // the file could not be read, check existence and permissions
		FailedToWriteFile = 32, // the file could not be written, check permissions
		MissingCallback = 64,   // one or more gui callback where not specified, gui not initialized
		Unknown = 1073741824    // an unknown error occurred
	};                          // TODO change Error to use Code as bitfield instead of vector?

	using CodeContainer = std::vector<Code>;

private:
	CodeContainer codes;

public:
	Error() = default;
	Error(Code code) : codes{1, code} // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
	{
		if(codes.front() == Code::Nothing)
		{
			codes.clear();
		}
	}
	Error(std::initializer_list<Code> codesList) : codes{codesList}
	{
		codes.erase(std::remove(codes.begin(), codes.end(), Code::Nothing), codes.end());
	}

	/**
	 * @brief conversion to bool
	 *
	 * @return true if an error occurred
	 */
	[[nodiscard]] explicit operator bool() const
	{
		return !codes.empty();
	}

	[[nodiscard]] int getReturnCode() const
	{
		unsigned combined = 0;
		for(Code code : codes)
		{
			combined |= static_cast<unsigned>(code);
		}
		return static_cast<int>(combined);
	}

	/**
	 * @brief resets codes to only the one passed in
	 *
	 * @param code the code to assign
	 * @return Error& reference to this object
	 */
	Error& operator=(const Code& code)
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
	Error& operator+=(const Code& code)
	{
		if(code == Code::Nothing)
		{
			return *this;
		}
		codes.push_back(code);
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
	[[nodiscard]] bool operator==(const Error& other) const
	{
		if(codes.size() != other.codes.size())
		{
			return false;
		}
		std::map<Code, unsigned> errCntThis;
		std::map<Code, unsigned> errCntThat;
		for(Code code : codes)
		{
			++errCntThis[code];
		}
		for(Code code : other.codes)
		{
			++errCntThat[code];
		}
		return errCntThis == errCntThat;
	}

	/**
	 * @brief checks if this only contains the code passed in
	 *
	 * @param code the code to check
	 * @return true if this only contains the code passed in
	 */
	[[nodiscard]] bool operator==(const Code& code) const
	{
		if(code == Code::Nothing)
		{
			return codes.empty();
		}
		if(codes.size() > 1)
		{
			return false;
		}
		return codes[0] == code;
	}

	/**
	 * @brief return an const iterator to the begin of the internal code container
	 *
	 */
	[[nodiscard]] CodeContainer::const_iterator begin() const
	{
		return codes.cbegin();
	}

	/**
	 * @brief return an const iterator to the end of the internal code container
	 *
	 */
	[[nodiscard]] CodeContainer::const_iterator end() const
	{
		return codes.cend();
	}
};
} // namespace WhipseeySaveManager::Types
