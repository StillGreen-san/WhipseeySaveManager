#pragma once

#include "ini.hpp"

#include <functional>

namespace WhipseeySaveManager::GUI
{
	/**
	 * @brief internal implementation of GUI
	 * 
	 */
	struct GUIimpl;
	
	/**
	 * @brief userinterface, will run the main program loop
	 * 
	 */
	class GUI
	{
	public:
		/**
		 * @brief show the GUI, starts main loop (connect functions first)
		 * 
		 * @return Types::Error 
		 */
		Types::Error run();

		//TODO add comments
		void connectOnSystemTheme(std::function<std::optional<Types::Theme>()> func);
		void connectOnDefaultSavePath(std::function<std::optional<std::filesystem::path>()> func);
		void connectOnDefaultSettingsPath(std::function<std::optional<std::filesystem::path>()> func);
		void connectOnRead(std::function<Types::Error(std::shared_ptr<INI::ISection>, std::filesystem::path)> func);
		void connectOnRead(std::function<Types::Error(std::shared_ptr<INI::IIni>, std::filesystem::path)> func);
		void connectOnWrite(std::function<Types::Error(std::shared_ptr<INI::ISection>, std::filesystem::path)> func);
		void connectOnWrite(std::function<Types::Error(std::shared_ptr<INI::IIni>, std::filesystem::path)> func);
	private:
		std::unique_ptr<GUIimpl> guiImpl;
	};
} // namespace WhipseeySaveManager::GUI
