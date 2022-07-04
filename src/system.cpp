#include "system.hpp"

#define UNICODE
#include <ShlObj.h>
#include <winreg/WinReg.hpp>

#include <fstream>
#include <regex>

namespace
{
const HKEY HKeyCurrentUser = HKEY_CURRENT_USER;   // NOLINT
const HKEY HKeyLocalMachine = HKEY_LOCAL_MACHINE; // NOLINT
constexpr REGSAM KeyRead = KEY_READ;              // NOLINT
constexpr REGSAM KeyWow6432 = KEY_WOW64_32KEY;    // NOLINT
} // namespace

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

	if(!regHandler.TryOpen(HKeyCurrentUser, themeKey, KeyRead))
	{
		return {};
	}

	if(std::optional<DWORD> lightTheme = regHandler.TryGetDwordValue(themeDW))
	{
		theme.darkmode = lightTheme.value() != 0U ? Types::Darkmode::Disabled : Types::Darkmode::Enabled;
	}
	else
	{
		return {};
	}

	if(!regHandler.TryOpen(HKeyCurrentUser, colorKey, KeyRead))
	{
		return {};
	}

	if(std::optional<DWORD> color = regHandler.TryGetDwordValue(colorDW))
	{
		theme.accent = color.value();
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
		pointer = other.pointer; // NOLINT(cppcoreguidelines-prefer-member-initializer)
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
		pointer = nullptr;
	}
	~CoTaskMem()
	{
		Free();
	}
	[[nodiscard]] T* Get() const
	{
		return pointer;
	}
	[[nodiscard]] operator T*() // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
	{
		return pointer;
	}
	[[nodiscard]] operator T**() // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
	{
		return &pointer;
	}

private:
	T* pointer = nullptr;
};

std::optional<std::filesystem::path> defaultSavePath()
{
	std::filesystem::path path;
	CoTaskMem<WCHAR> szPath;
	if(SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, nullptr, szPath) != S_OK)
	{
		return {};
	}

	path.assign(szPath.Get());

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

	if(!regHandler.TryOpen(HKeyLocalMachine, steamKey, KeyRead | KeyWow6432))
	{
		return {};
	}

	if(std::optional<std::wstring> installPath = regHandler.TryGetStringValue(steamSZ))
	{
		steamPath.assign(installPath.value());
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
	if(!librariesFile.is_open())
	{
		return {};
	}

	const std::regex libraryEntryReg(R"#(.*"path".*"(.*)")#", std::regex_constants::optimize);
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

Types::Error read(const std::shared_ptr<INI::ISection>& section, const std::filesystem::path& file)
{
	INI::INI ini;
	if(!ini.loadFile(file))
	{
		return ini.extractError();
	}
	ini.read(section);
	return ini.extractError();
}

Types::Error read(const std::shared_ptr<INI::IIni>& ini, const std::filesystem::path& file)
{
	INI::INI sIni;
	if(!sIni.loadFile(file))
	{
		return sIni.extractError();
	}
	sIni.read(ini);
	return sIni.extractError();
}

Types::Error write(const std::shared_ptr<INI::ISection>& section, const std::filesystem::path& file)
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

Types::Error write(const std::shared_ptr<INI::IIni>& ini, const std::filesystem::path& file)
{
	INI::INI sIni;
	sIni.write(ini);
	sIni.writeFile(file);
	return sIni.extractError();
}
} // namespace WhipseeySaveManager::System
