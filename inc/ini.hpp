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
		std::string_view key();
		Types::Error fromString(std::string_view string);
		std::string toString() const;
		bool operator==(const IKey& other) const
		{
			return mValue == other.mValue
				&& mLimits == other.mLimits
				&& mNumber == other.mNumber;
		}
	protected:
		const std::string_view mName;
		float mValue;
		enum class Limits : uint8_t
		{
			MinMax,
			EitherOr
		} const mLimits;
		enum class Number : uint8_t
		{
			StringFloat,
			StringInt,
			Int
		} const mNumber;
		explicit IKey(std::string_view name, float value, Limits limits, Number number) :
			mName{name}, mValue{value}, mLimits{limits}, mNumber{number}
		{}
	};

	class ISection
	{
	public:
		ISection(std::string_view name, std::initializer_list<std::shared_ptr<IKey>> il) :
			mName{name}, mKeys{il}
		{}
		std::string_view section();
		const std::vector<std::shared_ptr<IKey>>& keys()
		{
			return mKeys;
		}
	protected:
		const std::string_view mName;
		std::vector<std::shared_ptr<IKey>> mKeys;
	};

	class IIni
	{
	public:
		IIni(std::initializer_list<std::shared_ptr<ISection>> il) : mSections{il} {}
		const std::vector<std::shared_ptr<ISection>>& sections()
		{
			return mSections;
		}
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
		Language() : IKey(
			name,
			static_cast<float>(Types::Language::English),
			Limits::MinMax,
			Number::StringInt
		) { }
		Language& operator=(Types::Language value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Language()
		{
			return static_cast<Types::Language>(mValue);
		}
	private:
		static constexpr std::string_view name = "language";
	};

	class Scale final : public IKey
	{
	public:
		Scale() : IKey(
			name,
			static_cast<float>(Types::Scale::R768x432),
			Limits::MinMax,
			Number::StringInt
		) { }
		Scale& operator=(Types::Scale value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Scale()
		{
			return static_cast<Types::Scale>(mValue);
		}
	private:
		static constexpr std::string_view name = "scale";
	};

	class Fullscreen final : public IKey
	{
	public:
		Fullscreen() : IKey(
			name,
			static_cast<float>(Types::Fullscreen::Enabled),
			Limits::EitherOr,
			Number::StringInt
		) { }
		Fullscreen& operator=(Types::Fullscreen value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Fullscreen()
		{
			return static_cast<Types::Fullscreen>(mValue);
		}
	private:
		static constexpr std::string_view name = "fullscreen";
	};

	class LeftHanded final : public IKey
	{
	public:
		LeftHanded() : IKey(
			name,
			static_cast<float>(Types::LeftHanded::Enabled),
			Limits::EitherOr,
			Number::StringInt
		) { }
		LeftHanded& operator=(Types::LeftHanded value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::LeftHanded()
		{
			return static_cast<Types::LeftHanded>(mValue);
		}
	private:
		static constexpr std::string_view name = "left_handed";
	};



	class Options final : public ISection
	{
	public:
		Options() : ISection(
			name,
			{
				std::make_shared<Language>(),
				std::make_shared<Scale>(),
				std::make_shared<Fullscreen>()
			}
		) { }
		Language& getLanguage()
		{
			return *std::static_pointer_cast<Language>(mKeys[0]);
		}
		Scale& getScale()
		{
			return *std::static_pointer_cast<Scale>(mKeys[1]);
		}
		Fullscreen& getFullscreen()
		{
			return *std::static_pointer_cast<Fullscreen>(mKeys[2]);
		}
	private:
		static constexpr std::string_view name = "options";
	};



	class Save final : public IIni
	{
	public:
		Save() : IIni({
			std::make_shared<Options>()
		}) { }
		Options& getOptions()
		{
			return *std::static_pointer_cast<Options>(mSections[0]);
		}
	};
} // namespace INI
} // namespace WhispseeySaveManager
