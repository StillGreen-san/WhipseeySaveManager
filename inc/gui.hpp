#pragma once

#include "data.hpp"

namespace WhipseeySaveManager
{
namespace GUI
{
	struct GUIimpl;
	
	class GUI
	{
	public:
		GUI();
		~GUI();
		Error::Error run();//TODO add error to returns of connections
		void connectOnOpenSave(std::filesystem::path (*func)());
		void connectOnOpenSettings(std::filesystem::path (*func)());
		void connectOnWriteSave(Data::Save (*func)(std::filesystem::path));
		void connectOnWriteSettings(Data::Settings (*func)(std::filesystem::path));
		void connectOnWriteFile(Data::File (*func)(std::filesystem::path, Data::FileIndex));
		void connectOnWriteOptions(Data::Options (*func)(std::filesystem::path));
		void connectOnReadSave(void (*func)(std::filesystem::path, Data::Save));
		void connectOnReadSettings(void (*func)(std::filesystem::path, Data::Settings));
		void connectOnReadFile(void (*func)(std::filesystem::path, Data::FileIndex, Data::File));
		void connectOnReadOptions(void (*func)(std::filesystem::path, Data::Options));
	private:
		std::unique_ptr<GUIimpl> guiImpl;
	};
} // namespace GUI
} // namespace WhipseeySaveManager
