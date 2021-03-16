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

	TabCheats::TabCheats(nana::window wd, std::shared_ptr<INI::Settings> sttngs,
		std::function<GUI::IniSignature> cbReadIni, std::function<GUI::IniSignature> cbWriteIni
	) :
		nana::panel<false>(wd),
		settings{std::move(sttngs)},
		onReadIni{std::move(cbReadIni)},
		onWriteIni{std::move(cbWriteIni)}
	{
		place.div("vert <path gap=5 margin=5 weight=35><cheats margin=5>");
		place["path"] << path;
		place["cheats"] << cheats;
		cheats.onEnabledChanged().connect_front([&](nana::arg_checkbox cb){
			settings->getCheats().getCheatsEnabled() = static_cast<Types::CheatsEnabled>(cb.widget->checked());
		});
		if(onReadIni)
		{
			onReload().connect_front([&](nana::arg_click){
				Types::Error error = onReadIni(settings, path.getPath());
				//TODO show error
				cheats.update(settings->getCheats());
			});
		}
		if(onWriteIni)
		{
			onSave().connect_front([&](nana::arg_click){
				Types::Error error = onWriteIni(settings, path.getPath());
				//TODO show error
			});
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
