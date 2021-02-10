#pragma once

#include <system_error>

namespace WhipseeySaveManager
{
namespace Error
{
	enum class Where
	{
		Nowhere,
		Unknown,
		Save,
		Settings,
		Options,
		File,
		Data
	};

	enum class What
	{
		Nothing,
		Unknown,
		IO,
		Syntax,
		Value
	};

	struct Error
	{
		Where where = Where::Unknown;
		What what = What::Unknown;
		operator bool() const;
	};

	Error makeError(Where where = Where::Nowhere, What what = What::Nothing);
} // namespace Error
} // namespace WhipseeySaveManager



