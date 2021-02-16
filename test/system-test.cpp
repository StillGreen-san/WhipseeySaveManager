#include "system.hpp"

#include <catch.hpp>
#include <sstream>
#include <ios>

using namespace WhipseeySaveManager;

TEST_CASE("System::systemTheme", "[.][System]")
{
	Types::ErrDat<Types::Theme> ret = System::systemTheme();
	if(ret.error)
	{
		CHECK((ret.error.code == Types::Error::Code::ColorDwordNotFound
			|| ret.error.code == Types::Error::Code::ColorKeyNotFound
			|| ret.error.code == Types::Error::Code::ThemeDwordNotFound
			|| ret.error.code == Types::Error::Code::ThemeKeyNotFound));
		FAIL("theme not retrieved");
	}
	else
	{
		SUCCEED("theme retrieved");
	}
	std::stringstream msg;
	msg << "Theme status must be check manually!"
		<< "\nDarkmode: " << (ret.theme.darkmode == Types::Toggle::Enabled ? "Enabled" : "Disabled")
		<< "\nAccent  : 0x" << std::hex << ret.theme.accent;
	WARN(msg.str());
}
