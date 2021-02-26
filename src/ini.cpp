#include "ini.hpp"

#include <SimpleIni.h>

namespace WhipseeySaveManager::INI
{
	class INI::INIintern final : public CSimpleIniA
	{
	public:
		std::string_view GetValue(std::string_view section, std::string_view key, std::string_view default)
		{
			return CSimpleIniA::GetValue(section.data(), key.data(), default.data());
		}
	};

	INI::INI() : mIni(std::make_unique<INIintern>()) { }

	Types::Error INI::extractError()
	{
		// Types::Error error = std::move(mError);
		return std::move(mError);
	}
	
	bool INI::loadFile(const std::filesystem::path& path)
	{
		SI_Error siErr = mIni->LoadFile(path.native().c_str());
		if(siErr == SI_Error::SI_OK)
		{
			return true;
		}
		mError += Types::Error::Code::FailedToLoadFile;
		return false;
	}
	
	bool INI::has(const std::shared_ptr<ISection>& section)
	{
		if(mIni->IsEmpty())
		{
			return false;
		}
		if(mIni->GetSection(section->section().data()) == nullptr)
		{
			mError += Types::Error::Code::SectionNotFound;
			return false;
		}
		return true;
	}
	
	constexpr std::string_view NOT_FOUND = "%INVALID%";

	bool INI::read(const std::shared_ptr<ISection>& section)
	{
		bool success = true;
		for(auto& key : section->keys())
		{
			std::string_view value = mIni->GetValue(section->section(), key->key(), NOT_FOUND);
			if(value == NOT_FOUND)
			{
				mError += Types::Error::Code::KeyNotFound;
				success = false;
			}
			else
			{
				mError += key->fromString(value);
			}
		}
		return success;
	}
	
	bool INI::read(const std::shared_ptr<IIni>& ini)
	{
		bool success = true;
		for(auto& section : ini->sections())
		{
			bool hasSection = has(section);
			success &= hasSection;
			if(hasSection)
			{
				success &= read(section);
			}
		}
		return success;
	}
}