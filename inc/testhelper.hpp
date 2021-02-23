#pragma once

#include <filesystem>

#include "types.hpp"

namespace WhipseeySaveManager
{
namespace Test
{
	template<typename ENUM>
	constexpr ENUM maxEnumVal() noexcept
	{
		return static_cast<ENUM>(std::numeric_limits<std::underlying_type_t<ENUM>>::max());
	}

	template<typename TYPE>
	constexpr TYPE maxTypeVal(const TYPE&)
	{
		return std::numeric_limits<TYPE>::max();
	}

	Types::Options makeInvalidOptions();

	Types::File makeInvalidFile();

	Types::Save makeInvalidSave();

	Types::Options makeValidOptions();

	Types::File makeValidFile();

	Types::Save makeValidSave();

	Types::Settings makeInvalidSettings();

	Types::Settings makeValidSettings();
} // namespace Test
} // namespace WhipseeySaveManager
