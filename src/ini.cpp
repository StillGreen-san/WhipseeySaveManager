#include "ini.hpp"

#include <SimpleIni.h>

namespace WhipseeySaveManager::INI
{
	Types::Error IKey::fromString(std::string_view string)
	{
		float newValue = mValue;
		applyDefaults();
		switch(mNumber)
		{
		case Number::Int :
		{
			if(std::all_of(string.begin(), string.end(), std::isdigit))
			{
				newValue = std::strtof(string.data(), nullptr);
			}
			else
			{
				return Types::Error::Code::InvalidFormat;
			}
			break;
		}
		case Number::StringFloat :
		case Number::StringInt :
		default:
		{
			auto fractionalPrecision = 0;
			auto ifFloatPart = [hasDot = bool(false), &fractionalPrecision](char chr) mutable
			{
				if(chr == '.')
				{
					if(hasDot == true)
					{
						return false;
					}
					else
					{
						hasDot = true;
						return true;
					}
				}
				else
				{
					if(hasDot)
					{
						++fractionalPrecision;
					}
					if(fractionalPrecision > 6)
					{
						return false;
					}
					return static_cast<bool>(std::isdigit(chr));
				}
			};
			if((string.front() == '"' && string.back() == '"')
			&& std::all_of(++string.begin(), --string.end(), ifFloatPart)
			&& fractionalPrecision == 6)
			{
				newValue = std::strtof(string.data()+1, nullptr);
				if(mNumber == Number::StringInt)
				{
					float asInt = std::floorf(newValue);
					if(asInt != newValue)
					{
						return Types::Error::Code::InvalidValue;
					}
					newValue = asInt;
				}
				break;
			}
			else
			{
				return Types::Error::Code::InvalidFormat;
			}
		}
		}
		switch(mLimits)
		{
		case Limits::EitherOr :
		{
			if(!(newValue == mMinOrA || newValue == mMaxOrB))
			{
				return Types::Error::Code::InvalidValue;
			}
			break;
		}
		case Limits::MinMax :
		default:
		{
			if(newValue < mMinOrA || newValue > mMaxOrB)
			{
				return Types::Error::Code::InvalidValue;
			}
			break;
		}
		}
		mValue = newValue;
		return {};
	}
	
	std::string IKey::toString() const
	{
		switch(mNumber)
		{
		case Number::Int :
			return std::to_string(static_cast<int>(mValue));
		case Number::StringFloat :
		case Number::StringInt :
		default:
			return std::to_string(mValue).insert(0, 1, '"').append(1, '"');
		}
	}

	class INI::INIintern final : public CSimpleIniA
	{
	public:
		static constexpr std::string_view NOT_FOUND = "%INVALID%";
		std::string_view GetValue(std::string_view section, std::string_view key, std::string_view default = NOT_FOUND)
		{
			return CSimpleIniA::GetValue(section.data(), key.data(), default.data());
		}
		void SetValue(std::string_view section, std::string_view key, std::string&& value)
		{
			CSimpleIniA::SetValue(section.data(), key.data(), value.data());
		}
	};

	INI::INI() : mIni(std::make_unique<INIintern>()) { }
	INI::~INI() = default;

	Types::Error INI::extractError()
	{
		return std::move(mError);
	}
	
	bool INI::loadFile(const std::filesystem::path& path)
	{
		mIni->Reset();
		const SI_Error siErr = mIni->LoadFile(path.native().c_str());
		if(siErr == SI_Error::SI_OK)
		{
			return true;
		}
		mError += Types::Error::Code::FailedToLoadFile;
		return false;
	}
	
	bool INI::writeFile(const std::filesystem::path& path) 
	{
		const SI_Error siErr = mIni->SaveFile(path.native().c_str());
		if(siErr == SI_Error::SI_OK)
		{
			return true;
		}
		mError += Types::Error::Code::FailedToWriteFile;
		return false;
	}
	
	bool INI::has(const std::shared_ptr<ISection>& section)
	{
		if(mIni->IsEmpty() || mIni->GetSection(section->section().data()) == nullptr)
		{
			mError += Types::Error::Code::SectionNotFound;
			return false;
		}
		return true;
	}

	bool INI::read(const std::shared_ptr<ISection>& section)
	{
		bool success = has(section);
		if(!success)
		{
			for(auto& key : section->keys())
			{
				key->applyDefaults();
			}
			return success;
		}
		for(auto& key : section->keys())
		{
			std::string_view value = mIni->GetValue(section->section(), key->key());
			if(value == INIintern::NOT_FOUND)
			{
				key->applyDefaults();
				mError += Types::Error::Code::KeyNotFound;
				success = false;
			}
			else
			{
				Types::Error error = key->fromString(value);
				if(error)
				{
					mError += error;
					success = false;
				}
			}
		}
		return success;
	}
	
	bool INI::read(const std::shared_ptr<IIni>& ini)
	{
		bool success = true;
		for(auto& section : ini->sections())
		{
			success &= read(section);
		}
		return success;
	}
	
	void INI::write(const std::shared_ptr<ISection>& section) 
	{
		for(auto& key : section->keys())
		{
			mIni->SetValue(section->section(), key->key(), key->toString());
		}
	}
	
	void INI::write(const std::shared_ptr<IIni>& ini) 
	{
		for(auto& section : ini->sections())
		{
			write(section);
		}
	}
}