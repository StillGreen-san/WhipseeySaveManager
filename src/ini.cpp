#include "ini.hpp"

#include <SimpleIni.h>

namespace WhipseeySaveManager::INI
{
	Types::Error IKey::fromString(std::string_view string)
	{
		float newValue = mValue;
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
			auto ifFloatPart = [hasDot = bool(false)](char chr) mutable
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
					return static_cast<bool>(std::isdigit(chr));
				}
			};
			if((string.front() == '"' && string.back() == '"')
			&& std::all_of(++string.begin(), --string.end(), ifFloatPart))
			{//TODO check fractional lenght? (not testet yet)
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
			if(newValue != mMinOrA || newValue != mMaxOrB)
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
		const SI_Error siErr = mIni->LoadFile(path.native().c_str());
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
			const bool hasSection = has(section);
			success &= hasSection;
			if(hasSection)
			{
				success &= read(section);
			}
		}
		return success;
	}
}