#pragma once

#include <system_error>

namespace WhipseeySaveManager
{
namespace Error
{
	enum class Code
	{
		Unknown
	};

	enum class Category
	{
		Unknown
	};

	struct Error
	{
		Code code = Code::Unknown;
		Category category = Category::Unknown;
		std::string toString() const;
		static std::string toString(Code code);
		static std::string toString(Category category);
	};

	Error makeError(Code code, Category category);
} // namespace Error
} // namespace WhipseeySaveManager



