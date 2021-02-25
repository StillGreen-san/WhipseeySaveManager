#pragma once

#include "types.hpp"
#include <memory>

namespace WhipseeySaveManager
{
/**
 * @brief 
 * 
 */
namespace INI
{//TODO remove magic index numbers
	class IKey
	{
	public:
		virtual std::string_view key() const = 0;
		virtual Types::Error fromString(std::string_view string)
		{
			char* end = nullptr;
			size_t offset = string.front() == '"' ? 1 : 0;
			float value = strtof(string.data()+offset, &end);
			if(value == 0 && string.data()+offset == end)
			{
				return Types::Error::Code::InvalidFormat;
			}
			Types::Error error = validate(value);
			if(error == false)
			{
				mValue = value;
			}
			return error;
		}
		virtual std::string toString() const
		{
			return std::to_string(mValue).insert(0, 1, '"').append(1, '"');
		}
		bool operator==(const IKey& other) const
		{
			return mValue == other.mValue;
		}
		virtual ~IKey() = default;
	protected:
		explicit IKey(float value) : mValue{value} {}
		virtual Types::Error validate(float value) const = 0;
		float mValue;
	};

	class ISection
	{
	public:
		ISection(std::initializer_list<std::shared_ptr<IKey>> il) : mKeys{il} {}
		virtual std::string_view section() const = 0;
		virtual const std::vector<std::shared_ptr<IKey>>& keys()
		{
			return mKeys;
		}
		virtual ~ISection() = default;
	protected:
		std::vector<std::shared_ptr<IKey>> mKeys;
	};

	class IIni
	{
	public:
		IIni(std::initializer_list<std::shared_ptr<ISection>> il) : mSections{il} {}
		virtual const std::vector<std::shared_ptr<ISection>>& sections()
		{
			return mSections;
		}
		virtual ~IIni() = default;
	protected:
		std::vector<std::shared_ptr<ISection>> mSections;
	};



	class INI
	{
	public:
		Types::Error extractError();
		bool loadFile(std::filesystem::path);
		bool has(std::shared_ptr<ISection>) const;
		bool read(std::shared_ptr<ISection>);
		bool read(std::shared_ptr<IIni>);
	private:
		Types::Error mError;
	};



	class Language final : public IKey
	{
	public:
		static constexpr std::string_view name = "language";
		Language() : IKey(static_cast<float>(Types::Language::English)) { }
		bool operator==(const Types::Language& other) const
		{
			return mValue == static_cast<float>(other);
		}
		Language& operator=(Types::Language value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		Types::Language get()
		{
			return static_cast<Types::Language>(mValue);
		}
		std::string_view key() const override
		{
			return name;
		}
	private:
		Types::Error validate(float value) const override
		{
			if(value >= 0 && value <= 9)
			{
				return {};
			}
			return Types::Error::Code::InvalidValue;
		}
	};

	class Scale final : public IKey
	{
	public:
		static constexpr std::string_view name = "scale";
		Scale() : IKey(static_cast<float>(Types::Scale::R768x432)) { }
		bool operator==(const Types::Scale& other) const
		{
			return mValue == static_cast<float>(other);
		}
		Scale& operator=(Types::Scale value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		Types::Scale get()
		{
			return static_cast<Types::Scale>(mValue);
		}
		std::string_view key() const override
		{
			return name;
		}
	private:
		Types::Error validate(float value) const override
		{
			if(value >= 2 && value <= 4)
			{
				return {};
			}
			return Types::Error::Code::InvalidValue;
		}
	};

	class Fullscreen final : public IKey
	{
	public:
		static constexpr std::string_view name = "fullscreen";
		Fullscreen() : IKey(static_cast<float>(Types::Toggle::Enabled)) { }
		bool operator==(const Types::Toggle& other) const
		{
			return mValue == static_cast<float>(other);
		}
		Fullscreen& operator=(Types::Toggle value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		Types::Toggle get()
		{
			return static_cast<Types::Toggle>(mValue);
		}
		std::string_view key() const override
		{
			return name;
		}
	private:
		Types::Error validate(float value) const override
		{
			if(value == 0 || value == 1)
			{
				return {};
			}
			return Types::Error::Code::InvalidValue;
		}
	};

	class LeftHanded final : public IKey
	{
	public:
		static constexpr std::string_view name = "left_handed";
		LeftHanded() : IKey(static_cast<float>(Types::Toggle::Disabled)) { }
		bool operator==(const Types::Toggle& other) const
		{
			return mValue == static_cast<float>(other);
		}
		LeftHanded& operator=(Types::Toggle value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		Types::Toggle get()
		{
			return static_cast<Types::Toggle>(mValue);
		}
		std::string_view key() const override
		{
			return name;
		}
	private:
		Types::Error validate(float value) const override
		{
			if(value == 0 || value == 1)
			{
				return {};
			}
			return Types::Error::Code::InvalidValue;
		}
	};
	//! static functions selected by enums instead of virtual?


	class Options final : public ISection
	{
	public:
		static constexpr std::string_view name = "options";
		Options() : ISection({
			std::make_shared<Language>(),
			std::make_shared<Scale>(),
			std::make_shared<Fullscreen>()
		}) { }
		std::string_view section() const override
		{
			return name;
		}
		Types::Language getLanguage()
		{
			return std::static_pointer_cast<Language>(mKeys[0])->get();
		}
		Types::Scale getScale()
		{
			return std::static_pointer_cast<Scale>(mKeys[1])->get();
		}
		Types::Toggle getFullscreen()
		{
			return std::static_pointer_cast<Fullscreen>(mKeys[2])->get();
		}
	};



	class Save final : public IIni
	{
	public:
		Save() : IIni({
			std::make_shared<Options>()
		}) { }
		Options& getOptions()
		{
			return *static_cast<Options*>(mSections[0].get());
		}
	};
} // namespace INI
} // namespace WhispseeySaveManager
