#pragma once

#include "types.hpp"
#include <memory>
#include <charconv>
namespace WhipseeySaveManager
{
/**
 * @brief 
 * 
 */
namespace INI
{
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



	class Options final : public ISection
	{
	public:
		static constexpr std::string_view name = "options";
		Options() : ISection({
			std::make_shared<Language>(),
			std::make_shared<Language>()
		}) { }
		std::string_view section() const override
		{
			return name;
		}
	};



	class Save final : public IIni
	{
	public:
		Save() : IIni({
			std::make_shared<Options>(),
			std::make_shared<Options>()
		}) { }
	};

	int test()
	{
		std::shared_ptr<IIni> spi = std::make_shared<Save>();
		return spi->sections().front()->keys().front()->key().front();//sizeof(Language);//
		// Language::name;
	}

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
} // namespace INI
} // namespace WhispseeySaveManager
