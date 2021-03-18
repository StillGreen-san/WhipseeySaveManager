#include "gui-impl-common.hpp"
#include "gui-impl-files.hpp"
#include "gui-impl-options.hpp"
#include "gui-impl-cheats.hpp"

#include <nana/gui/widgets/tabbar.hpp>

//TODO delegate reset&max to INI types
//TODO connect vs connect_front

namespace WhipseeySaveManager::GUI
{
	Types::Error GUI::run()
	{
		if(	   !callbacks.onDefaultSavePath
			|| !callbacks.onDefaultSettingsPath
			|| !callbacks.onReadIni
			|| !callbacks.onReadSection
			|| !callbacks.onSystemTheme
			|| !callbacks.onWriteIni
			|| !callbacks.onWriteSection
		)
		{
			return Types::Error::Code::MissingCallback;
		}
		
		nana::form mainForm(nana::api::make_center(615, 255));
		mainForm.caption("Whipseey Save Manager");

		//TODO check shared_ptr usage in gui impl (reference lifetime lamdas)
		auto save = std::make_shared<INI::Save>();
		auto settings = std::make_shared<INI::Settings>();

		TabFiles files(mainForm, save, callbacks);
		TabOptions options(mainForm, save, callbacks);
		TabCheats cheats(mainForm, settings, callbacks);

		nana::tabbar<size_t> tabs(mainForm);
		tabs.append("Files", files);
		tabs.append("Options", options);
		tabs.append("Cheats", cheats);
		tabs.activated(0);

		mainForm.div("vertical<tabbar weight=28><tabframe>");
		mainForm["tabbar"] << tabs;
		mainForm["tabframe"].fasten(files).fasten(options).fasten(cheats);
		mainForm.collocate();

		mainForm.show();
		nana::exec();

		return Types::Error::Code::Nothing;
	}

	void GUI::connectOnSystemTheme(std::function<GUI::ThemeSignature> func)
	{
		callbacks.onSystemTheme = func;
	}

	void GUI::connectOnDefaultSavePath(std::function<GUI::PathSignature> func)
	{
		callbacks.onDefaultSavePath = func;
	}

	void GUI::connectOnDefaultSettingsPath(std::function<GUI::PathSignature> func)
	{
		callbacks.onDefaultSettingsPath = func;
	}

	void GUI::connectOnReadSection(std::function<GUI::SectionSignature> func)
	{
		callbacks.onReadSection = func;
	}

	void GUI::connectOnReadIni(std::function<GUI::IniSignature> func)
	{
		callbacks.onReadIni = func;
	}

	void GUI::connectOnWriteSection(std::function<GUI::SectionSignature> func)
	{
		callbacks.onWriteSection = func;
	}

	void GUI::connectOnWriteIni(std::function<GUI::IniSignature> func)
	{
		callbacks.onWriteIni = func;
	}
}
