#pragma once

#include "types.hpp"

#include <functional>

namespace WhipseeySaveManager
{
namespace GUI
{
	struct GUIimpl;
	
	class GUI
	{
	public:
		Types::Error run();
		void connectOnDefaultSavePath(std::function<Types::Return<std::filesystem::path>()> func);
		void connectOnDefaultSettingsPath(std::function<Types::Return<std::filesystem::path>()> func);
		void connectOnSelectSavePath(std::function<Types::Return<std::filesystem::path>()> func);
		void connectOnSelectSettingsPath(std::function<Types::Return<std::filesystem::path>()> func);
		void connectOnReadSave(std::function<Types::Return<Types::Save>(std::filesystem::path)> func);
		void connectOnReadSettings(std::function<Types::Return<Types::Settings>(std::filesystem::path)> func);
		void connectOnReadOptions(std::function<Types::Return<Types::Options>(std::filesystem::path)> func);
		void connectOnReadFile(std::function<Types::Return<Types::File>(std::filesystem::path, Types::FileIndex)> func);
		void connectOnWriteSave(std::function<Types::Return<void>(std::filesystem::path, Types::Save)> func);
		void connectOnWriteSettings(std::function<Types::Return<void>(std::filesystem::path, Types::Settings)> func);
		void connectOnWriteOptions(std::function<Types::Return<void>(std::filesystem::path, Types::Options)> func);
		void connectOnWriteFile(std::function<Types::Return<void>(std::filesystem::path, Types::FileIndex, Types::File)> func);
	private:
		std::unique_ptr<GUIimpl> guiImpl;
	};
} // namespace GUI
} // namespace WhipseeySaveManager
