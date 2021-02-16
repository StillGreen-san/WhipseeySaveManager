#include "system.hpp"

#include <catch.hpp>

using namespace WhipseeySaveManager;

// TEST_CASE("System::darkmodeEnabled", "[.][System]")
// {
// 	Types::ErrDat<Types::Toggle> ret = System::darkmodeEnabled();
// 	if(ret.error)
// 	{
// 		CHECK(ret.error.where == Types::Error::Where::System);
// 		CHECK(ret.error.what == Types::Error::What::Registry);
// 	}
// 	else
// 	{
// 		CHECK_FALSE(ret.error);
// 	}
// 	std::string msg = "Enabled status must be check manually!\n" "Enabled: ";
// 	msg.append(static_cast<bool>(ret.toggle) ? "true" : "false");
// 	WARN(msg);
// }
