#pragma once

#include "ini.hpp"

#include <functional>

namespace WhipseeySaveManager::GUI
{
/**
 * @brief userinterface, will run the main program loop
 *
 */
class GUI
{
public:
	/**
	 * @brief show the GUI, starts main loop (connect functions first),
	 * returns when the gui is closed or an error occurred
	 *
	 * @return Types::Error if error MissingCallback
	 */
	Types::Error run();

	using ThemeSignature = std::optional<Types::Theme>();
	using PathSignature = std::optional<std::filesystem::path>();
	using SectionSignature = Types::Error(const std::shared_ptr<INI::ISection>&, const std::filesystem::path&);
	using IniSignature = Types::Error(const std::shared_ptr<INI::IIni>&, const std::filesystem::path&);

	/**
	 * @brief assigns callback to supply a Theme object
	 *
	 * @param func see GUI::ThemeSignature
	 */
	void connectOnSystemTheme(std::function<ThemeSignature> func);

	/**
	 * @brief assigns callback to supply a path for INI::Save
	 *
	 * @param func see GUI::PathSignature
	 */
	void connectOnDefaultSavePath(std::function<PathSignature> func);

	/**
	 * @brief assigns callback to supply a path for INI::Settings
	 *
	 * @param func see GUI::PathSignature
	 */
	void connectOnDefaultSettingsPath(std::function<PathSignature> func);

	/**
	 * @brief assigns callback for reading INI::ISection
	 *
	 * @param func see GUI::SectionSignature
	 */
	void connectOnReadSection(std::function<SectionSignature> func);

	/**
	 * @brief assigns callback for reading INI::IIni
	 *
	 * @param func see GUI::IniSignature
	 */
	void connectOnReadIni(std::function<IniSignature> func);

	/**
	 * @brief assigns callback for writing INI::ISection
	 *
	 * @param func see GUI::SectionSignature
	 */
	void connectOnWriteSection(std::function<SectionSignature> func);

	/**
	 * @brief assigns callback for writing INI::IIni
	 *
	 * @param func see GUI::IniSignature
	 */
	void connectOnWriteIni(std::function<IniSignature> func);

	/**
	 * @brief struct holding all necessary callback
	 *
	 */
	struct FunctionStore
	{
		std::function<ThemeSignature> onSystemTheme;
		std::function<PathSignature> onDefaultSavePath;
		std::function<PathSignature> onDefaultSettingsPath;
		std::function<SectionSignature> onReadSection;
		std::function<IniSignature> onReadIni;
		std::function<SectionSignature> onWriteSection;
		std::function<IniSignature> onWriteIni;
	};

private:
	struct FunctionStore callbacks;
};
} // namespace WhipseeySaveManager::GUI
