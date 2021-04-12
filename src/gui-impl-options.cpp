#include "gui-impl-options.hpp"

namespace WhipseeySaveManager::GUI
{
OptionBase::OptionBase(nana::window wd, std::string_view labelText) :
    nana::panel<false>(wd), label{*this, labelText.data()}
{
	label.text_align(nana::align::right, nana::align_v::center);
	place.div("things gap=8 arrange=[190,300]");
	place["things"] << label << combo;
}

nana::basic_event<nana::arg_combox>& OptionBase::onChanged()
{
	return combo.events().text_changed;
}

size_t OptionBase::option()
{
	return combo.option();
}

OptionLanguage::OptionLanguage(nana::window wd) : OptionBase(wd, "Language")
{
	combo.push_back("English")
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
	combo.push_back("R768x432").push_back("R1152x648").push_back("R1536x864");
}

void OptionScale::update(INI::Options& options)
{
	combo.option(options.getScale().asInt() - 2);
}

OptionFullscreen::OptionFullscreen(nana::window wd) : OptionBase(wd, "Fullscreen")
{
	combo.push_back("Disabled").push_back("Enabled");
}

void OptionFullscreen::update(INI::Options& options)
{
	combo.option(options.getFullscreen().asInt());
}

OptionLeftHanded::OptionLeftHanded(nana::window wd) : OptionBase(wd, "Left Handed")
{
	combo.push_back("Disabled").push_back("Enabled");
}

void OptionLeftHanded::update(INI::Options& options)
{
	combo.option(options.getLeftHanded().asInt());
}

OptionSoundVolume::OptionSoundVolume(nana::window wd) : OptionBase(wd, "Sound Volume")
{
	combo.push_back("0%")
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

OptionSoundToggle::OptionSoundToggle(nana::window wd) : OptionBase(wd, "Sound Toggle")
{
	combo.push_back("Disabled").push_back("Enabled");
}
void OptionSoundToggle::update(INI::Options& options)
{
	combo.option(options.getSoundToggle().asInt());
}

OptionMusicVolume::OptionMusicVolume(nana::window wd) : OptionBase(wd, "Music Volume")
{
	combo.push_back("0%")
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

OptionMusicToggle::OptionMusicToggle(nana::window wd) : OptionBase(wd, "Music Toggle")
{
	combo.push_back("Disabled").push_back("Enabled");
}

void OptionMusicToggle::update(INI::Options& options)
{
	combo.option(options.getMusicToggle().asInt());
}

OptionsBox::OptionsBox(nana::window wd) : nana::panel<false>(wd)
{
	place.div("vert things gap=2");
	place["things"] << language << scale << fullscreen << lefthanded << soundvolume << soundtoggle << musicvolume
	                << musictoggle;
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

void OptionsBox::get(INI::Options& options)
{
	options.getLanguage() = static_cast<Types::Language>(language.option());
	options.getScale() = static_cast<Types::Scale>(scale.option() + 2);
	options.getFullscreen() = static_cast<Types::Fullscreen>(fullscreen.option());
	options.getLeftHanded() = static_cast<Types::LeftHanded>(lefthanded.option());
	options.getSoundVolume() = static_cast<Types::SoundVolume>(soundvolume.option());
	options.getSoundToggle() = static_cast<Types::SoundToggle>(soundtoggle.option());
	options.getMusicVolume() = static_cast<Types::MusicVolume>(musicvolume.option());
	options.getMusicToggle() = static_cast<Types::MusicToggle>(musictoggle.option());
}

TabOptions::TabOptions(nana::window wd, const std::shared_ptr<INI::Save>& save, const GUI::FunctionStore& callbacks) :
    nana::panel<false>(wd)
{
	place.div("vert <path gap=5 margin=5 weight=35><options margin=5>");
	place["path"] << path;
	place["options"] << options;

	path.onReload().connect_front(
	    [&]([[maybe_unused]] const nana::arg_click& click)
	    {
		    showErrorMsg(callbacks.onReadSection(save->getOptions(), path.getPath()));
		    options.update(*save->getOptions());
	    });

	path.onSave().connect_front(
	    [&]([[maybe_unused]] const nana::arg_click& click)
	    {
		    options.get(*save->getOptions());
		    showErrorMsg(callbacks.onWriteSection(save->getOptions(), path.getPath()));
	    });

	std::optional<std::filesystem::path> savePath = callbacks.onDefaultSavePath();
	if(savePath)
	{
		path.setPath(*savePath);
	}
}
} // namespace WhipseeySaveManager::GUI
