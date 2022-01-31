#pragma once

#include "gui-impl-common.hpp"

#include <nana/gui/widgets/combox.hpp>

namespace WhipseeySaveManager::GUI
{
/**
 * @brief base control for all options with label & combobox
 *
 */
class OptionBase : public nana::panel<false>
{
protected:
	nana::place place{*this};
	nana::label label;
	nana::combox combo{*this};
	OptionBase(nana::window wd, std::string_view labelText);

	/**
	 * @brief returns event reference to combobox changed
	 *
	 * @return nana::basic_event<nana::arg_checkbox>& the event ref
	 */
	nana::basic_event<nana::arg_combox>& onChanged();

public:
	/**
	 * @brief return the index of the selected option of the combobox
	 *
	 * @return size_t index of selected option
	 */
	size_t option();
};

/**
 * @brief language option control
 *
 */
class OptionLanguage : public OptionBase
{
public:
	OptionLanguage(nana::window wd);
	void update(INI::Options& options);
};

/**
 * @brief scale option control
 *
 */
class OptionScale : public OptionBase
{
public:
	OptionScale(nana::window wd);
	void update(INI::Options& options);
};

/**
 * @brief fullscreen option control
 *
 */
class OptionFullscreen : public OptionBase
{
public:
	OptionFullscreen(nana::window wd);
	void update(INI::Options& options);
};

/**
 * @brief lefthanded option control
 *
 */
class OptionLeftHanded : public OptionBase
{
public:
	OptionLeftHanded(nana::window wd);
	void update(INI::Options& options);
};

/**
 * @brief soundvolume option control
 *
 */
class OptionSoundVolume : public OptionBase
{
public:
	OptionSoundVolume(nana::window wd);
	void update(INI::Options& options);
};

/**
 * @brief soundtoggle option control
 *
 */
class OptionSoundToggle : public OptionBase
{
public:
	OptionSoundToggle(nana::window wd);
	void update(INI::Options& options);
};

/**
 * @brief musicvolume option control
 *
 */
class OptionMusicVolume : public OptionBase
{
public:
	OptionMusicVolume(nana::window wd);
	void update(INI::Options& options);
};

/**
 * @brief musictoggle option control
 *
 */
class OptionMusicToggle : public OptionBase
{
public:
	OptionMusicToggle(nana::window wd);
	void update(INI::Options& options);
};

/**
 * @brief options control with controls for language, scale, fullscreen, lefthanded, soundvolume, soundtoggle,
 * musicvolume, musictoggle
 *
 */
class OptionsBox : public nana::panel<false>
{
	nana::place place{*this};
	OptionLanguage language{*this};
	OptionScale scale{*this};
	OptionFullscreen fullscreen{*this};
	OptionLeftHanded lefthanded{*this};
	OptionSoundVolume soundvolume{*this};
	OptionSoundToggle soundtoggle{*this};
	OptionMusicVolume musicvolume{*this};
	OptionMusicToggle musictoggle{*this};

	void update(INI::Options& options);
	void get(INI::Options& options);

public:
	OptionsBox(nana::window wd);

	friend class TabOptions;
};

/**
 * @brief tab control with PathControls & OptionsBox
 *
 */
class TabOptions : public nana::panel<false>
{
	nana::place place{*this};
	PathControls path{*this, {{"Save (*.sav)", "*.sav"}}};
	OptionsBox options{*this};

public:
	TabOptions(nana::window wd, const std::shared_ptr<INI::Save>& save, const GUI::FunctionStore& callbacks);
};
} // namespace WhipseeySaveManager::GUI
