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

	Error makeError(Where where = Where::Nowhere, What what = What::Nothing)
	{
		return {where, what};
	}
} // namespace Error
} // namespace WhipseeySaveManager



