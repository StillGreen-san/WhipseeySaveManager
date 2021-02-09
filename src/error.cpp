#include "error.hpp"

namespace WhipseeySaveManager
{
namespace Error
{
	Error::operator bool() const
	{
		return where != Where::Nowhere || what != What::Nothing;
	}

	Error makeError(Where where, What what)
	{
		return {where, what};
	}
}
}
