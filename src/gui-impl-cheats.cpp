#include "gui-impl-cheats.hpp"

namespace WhipseeySaveManager::GUI
{
CheatsBox::CheatsBox(nana::window wnd) : nana::panel<false>(wnd)
{
	place.div("vert things margin=[0, 50] arrange=[40,variable]");
	place["things"] << cheatsEnabled << description;
}

nana::basic_event<nana::arg_checkbox>& CheatsBox::onEnabledChanged()
{
	return cheatsEnabled.events().checked;
}

void CheatsBox::update(INI::Cheats& cheats)
{
	cheatsEnabled.check(cheats.getCheatsEnabled() == Types::CheatsEnabled::Enabled);
}

TabCheats::TabCheats(
    nana::window wnd, const std::shared_ptr<INI::Settings>& sttngs, const GUI::FunctionStore& callbacks) :
    nana::panel<false>(wnd)
{
	place.div("vert <path gap=5 margin=5 weight=35><cheats margin=5>");
	place["path"] << path;
	place["cheats"] << cheats;

	cheats.onEnabledChanged().connect_front(
	    [&](const nana::arg_checkbox& checkbox)
	    {
		    sttngs->getCheats().getCheatsEnabled() = static_cast<Types::CheatsEnabled>(checkbox.widget->checked());
	    });

	path.onReload().connect_front(
	    [&]([[maybe_unused]] const nana::arg_click& click)
	    {
		    showErrorMsg(callbacks.onReadIni(sttngs, path.getPath()));
		    cheats.update(sttngs->getCheats());
	    });

	path.onSave().connect_front(
	    [&]([[maybe_unused]] const nana::arg_click& click)
	    {
		    showErrorMsg(callbacks.onWriteIni(sttngs, path.getPath()));
	    });

	std::optional<std::filesystem::path> sttngsPath = callbacks.onDefaultSettingsPath();
	if(sttngsPath)
	{
		path.setPath(*sttngsPath);
	}
}
} // namespace WhipseeySaveManager::GUI
