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
		using SectionSignature = Types::Error(std::shared_ptr<INI::ISection>, std::filesystem::path);
		using IniSignature = Types::Error(std::shared_ptr<INI::IIni>, std::filesystem::path);

		//TODO add comments
		void connectOnSystemTheme(std::function<ThemeSignature> func);
		void connectOnDefaultSavePath(std::function<PathSignature> func);
		void connectOnDefaultSettingsPath(std::function<PathSignature> func);
		void connectOnReadSection(std::function<SectionSignature> func);
		void connectOnReadIni(std::function<IniSignature> func);
		void connectOnWriteSection(std::function<SectionSignature> func);
		void connectOnWriteIni(std::function<IniSignature> func);

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
