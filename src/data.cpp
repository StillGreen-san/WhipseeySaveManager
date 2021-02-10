#include "data.hpp"

//TODO remove warning pragma
#pragma warning(disable:4100)

namespace WhipseeySaveManager
{
namespace Data
{
	Error::Error Data::readSave(const std::filesystem::path& path)
	{
		return Error::makeError(Error::Where::Save, Error::What::Unknown);
	}
	
	Error::Error Data::writeSave(const std::filesystem::path& path) const
	{
		return Error::makeError(Error::Where::Save, Error::What::Unknown);
	}
	
	Error::Error Data::writeSave() const
	{
		return Error::makeError(Error::Where::Save, Error::What::Unknown);
	}
	
	Error::Error Data::readSettings(const std::filesystem::path& path)
	{
		return Error::makeError(Error::Where::Settings, Error::What::Unknown);
	}
	
	Error::Error Data::writeSettings(const std::filesystem::path& path) const
	{
		return Error::makeError(Error::Where::Settings, Error::What::Unknown);
	}
	
	Error::Error Data::writeSettings() const
	{
		return Error::makeError(Error::Where::Save, Error::What::Unknown);
	}
	
	const Save& Data::getSave() const
	{
		return mSave;
	}
	
	const Settings& Data::getSettings() const
	{
		return mSettings;
	}
	
	const File& Data::getFile(FileIndex index) const
	{
		switch(index)
		{
		case FileIndex::File1 : return mSave.file1;
		case FileIndex::File2 : return mSave.file2;
		case FileIndex::File3 : return mSave.file3;
		}

		throw Error::makeError(Error::Where::Data, Error::What::Value);
	}
	
	Error::Error Data::setSave(Save save) 
	{
		return Error::makeError(Error::Where::Save, Error::What::Unknown);
	}
	
	Error::Error Data::setSettings(Settings settings) 
	{
		return Error::makeError(Error::Where::Settings, Error::What::Unknown);
	}
	
	Error::Error Data::setFile(FileIndex index, File file) 
	{
		return Error::makeError(Error::Where::File, Error::What::Unknown);
	}
}
}

//TODO remove warning pragma
#pragma warning(default:4100)
