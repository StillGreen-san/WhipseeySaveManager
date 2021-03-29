#include "gui-impl-cheats.hpp"

namespace WhipseeySaveManager::GUI
{
	CheatsBox::CheatsBox(nana::window wd) : nana::panel<false>(wd)
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

	TabCheats::TabCheats(nana::window wd, const std::shared_ptr<INI::Settings>& sttngs, const GUI::FunctionStore& callbacks) :
		nana::panel<false>(wd)
	{
		place.div("vert <path gap=5 margin=5 weight=35><cheats margin=5>");
		place["path"] << path;
		place["cheats"] << cheats;
		cheats.onEnabledChanged().connect_front([&](nana::arg_checkbox cb){
			sttngs->getCheats().getCheatsEnabled() = static_cast<Types::CheatsEnabled>(cb.widget->checked());
		});
		onReload().connect_front([&](nana::arg_click){
			showErrorMsg(callbacks.onReadIni(sttngs, path.getPath()));
			cheats.update(sttngs->getCheats());
		});
		onSave().connect_front([&](nana::arg_click){
			showErrorMsg(callbacks.onWriteIni(sttngs, path.getPath()));
		});
		std::optional<std::filesystem::path> sttngsPath = callbacks.onDefaultSettingsPath();
		if(sttngsPath)
		{
			path.setPath(*sttngsPath);
		}
	}

	nana::basic_event<nana::arg_click>& TabCheats::onReload()
	{
		return path.reloadFile.events().click;
	}

	nana::basic_event<nana::arg_click>& TabCheats::onSave()
	{
		return path.saveFile.events().click;
	}
}
