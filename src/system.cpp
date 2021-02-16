#include "system.hpp"

#define UNICODE
#include <winreg/WinReg.hpp>

namespace WhipseeySaveManager
{
namespace System
{
	Types::ErrDat<Types::Theme> systemTheme() 
	{
		Types::ErrDat<Types::Theme> errTheme;
		const std::wstring themeKey = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\Personalize)";
		const std::wstring themeDW = LR"(AppsUseLightTheme)";
		const std::wstring colorKey = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\History\Colors)";
		const std::wstring colorDW = LR"(ColorHistory0)";

		winreg::RegKey regHandler;

		if(regHandler.TryOpen(HKEY_CURRENT_USER, themeKey, KEY_READ))
		{
			std::optional<DWORD> lightTheme = regHandler.TryGetDwordValue(themeDW);
			if(lightTheme.has_value())
			{
				errTheme.data.darkmode = lightTheme.value() ? Types::Toggle::Disabled : Types::Toggle::Enabled;
			}
			else
			{
				errTheme.error.code = Types::Error::Code::ThemeDwordNotFound;
			}
		}
		else
		{
			errTheme.error.code = Types::Error::Code::ThemeKeyNotFound;
		}
		
		if(regHandler.TryOpen(HKEY_CURRENT_USER, colorKey, KEY_READ))
		{
			std::optional<DWORD> color = regHandler.TryGetDwordValue(colorDW);
			if(color.has_value())
			{
				errTheme.data.accent = color.value();
			}
			else
			{
				errTheme.error.code = Types::Error::Code::ColorDwordNotFound;
			}
		}
		else
		{
			errTheme.error.code = Types::Error::Code::ColorKeyNotFound;
		}

		return errTheme;
	}
}
}
