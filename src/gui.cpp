#include "gui.hpp"

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/label.hpp>

namespace WhipseeySaveManager::GUI
{
	class PathControls : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		nana::textbox filePath{*this};
		nana::button openFile{*this, "..."};
		nana::button saveFile{*this, "save"};
		nana::button reloadFile{*this, "reload"};
		PathControls(nana::window wd) : nana::panel<false>(wd)
		{
			place.div("<path><buttons>");
			place["path"] << filePath;
			place["buttons"] << openFile << saveFile << reloadFile;
		}
	};
	class FileBox : public nana::panel<false>
	{
	public:
		nana::group group{*this};
		nana::group progress{group, "fileX"};
		nana::checkbox intro{group, "Intro"};
		nana::checkbox ending{group, "Ending"};
		nana::label lgems{group, "Gems"};
		nana::label llives{group, "Lives"};
		nana::textbox tgems{group, "99"};
		nana::textbox tlives{group, "9999"};
		nana::button bgems{group, "cycle"};
		nana::button blives{group, "cycle"};
		nana::button max{group, "max"};
		nana::button remove{group, "delete"};
		nana::button save{group, "save"};
		nana::button reload{group, "reload"};
		FileBox(nana::window wd) : nana::panel<false>(wd)
		{
			group.div("<progress><gems><lives>");
			group["progress"] << progress;
			group["gems"] << intro << lgems << tgems << bgems << max << save;
			group["lives"] << ending << llives << tlives << blives << remove << reload;
			group.collocate();
		}//! broken
	};
	
	Types::Error GUI::run()
	{
		nana::form mainForm(nana::api::make_center(625, 255));
		mainForm.caption("Whipseey Save Manager");

		PathControls pcsave(mainForm);
		pcsave.openFile.events().click.connect([&](nana::arg_click){
			nana::filebox fb(mainForm, true);
			fb.show();
		});
		PathControls pcset(mainForm);

		FileBox fb1(mainForm);
		FileBox fb2(mainForm);
		FileBox fb3(mainForm);

		mainForm.div("vertical<vertical ron><don>");
		mainForm["ron"] << pcsave << pcset;
		mainForm["don"] << fb1 << fb2 << fb3;
		mainForm.collocate();

		mainForm.show();
		nana::exec();

		return {};
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
