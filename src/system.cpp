#include "system.hpp"

#define UNICODE
#include <ShlObj.h>
#include <winreg/WinReg.hpp>

#include <fstream>
#include <regex>

#include <SimpleIni.h>

namespace WhipseeySaveManager::System
{
std::optional<Types::Theme> systemTheme()
{
	const std::wstring themeKey = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\Personalize)";
	const std::wstring themeDW = LR"(AppsUseLightTheme)";
	const std::wstring colorKey = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\History\Colors)";
	const std::wstring colorDW = LR"(ColorHistory0)";

	Types::Theme theme;
	winreg::RegKey regHandler;

	if(regHandler.TryOpen(HKEY_CURRENT_USER, themeKey, KEY_READ))
	{
		std::optional<DWORD> lightTheme = regHandler.TryGetDwordValue(themeDW);
		if(lightTheme.has_value())
		{
			theme.darkmode = lightTheme.value() ? Types::Darkmode::Disabled : Types::Darkmode::Enabled;
		}
		else
		{
			return {};
		}
	}
	else
	{
		return {};
	}

	if(regHandler.TryOpen(HKEY_CURRENT_USER, colorKey, KEY_READ))
	{
		std::optional<DWORD> color = regHandler.TryGetDwordValue(colorDW);
		if(color.has_value())
		{
			theme.accent = color.value();
		}
		else
		{
			return {};
		}
	}
	else
	{
		return {};
	}

	return theme;
}

template<typename T>
class CoTaskMem
{
public:
	CoTaskMem() = default;
	CoTaskMem(const CoTaskMem&) = delete;
	CoTaskMem(CoTaskMem&& other) noexcept
	{
		pointer = other.pointer;
		other.pointer = nullptr;
	}
	CoTaskMem& operator=(const CoTaskMem&) = delete;
	CoTaskMem& operator=(CoTaskMem&& other) noexcept
	{
		pointer = other.pointer;
		other.pointer = nullptr;
		return *this;
	}
	void Alloc(size_t elements)
	{
		Free();
		pointer = static_cast<T*>(CoTaskMemAlloc(elements * sizeof(T)));
	}
	void Realloc(size_t elements)
	{
		pointer = static_cast<T*>(CoTaskMemRealloc(pointer, elements * sizeof(T)));
	}
	void Free()
	{
		CoTaskMemFree(pointer);
		pointer = NULL;
	}
	~CoTaskMem()
	{
		Free();
	}
	T* Get() const
	{
		return pointer;
	}
	operator T*()
	{
		return pointer;
	}
	operator T**()
	{
		return &pointer;
	}

private:
	T* pointer = NULL;
};

std::optional<std::filesystem::path> defaultSavePath()
{
	std::filesystem::path path;
	CoTaskMem<WCHAR> szPath;
	HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, NULL, szPath);
	if(result == S_OK)
	{
		path.assign(szPath.Get());
	}
	else
	{
		return {};
	}

	const std::filesystem::path saveRelativePath(R"(Whipseey\savedata\whipseey.sav)");
	path /= saveRelativePath;
	if(!std::filesystem::exists(path))
	{
		return {};
	}

	return path.make_preferred();
}

std::optional<std::filesystem::path> defaultSettingsPath()
{
	const std::wstring steamKey = LR"(SOFTWARE\Valve\Steam)";
	const std::wstring steamSZ = LR"(InstallPath)";

	std::filesystem::path steamPath;
	winreg::RegKey regHandler;

	if(regHandler.TryOpen(HKEY_LOCAL_MACHINE, steamKey, KEY_READ | KEY_WOW64_32KEY))
	{
		std::optional<std::wstring> installPath = regHandler.TryGetStringValue(steamSZ);
		if(installPath.has_value())
		{
			steamPath.assign(installPath.value());
		}
		else
		{
			return {};
		}
	}
	else
	{
		return {};
	}

	const std::filesystem::path settingsRelativePath("steamapps/common/Whipseey and the Lost Atlas/bfs_settings.ini");
	std::filesystem::path settingsPath(steamPath / settingsRelativePath);

	if(std::filesystem::exists(settingsPath))
	{
		return settingsPath.make_preferred();
	}

	const std::filesystem::path librariesFilePath(steamPath / "steamapps/libraryfolders.vdf");

	if(!std::filesystem::exists(librariesFilePath))
	{
		return {};
	}

	std::ifstream librariesFile(librariesFilePath);
	const std::regex libraryEntryReg(R"#(.*"\d+".*"(.*)")#", std::regex_constants::optimize);
	std::smatch match;

	for(std::string line; std::getline(librariesFile, line);)
	{
		if(std::regex_match(line, match, libraryEntryReg) && match.size() == 2)
		{
			settingsPath.assign(match[1].str() / settingsRelativePath);
			if(std::filesystem::exists(settingsPath))
			{
				return settingsPath.make_preferred().lexically_normal();
			}
		}
	}

	return {};
}

Types::Error read(std::shared_ptr<INI::ISection> section, std::filesystem::path file)
{
	INI::INI ini;
	if(!ini.loadFile(file))
	{
		return ini.extractError();
	}
	ini.read(section);
	return ini.extractError();
}

Types::Error read(std::shared_ptr<INI::IIni> ini, std::filesystem::path file)
{
	INI::INI sIni;
	if(!sIni.loadFile(file))
	{
		return sIni.extractError();
	}
	sIni.read(ini);
	return sIni.extractError();
}

Types::Error write(std::shared_ptr<INI::ISection> section, std::filesystem::path file)
{
	INI::INI ini;
	if(std::filesystem::exists(file))
	{
		ini.loadFile(file);
	}
	ini.write(section);
	ini.writeFile(file);
	return ini.extractError();
}

Types::Error write(std::shared_ptr<INI::IIni> ini, std::filesystem::path file)
{
	INI::INI sIni;
	sIni.write(ini);
	sIni.writeFile(file);
	return sIni.extractError();
}
} // namespace WhipseeySaveManager::System
