#pragma once

#include "types.hpp"

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
		void connectOnDarkmodeEnabled(std::function<Types::ErrDat<Types::Toggle>()> func);
		void connectOnDefaultSavePath(std::function<Types::ErrDat<std::filesystem::path>()> func);
		void connectOnDefaultSettingsPath(std::function<Types::ErrDat<std::filesystem::path>()> func);
		void connectOnReadSave(std::function<Types::ErrDat<Types::Save>(std::filesystem::path)> func);
		void connectOnReadSettings(std::function<Types::ErrDat<Types::Settings>(std::filesystem::path)> func);
		void connectOnReadOptions(std::function<Types::ErrDat<Types::Options>(std::filesystem::path)> func);
		void connectOnReadFile(std::function<Types::ErrDat<Types::File>(std::filesystem::path, Types::FileIndex)> func);
		void connectOnWriteSave(std::function<Types::Error(std::filesystem::path, Types::Save)> func);
		void connectOnWriteSettings(std::function<Types::Error(std::filesystem::path, Types::Settings)> func);
		void connectOnWriteOptions(std::function<Types::Error(std::filesystem::path, Types::Options)> func);
		void connectOnWriteFile(std::function<Types::Error(std::filesystem::path, Types::FileIndex, Types::File)> func);
	private:
		std::unique_ptr<GUIimpl> guiImpl;
	};
} // namespace WhipseeySaveManager::GUI
