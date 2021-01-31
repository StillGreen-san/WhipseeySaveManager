/**
 * @file WhipseeySaveManager.cpp
 * @brief this is the main file for WhispeeySaveManager, tying all functionality together
 */

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>

int WinMain()
{
	//Define a form.
	nana::form fm;

	//Define a label and display a text.
	nana::label lab{fm, "Hello, <bold blue size=16>Nana C++ Library</>"};
	lab.format(true);

	//Define a button and answer the click event.
	nana::button btn{fm, "Quit"};
	btn.events().click([&fm]{
		fm.close();
	});

	//Layout management
	fm.div("vert <><<><weight=80% text><>><><weight=24<><button><>><>");
	fm["text"]<<lab;
	fm["button"] << btn;
	fm.collocate();
	
	//Show the form
	fm.show();

	//Start to event loop process, it blocks until the form is closed.
	nana::exec();
}
