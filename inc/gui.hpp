#pragma once

#include "data.hpp"

namespace WhipseeySaveManager
{
namespace GUI
{
	template<typename Data>
	struct Return
	{
		Error::Error error;
		Data data;
	};

	struct Return<void>
	{
		Error::Error error;
	};

	struct Return<Data::Save>
	{
		Error::Error error;
		Data::Save save;
	};

	struct Return<Data::Settings>
	{
		Error::Error error;
		Data::Settings settings;
	};

	struct Return<std::filesystem::path>
	{
		Error::Error error;
		std::filesystem::path path;
	};

	struct Return<Data::File>
	{
		Error::Error error;
		Data::File file;
	};

	struct Return<Data::Options>
	{
		Error::Error error;
		Data::Options options;
	};

	struct GUIimpl;
	
	class GUI
	{
	public:
		Error::Error run();
		void connectOnDefaultSavePath(std::function<Return<std::filesystem::path>()> func);
		void connectOnDefaultSettingsPath(std::function<Return<std::filesystem::path>()> func);
		void connectOnSelectSavePath(std::function<Return<std::filesystem::path>()> func);
		void connectOnSelectSettingsPath(std::function<Return<std::filesystem::path>()> func);
		void connectOnReadSave(std::function<Return<Data::Save>(std::filesystem::path)> func);
		void connectOnReadSettings(std::function<Return<Data::Settings>(std::filesystem::path)> func);
		void connectOnReadOptions(std::function<Return<Data::Options>(std::filesystem::path)> func);
		void connectOnReadFile(std::function<Return<Data::File>(std::filesystem::path, Data::FileIndex)> func);
		void connectOnWriteSave(std::function<Return<void>(std::filesystem::path, Data::Save)> func);
		void connectOnWriteSettings(std::function<Return<void>(std::filesystem::path, Data::Settings)> func);
		void connectOnWriteOptions(std::function<Return<void>(std::filesystem::path, Data::Options)> func);
		void connectOnWriteFile(std::function<Return<void>(std::filesystem::path, Data::FileIndex, Data::File)> func);
	private:
		std::unique_ptr<GUIimpl> guiImpl;
	};
} // namespace GUI
} // namespace WhipseeySaveManager
