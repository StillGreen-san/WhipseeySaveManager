#include "system.hpp"

#include <catch.hpp>
#include <sstream>

using namespace WhipseeySaveManager;

TEST_CASE("System::darkmodeEnabled", "[.][System]")
{
	Types::ErrDat<Types::Toggle> ret = System::darkmodeEnabled();
	if(ret.error)
	{
		CHECK(ret.error.where == Types::Error::Where::System);
		CHECK(ret.error.what == Types::Error::What::Registry);
	}
	else
	{
		CHECK(ret.error);
	}
	std::stringstream ss;
	ss << "Enabled status must be check manually!\n" 
		<< "Enabled: " << std::boolalpha << static_cast<bool>(ret.toggle);
	WARN(ss.str());
}
