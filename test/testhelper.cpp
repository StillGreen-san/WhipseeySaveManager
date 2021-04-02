#include "testhelper.hpp"

#include <string>

namespace WhipseeySaveManager::Test
{
std::string stringify(float value)
{
	return std::string("\"").append(std::to_string(value)).append("\"");
}
} // namespace WhipseeySaveManager::Test
