#pragma once

#include <filesystem>

#include "data.hpp"

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

	Data::Options makeInvalidOptions();

	Data::File makeInvalidFile();

	Data::Save makeInvalidSave();

	Data::Options makeValidOptions();

	Data::File makeValidFile();

	Data::Save makeValidSave();

	Data::Settings makeInvalidSettings();

	Data::Settings makeValidSettings();

	bool prepareTetstdata();
} // namespace Test
} // namespace WhipseeySaveManager
