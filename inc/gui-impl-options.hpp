#pragma once

#include "gui-impl-common.hpp"

#include <nana/gui/widgets/combox.hpp>

namespace WhipseeySaveManager::GUI
{
	class OptionBase : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		nana::label label;
		nana::combox combo{*this};
		OptionBase(nana::window wd, std::string_view labelText);
		static constexpr char* FIELD = "things";
		nana::basic_event<nana::arg_combox>& onChanged();
	};

	class OptionLanguage : public OptionBase
	{
	public:
		OptionLanguage(nana::window wd);
		void update(INI::Options& options);
	};

	class OptionScale : public OptionBase
	{
	public:
		OptionScale(nana::window wd);
		void update(INI::Options& options);
	};

	class OptionFullscreen : public OptionBase
	{
	public:
		OptionFullscreen(nana::window wd);
		void update(INI::Options& options);
	};

	class OptionLeftHanded : public OptionBase
	{
	public:
		OptionLeftHanded(nana::window wd);
		void update(INI::Options& options);
	};

	class OptionSoundVolume : public OptionBase
	{
	public:
		OptionSoundVolume(nana::window wd);
		void update(INI::Options& options);
	};

	class OptionSoundToggle : public OptionBase
	{
	public:
		OptionSoundToggle(nana::window wd);
		void update(INI::Options& options);
	};

	class OptionMusicVolume : public OptionBase
	{
	public:
		OptionMusicVolume(nana::window wd);
		void update(INI::Options& options);
	};

	class OptionMusicToggle : public OptionBase
	{
	public:
		OptionMusicToggle(nana::window wd);
		void update(INI::Options& options);
	};

	class OptionsBox : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		OptionLanguage language{*this};
		OptionScale scale{*this};
		OptionFullscreen fullscreen{*this};
		OptionLeftHanded lefthanded{*this};
		OptionSoundVolume soundvolume{*this};
		OptionSoundToggle soundtoggle{*this};
		OptionMusicVolume musicvolume{*this};
		OptionMusicToggle musictoggle{*this};

		OptionsBox(nana::window wd);

		void update(INI::Options& options);
		nana::basic_event<nana::arg_combox>& onLanguageChanged();
		nana::basic_event<nana::arg_combox>& onScaleChanged();
		nana::basic_event<nana::arg_combox>& onFullscreenChanged();
		nana::basic_event<nana::arg_combox>& onLeftHandedChanged();
		nana::basic_event<nana::arg_combox>& onSoundVolumeChanged();
		nana::basic_event<nana::arg_combox>& onSoundToggleChanged();
		nana::basic_event<nana::arg_combox>& onMusicVolumeChanged();
		nana::basic_event<nana::arg_combox>& onMusicToggleChanged();
	};

	class TabOptions : public nana::panel<false>
	{
	public:
		nana::place place{*this};
		PathControls path{*this, {{"Save (*.sav)", "*.sav"}}};
		OptionsBox options{*this};
		TabOptions(nana::window wd, const std::shared_ptr<INI::Save>& save, const GUI::FunctionStore& callbacks);
		nana::basic_event<nana::arg_click>& onReload();
		nana::basic_event<nana::arg_click>& onSave();
	};
}
