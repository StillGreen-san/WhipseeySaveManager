#include "gui-impl-options.hpp"

namespace WhipseeySaveManager::GUI
{
	OptionBase::OptionBase(nana::window wd, std::string_view labelText) :
		nana::panel<false>(wd),
		label{*this, labelText}
	{
		label.text_align(nana::align::right, nana::align_v::center);
		place.div("things gap=8 arrange=[190,300]");
		place[FIELD] << label << combo;
	}

	nana::basic_event<nana::arg_combox>& OptionBase::onChanged()
	{
		return combo.events().text_changed;
	}

	OptionLanguage::OptionLanguage(nana::window wd) : OptionBase(wd, "Language")
	{
		combo
			.push_back("English")
			.push_back("Spanish")
			.push_back("French")
			.push_back("Italian")
			.push_back("German")
			.push_back("Russian")
			.push_back("Swedish")
			.push_back("Japanese")
			.push_back("Chinese")
			.push_back("Portogese");
	}

	void OptionLanguage::update(INI::Options& options)
	{
		combo.option(options.getLanguage().asInt());
	}

	OptionScale::OptionScale(nana::window wd) : OptionBase(wd, "Scale")
	{
		combo
			.push_back("R768x432")
			.push_back("R1152x648")
			.push_back("R1536x864");
	}

	void OptionScale::update(INI::Options& options)
	{
		combo.option(options.getScale().asInt() - 2);
	}

	OptionFullscreen::OptionFullscreen(nana::window wd) : OptionBase(wd, "Fullscreen")
	{
		combo
			.push_back("Disabled")
			.push_back("Enabled");
	}

	void OptionFullscreen::update(INI::Options& options)
	{
		combo.option(options.getFullscreen().asInt());
	}

	OptionLeftHanded::OptionLeftHanded(nana::window wd) : OptionBase(wd, "LeftHanded")
	{
		combo
			.push_back("Disabled")
			.push_back("Enabled");
	}

	void OptionLeftHanded::update(INI::Options& options)
	{
		combo.option(options.getLeftHanded().asInt());
	}

	OptionSoundVolume::OptionSoundVolume(nana::window wd) : OptionBase(wd, "SoundVolume")
	{
		combo
			.push_back("0%")
			.push_back("10%")
			.push_back("20%")
			.push_back("30%")
			.push_back("40%")
			.push_back("50%")
			.push_back("60%")
			.push_back("70%")
			.push_back("80%")
			.push_back("90%")
			.push_back("100%");
	}

	void OptionSoundVolume::update(INI::Options& options)
	{
		combo.option(options.getSoundVolume().asInt());
	}

	OptionSoundToggle::OptionSoundToggle(nana::window wd) : OptionBase(wd, "SoundToggle")
	{
		combo
			.push_back("Disabled")
			.push_back("Enabled");
	}
	void OptionSoundToggle::update(INI::Options& options)
	{
		combo.option(options.getSoundToggle().asInt());
	}

	OptionMusicVolume::OptionMusicVolume(nana::window wd) : OptionBase(wd, "MusicVolume")
	{
		combo
			.push_back("0%")
			.push_back("10%")
			.push_back("20%")
			.push_back("30%")
			.push_back("40%")
			.push_back("50%")
			.push_back("60%")
			.push_back("70%")
			.push_back("80%")
			.push_back("90%")
			.push_back("100%");
	}

	void OptionMusicVolume::update(INI::Options& options)
	{
		combo.option(options.getMusicVolume().asInt());
	}

	OptionMusicToggle::OptionMusicToggle(nana::window wd) : OptionBase(wd, "MusicToggle")
	{
		combo
			.push_back("Disabled")
			.push_back("Enabled");
	}

	void OptionMusicToggle::update(INI::Options& options)
	{
		combo.option(options.getMusicToggle().asInt());
	}

	OptionsBox::OptionsBox(nana::window wd) : nana::panel<false>(wd)
	{
		place.div("vert things gap=2");
		place["things"] << language << scale << fullscreen << lefthanded
			<< soundvolume << soundtoggle << musicvolume << musictoggle;
	}

	void OptionsBox::update(INI::Options& options)
	{
		language.update(options);
		scale.update(options);
		fullscreen.update(options);
		lefthanded.update(options);
		soundvolume.update(options);
		soundtoggle.update(options);
		musicvolume.update(options);
		musictoggle.update(options);
	}

	TabOptions::TabOptions(nana::window wd) : nana::panel<false>(wd)
	{
		place.div("vert <path gap=5 margin=5 weight=35><options margin=5>");
		place["path"] << path;
		place["options"] << options;
	}

	void TabOptions::update(INI::Save& save)
	{
		options.update(save.getOptions());
	}
}