#include "ini.hpp"

#include <SimpleIni.h>

#include <fstream>

bool isdigt(char chr)
{
	return static_cast<bool>(std::isdigit(static_cast<unsigned char>(chr)));
}

namespace WhipseeySaveManager::INI
{
Types::Error IKey::fromString(std::string_view string)
{
	float newValue = mValue;
	applyDefaults();

	switch(mFormat)
	{
	case Format::Int:
	{
		if(std::all_of(string.begin(), string.end(), isdigit))
		{
			newValue = std::strtof(string.data(), nullptr);
		}
		else
		{
			return Types::Error::Code::InvalidFormat;
		}
		break;
	}
	case Format::StringFloat:
	case Format::StringInt:
	default:
	{
		constexpr unsigned expectedFractionalPrecision = 6;
		unsigned fractionalPrecision = 0;

		auto ifFloatPart = [hasDot = bool(false), &fractionalPrecision, expectedFractionalPrecision](char chr) mutable
		{
			if(chr == '.')
			{
				if(hasDot)
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
				if(fractionalPrecision > expectedFractionalPrecision)
				{
					return false;
				}
				return static_cast<bool>(std::isdigit(chr));
			}
		};

		if((string.front() == '"' && string.back() == '"') &&
		   std::all_of(++string.begin(), --string.end(), ifFloatPart) &&
		   fractionalPrecision == expectedFractionalPrecision)
		{
			newValue = std::strtof(string.data() + 1, nullptr);
			if(mFormat == Format::StringInt)
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
	case Limits::EitherOr:
	{
		if(!(newValue == mMinOrA || newValue == mMaxOrB))
		{
			return Types::Error::Code::InvalidValue;
		}
		break;
	}
	case Limits::MinMax:
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
	switch(mFormat)
	{
	case Format::Int:
		return std::to_string(static_cast<int>(mValue));
	case Format::StringFloat:
	case Format::StringInt:
	default:
		return std::to_string(mValue).insert(0, 1, '"').append(1, '"');
	}
}

Types::Level FileBase::getLevel()
{
	if(getCastle() == Types::Castle::Unlocked)
	{
		return Types::Level::Castle;
	}
	if(getMoon() == Types::Moon::Unlocked)
	{
		return Types::Level::Moon;
	}
	if(getSnow() == Types::Snow::Unlocked)
	{
		return Types::Level::Snow;
	}
	if(getDesert() == Types::Desert::Unlocked)
	{
		return Types::Level::Desert;
	}
	if(getForest() == Types::Forest::Unlocked)
	{
		return Types::Level::Forest;
	}
	return Types::Level::Beach;
}

void FileBase::setLevel(Types::Level level)
{
	getCastle() = Types::Castle::Locked;
	getMoon() = Types::Moon::Locked;
	getSnow() = Types::Snow::Locked;
	getDesert() = Types::Desert::Locked;
	getForest() = Types::Forest::Locked;

	switch(level)
	{
	case Types::Level::Castle:
		getCastle() = Types::Castle::Unlocked;
		[[fallthrough]];
	case Types::Level::Moon:
		getMoon() = Types::Moon::Unlocked;
		[[fallthrough]];
	case Types::Level::Snow:
		getSnow() = Types::Snow::Unlocked;
		[[fallthrough]];
	case Types::Level::Desert:
		getDesert() = Types::Desert::Unlocked;
		[[fallthrough]];
	case Types::Level::Forest:
		getForest() = Types::Forest::Unlocked;
	case Types::Level::Beach:
		break;
	}
}

class INI::INIintern final : public CSimpleIniA
{
public:
	static constexpr std::string_view NOT_FOUND = "%INVALID%";
	std::string_view GetValue(
	    std::string_view section, std::string_view key, std::string_view defaultVal = NOT_FOUND) const
	{
		return CSimpleIniA::GetValue(section.data(), key.data(), defaultVal.data());
	}
	void SetValue(std::string_view section, std::string_view key, std::string&& value)
	{
		CSimpleIniA::SetValue(section.data(), key.data(), value.data());
	}
	const std::multimap<CSimpleIniA::Entry, const char*, CSimpleIniA::Entry::KeyOrder>* GetSection(
	    std::string_view section) const
	{
		return CSimpleIniA::GetSection(section.data());
	}
};

INI::INI() : mIni(std::make_unique<INIintern>())
{
	mIni->SetSpaces(false); // to mimick original style of gamefiles
}

INI::~INI() = default;

Types::Error INI::extractError()
{
	return std::move(mError);
}

bool INI::loadFile(const std::filesystem::path& path)
{
	mIni->Reset();
	if(mIni->LoadFile(path.native().c_str()) == SI_Error::SI_OK)
	{
		return true;
	}
	mError += Types::Error::Code::FailedToLoadFile;
	return false;
}

bool INI::writeFile(const std::filesystem::path& path)
{
	if(mIni->GetSection(Cheats::name))
	{
		if(mIni->SaveFile(path.native().c_str()) == SI_Error::SI_OK)
		{
			return true;
		}
	}
	else
	{
		constexpr size_t ORG_FILESIZE = 1024;                       // the savefile is padded to 1kb by the game
		constexpr std::string_view SI_SECTION_END = "\r\n\r\n\r\n"; // SimpleIni adds two extra line end after a section
		std::string buffer;
		buffer.reserve(ORG_FILESIZE);

		if(mIni->Save(buffer) == SI_Error::SI_OK)
		{
			size_t sectionEnd = buffer.find(SI_SECTION_END);
			while(sectionEnd != std::string::npos)
			{
				buffer.erase(sectionEnd, 4);
				sectionEnd = buffer.find(SI_SECTION_END);
			}

			buffer.resize(ORG_FILESIZE);
			std::ofstream out(path, std::ios_base::binary); // binary to avoid extra newlines added by ofstream
			out << buffer;
			if(out.good())
			{
				return true;
			}
		}
	}
	mError += Types::Error::Code::FailedToWriteFile;
	return false;
}

bool INI::has(const std::shared_ptr<ISection>& section)
{
	if(mIni->IsEmpty() || !mIni->GetSection(section->section().data()))
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
		for(const auto& key : section->keys())
		{
			key->applyDefaults();
		}
		return success;
	}

	for(const auto& key : section->keys())
	{
		std::string_view value = mIni->GetValue(section->section(), key->key());
		if(value == INIintern::NOT_FOUND)
		{
			if(key->key() != BossNoDamageProgress::name) //! explicit instead of making changes to IKey
			{
				key->applyDefaults();
				mError += Types::Error::Code::KeyNotFound;
				success = false;
			}
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
	for(const auto& section : ini->sections())
	{
		success &= read(section);
	}
	return success;
}

void INI::write(const std::shared_ptr<ISection>& section)
{
	for(const auto& key : section->keys())
	{
		mIni->SetValue(section->section(), key->key(), key->toString());
	}
}

void INI::write(const std::shared_ptr<IIni>& ini)
{
	for(const auto& section : ini->sections())
	{
		write(section);
	}
}
} // namespace WhipseeySaveManager::INI
