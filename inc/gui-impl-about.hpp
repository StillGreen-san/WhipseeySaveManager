#pragma once

#include "gui-impl-common.hpp"

namespace WhipseeySaveManager::GUI
{
/**
 * @brief tab control with licence info
 *
 */
class TabAbout : public nana::panel<false>
{
	nana::place place{*this};
	nana::label info{
	    *this, "<bold font=\"Consolas\">this program uses the following libraries:"
	           "\n"
	           "\n   WinReg [MIT] (<color=0x1111dd url=\"https://github.com/GiovanniDicanio/WinReg\">https://github.com/GiovanniDicanio/WinReg</>)"
	           "\nsimpleini [MIT] (<color=0x1111dd url=\"https://github.com/brofield/simpleini\">https://github.com/brofield/simpleini</>)    "
	           "\n     nana [BSL] (<color=0x1111dd url=\"https://github.com/cnjinhao/nana\">https://github.com/cnjinhao/nana</>)         "
	           "\n"
	           "\nfor testing only:"
	           "\n   Catch2 [BSL] (<color=0x1111dd url=\"https://github.com/catchorg/Catch2\">https://github.com/catchorg/Catch2</>)       "
	           "\n</>"};

public:
	TabAbout(nana::window wd) : nana::panel<false>(wd)
	{
		info.format(true);
		info.text_align(nana::align::center, nana::align_v::center);

		place.div("vert <about margin=5>");
		place["about"] << info;
	}
};
} // namespace WhipseeySaveManager::GUI
