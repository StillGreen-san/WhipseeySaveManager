#include "error.hpp"

namespace WhipseeySaveManager
{
	namespace Error
	{
		std::string Error::toString() const
		{
			return toString(code).append(": ").append(toString(category));
		}
		
		std::string Error::toString(Code code) 
		{
			switch (code)
			{
			case Code::Unknown :
			default:
				return "An unknown code, this should not have happend!"
					" Please open an issue on GitHub to let the dev know.";
			}
		}
		
		std::string Error::toString(Category category) 
		{
			switch (category)
			{
			case Category::Unknown :
			default:
				return "An unknown category, this should not have happend!"
					" Please open an issue on GitHub to let the dev know.";
			}
		}
		
		Error makeError(Code code, Category category) 
		{
			return {code, category};
		}
	}
}
