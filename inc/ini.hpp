#pragma once

#include "types.hpp"

#include <memory>

//TODO remove magic numbers
//TODO provide namespaces for keys,sections inis ; hide INI
//TODO not using static_pointer_cast ?
//TODO comparison operator for IKey, ISection, IIni (update tests after)
//TODO tests?

namespace WhipseeySaveManager::INI
{
	class IKey
	{
	public:
		/**
		 * @return std::string_view with the key name
		 */
		std::string_view key() const
		{
			return mName;
		}

		/**
		 * @brief trys to parse the value from a raw format string
		 * 
		 * @param string the raw value in the expected format
		 * @return Types::Error if failed InvalidFormat, InvalidValue
		 */
		Types::Error fromString(std::string_view string);

		/**
		 * @return std::string the value in raw format
		 */
		std::string toString() const;

		/**
		 * @brief resets the value to its default
		 */
		void applyDefaults()
		{
			mValue = mDefault;
		}

		bool operator==(const IKey& other) const
		{
			return mValue == other.mValue
				&& mDefault == other.mDefault
				&& mMinOrA == other.mMinOrA
				&& mMaxOrB == other.mMaxOrB
				&& mLimits == other.mLimits
				&& mNumber == other.mNumber;
		}

		int asInt() const
		{
			return static_cast<int>(mValue);
		}
		float asFloat() const
		{
			return mValue;
		}
	protected:
		const float mMinOrA;
		const float mMaxOrB;
		const float mDefault;
		float mValue;
		const std::string_view mName;
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
		explicit IKey(std::string_view name, float value, Limits limits, Number number, float minOrA, float maxOrB) :
			mMinOrA{minOrA}, mMaxOrB{maxOrB}, mValue{value}, mDefault{value}, mName{name}, mLimits{limits}, mNumber{number}
		{}
	};

	class ISection
	{
	public:
		ISection(std::string_view name, std::initializer_list<std::shared_ptr<IKey>> il) :
			mName{name}, mKeys{il}
		{}

		/**
		 * @return std::string_view with the section name
		 */
		std::string_view section() const
		{
			return mName;
		}

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


	/**
	 * @brief internal helper object to load, check, read, write an ini file, 
	 * holds an Error onj containing all error that occured during operation
	 * 
	 */
	class INI
	{
	public:
		/**
		 * @brief extracts an error object containing all errors that occured when calling other funtions of this object
		 * 
		 * @return Types::Error see other functions for possible values
		 */
		Types::Error extractError();
		
		/**
		 * @brief trys to load the file at path, 
		 * adds FailedToLoadFile error code if failed
		 * 
		 * @return bool true when successful
		 */
		bool loadFile(const std::filesystem::path& path);
		
		/**
		 * @brief trys to write to the file at path, 
		 * adds FailedToWriteFile error code if failed
		 * 
		 * @return bool true when successful
		 */
		bool writeFile(const std::filesystem::path& path);
		
		/**
		 * @brief checks if a section is present in the loaded ini data, 
		 * adds SectionNotFound error code if failed
		 * 
		 * @return bool true when successful
		 */
		bool has(const std::shared_ptr<ISection>& section);
		
		/**
		 * @brief trys to read all keys in ISection from loaded ini data, 
		 * adds KeyNotFound, SectionNotFound error codes if failed
		 * 
		 * @return bool true when successful
		 */
		bool read(const std::shared_ptr<ISection>& section);
		
		/**
		 * @brief trys to read all keys in IIni from loaded ini data, 
		 * adds KeyNotFound, SectionNotFound error codes if failed
		 * 
		 * @return bool true when successful
		 */
		bool read(const std::shared_ptr<IIni>& ini);
		
		/**
		 * @brief writes all keys in ISection to loaded ini data
		 */
		void write(const std::shared_ptr<ISection>& section);
		
		/**
		 * @brief writs all keys in IIni to loaded ini data
		 */
		void write(const std::shared_ptr<IIni>& ini);

		INI();
		~INI();
	private:
		Types::Error mError;
		class INIintern;
		std::unique_ptr<INIintern> mIni;
	};



	class Language final : public IKey
	{
	public:
		Language() : IKey(
			name,
			static_cast<float>(Types::Language::English),
			Limits::MinMax,
			Number::StringInt,
			static_cast<float>(Types::Language::English),
			static_cast<float>(Types::Language::Portogese)
		) { }
		Language& operator=(Types::Language value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Language() const
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
			Number::StringInt,
			static_cast<float>(Types::Scale::R768x432),
			static_cast<float>(Types::Scale::R1536x864)
		) { }
		Scale& operator=(Types::Scale value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Scale() const
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
			Number::StringInt,
			static_cast<float>(Types::Fullscreen::Disabled),
			static_cast<float>(Types::Fullscreen::Enabled)
		) { }
		Fullscreen& operator=(Types::Fullscreen value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Fullscreen() const
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
			Number::StringInt,
			static_cast<float>(Types::LeftHanded::Disabled),
			static_cast<float>(Types::LeftHanded::Enabled)
		) { }
		LeftHanded& operator=(Types::LeftHanded value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::LeftHanded() const
		{
			return static_cast<Types::LeftHanded>(mValue);
		}
	private:
		static constexpr std::string_view name = "left_handed";
	};

	class SoundVolume final : public IKey
	{
	public:
		SoundVolume() : IKey(
			name,
			static_cast<float>(Types::SoundVolume::V100),
			Limits::MinMax,
			Number::StringFloat,
			static_cast<float>(Types::SoundVolume::V0),
			static_cast<float>(Types::SoundVolume::V100)
		) { }
		SoundVolume& operator=(Types::SoundVolume value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::SoundVolume() const
		{
			return static_cast<Types::SoundVolume>(mValue * 10);
		}
		int asInt()
		{
			return static_cast<int>(mValue * 10);
		}
	private:
		static constexpr std::string_view name = "sound_volume";
	};

	class SoundToggle final : public IKey
	{
	public:
		SoundToggle() : IKey(
			name,
			static_cast<float>(Types::SoundToggle::Enabled),
			Limits::EitherOr,
			Number::StringInt,
			static_cast<float>(Types::SoundToggle::Disabled),
			static_cast<float>(Types::SoundToggle::Enabled)
		) { }
		SoundToggle& operator=(Types::SoundToggle value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::SoundToggle() const
		{
			return static_cast<Types::SoundToggle>(mValue);
		}
	private:
		static constexpr std::string_view name = "sound_toggle";
	};

	class MusicVolume final : public IKey
	{
	public:
		MusicVolume() : IKey(
			name,
			static_cast<float>(Types::MusicVolume::V100),
			Limits::MinMax,
			Number::StringFloat,
			static_cast<float>(Types::MusicVolume::V0),
			static_cast<float>(Types::MusicVolume::V100)
		) { }
		MusicVolume& operator=(Types::MusicVolume value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::MusicVolume() const
		{
			return static_cast<Types::MusicVolume>(mValue * 10);
		}
		int asInt()
		{
			return static_cast<int>(mValue * 10);
		}
	private:
		static constexpr std::string_view name = "music_volume";
	};

	class MusicToggle final : public IKey
	{
	public:
		MusicToggle() : IKey(
			name,
			static_cast<float>(Types::MusicToggle::Enabled),
			Limits::EitherOr,
			Number::StringInt,
			static_cast<float>(Types::MusicToggle::Disabled),
			static_cast<float>(Types::MusicToggle::Enabled)
		) { }
		MusicToggle& operator=(Types::MusicToggle value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::MusicToggle() const
		{
			return static_cast<Types::MusicToggle>(mValue);
		}
	private:
		static constexpr std::string_view name = "music_toggle";
	};

	class Options final : public ISection
	{
	public:
		Options() : ISection(
			name,
			{
				std::make_shared<Language>(),
				std::make_shared<Scale>(),
				std::make_shared<Fullscreen>(),
				std::make_shared<LeftHanded>(),
				std::make_shared<SoundVolume>(),
				std::make_shared<SoundToggle>(),
				std::make_shared<MusicVolume>(),
				std::make_shared<MusicToggle>()
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
		LeftHanded& getLeftHanded()
		{
			return *std::static_pointer_cast<LeftHanded>(mKeys[3]);
		}
		SoundVolume& getSoundVolume()
		{
			return *std::static_pointer_cast<SoundVolume>(mKeys[4]);
		}
		SoundToggle& getSoundToggle()
		{
			return *std::static_pointer_cast<SoundToggle>(mKeys[5]);
		}
		MusicVolume& getMusicVolume()
		{
			return *std::static_pointer_cast<MusicVolume>(mKeys[6]);
		}
		MusicToggle& getMusicToggle()
		{
			return *std::static_pointer_cast<MusicToggle>(mKeys[7]);
		}
	private:
		static constexpr std::string_view name = "options";
	};



	class BossNoDamageProgress final : public IKey
	{
	public:
		BossNoDamageProgress() : IKey(
			name,
			static_cast<float>(Types::BossNoDamage::None),
			Limits::MinMax,
			Number::StringInt,
			static_cast<float>(Types::BossNoDamage::None),
			static_cast<float>(Types::BossNoDamage::All)
		) { }
		BossNoDamageProgress& operator=(Types::BossNoDamage value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::BossNoDamage() const
		{
			return static_cast<Types::BossNoDamage>(mValue);
		}
	private:
		static constexpr std::string_view name = "boss_no_damage_progress";
	};

	class EnemiesDefeated final : public IKey
	{
	public:
		EnemiesDefeated() : IKey(
			name,
			0,
			Limits::MinMax,
			Number::StringInt,
			0,
			1677215
		) { }
		EnemiesDefeated& operator=(Types::Scale value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator uint32_t()
		{
			return static_cast<uint32_t>(mValue);
		}
	private:
		static constexpr std::string_view name = "enemies_defeated";
	};

	class Castle final : public IKey
	{
	public:
		Castle() : IKey(
			name,
			static_cast<float>(Types::Castle::Locked),
			Limits::EitherOr,
			Number::StringInt,
			static_cast<float>(Types::Castle::Locked),
			static_cast<float>(Types::Castle::Unlocked)
		) { }
		Castle& operator=(Types::Castle value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Castle() const
		{
			return static_cast<Types::Castle>(mValue);
		}
	private:
		static constexpr std::string_view name = "castle";
	};

	class Moon final : public IKey
	{
	public:
		Moon() : IKey(
			name,
			static_cast<float>(Types::Moon::Locked),
			Limits::EitherOr,
			Number::StringInt,
			static_cast<float>(Types::Moon::Locked),
			static_cast<float>(Types::Moon::Unlocked)
		) { }
		Moon& operator=(Types::Moon value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Moon() const
		{
			return static_cast<Types::Moon>(mValue);
		}
	private:
		static constexpr std::string_view name = "moon";
	};

	class Snow final : public IKey
	{
	public:
		Snow() : IKey(
			name,
			static_cast<float>(Types::Snow::Locked),
			Limits::EitherOr,
			Number::StringInt,
			static_cast<float>(Types::Snow::Locked),
			static_cast<float>(Types::Snow::Unlocked)
		) { }
		Snow& operator=(Types::Snow value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Snow() const
		{
			return static_cast<Types::Snow>(mValue);
		}
	private:
		static constexpr std::string_view name = "snow";
	};

	class Desert final : public IKey
	{
	public:
		Desert() : IKey(
			name,
			static_cast<float>(Types::Desert::Locked),
			Limits::EitherOr,
			Number::StringInt,
			static_cast<float>(Types::Desert::Locked),
			static_cast<float>(Types::Desert::Unlocked)
		) { }
		Desert& operator=(Types::Desert value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Desert() const
		{
			return static_cast<Types::Desert>(mValue);
		}
	private:
		static constexpr std::string_view name = "desert";
	};

	class Forest final : public IKey
	{
	public:
		Forest() : IKey(
			name,
			static_cast<float>(Types::Forest::Locked),
			Limits::EitherOr,
			Number::StringInt,
			static_cast<float>(Types::Forest::Locked),
			static_cast<float>(Types::Forest::Unlocked)
		) { }
		Forest& operator=(Types::Forest value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Forest() const
		{
			return static_cast<Types::Forest>(mValue);
		}
	private:
		static constexpr std::string_view name = "forest";
	};

	class Ending final : public IKey
	{
	public:
		Ending() : IKey(
			name,
			static_cast<float>(Types::Ending::Unwatched),
			Limits::EitherOr,
			Number::StringInt,
			static_cast<float>(Types::Ending::Unwatched),
			static_cast<float>(Types::Ending::Watched)
		) { }
		Ending& operator=(Types::Ending value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Ending() const
		{
			return static_cast<Types::Ending>(mValue);
		}
	private:
		static constexpr std::string_view name = "ending";
	};

	class Intro final : public IKey
	{
	public:
		Intro() : IKey(
			name,
			static_cast<float>(Types::Intro::Unwatched),
			Limits::EitherOr,
			Number::StringInt,
			static_cast<float>(Types::Intro::Unwatched),
			static_cast<float>(Types::Intro::Watched)
		) { }
		Intro& operator=(Types::Intro value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Intro() const
		{
			return static_cast<Types::Intro>(mValue);
		}
	private:
		static constexpr std::string_view name = "intro";
	};

	class Lives final : public IKey
	{
	public:
		Lives() : IKey(
			name,
			5,
			Limits::MinMax,
			Number::StringFloat,
			1,
			16777215
		) { }
		Lives& operator=(Types::Lives value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Lives()
		{
			return static_cast<Types::Lives>(mValue);
		}
	private:
		static constexpr std::string_view name = "lives";
	};

	class Gems final : public IKey
	{
	public:
		Gems() : IKey(
			name,
			0,
			Limits::MinMax,
			Number::StringInt,
			0,
			99
		) { }
		Gems& operator=(Types::Gems value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::Gems()
		{
			return static_cast<Types::Gems>(mValue);
		}
	private:
		static constexpr std::string_view name = "gems";
	};

	class FileBase : public ISection
	{
	protected:
		explicit FileBase(std::string_view name) : ISection(
			name,
			{
				std::make_shared<BossNoDamageProgress>(),
				std::make_shared<EnemiesDefeated>(),
				std::make_shared<Castle>(),
				std::make_shared<Moon>(),
				std::make_shared<Snow>(),
				std::make_shared<Desert>(),
				std::make_shared<Forest>(),
				std::make_shared<Ending>(),
				std::make_shared<Intro>(),
				std::make_shared<Lives>(),
				std::make_shared<Gems>()
			}
		) { }
	public:
		BossNoDamageProgress& getBossNoDamageProgress()
		{
			return *std::static_pointer_cast<BossNoDamageProgress>(mKeys[0]);
		}
		EnemiesDefeated& getEnemiesDefeated()
		{
			return *std::static_pointer_cast<EnemiesDefeated>(mKeys[1]);
		}
		Castle& getCastle()
		{
			return *std::static_pointer_cast<Castle>(mKeys[2]);
		}
		Moon& getMoon()
		{
			return *std::static_pointer_cast<Moon>(mKeys[3]);
		}
		Snow& getSnow()
		{
			return *std::static_pointer_cast<Snow>(mKeys[4]);
		}
		Desert& getDesert()
		{
			return *std::static_pointer_cast<Desert>(mKeys[5]);
		}
		Forest& getForest()
		{
			return *std::static_pointer_cast<Forest>(mKeys[6]);
		}
		Types::Level getLevel();
		void setLevel(Types::Level level);
		Ending& getEnding()
		{
			return *std::static_pointer_cast<Ending>(mKeys[7]);
		}
		Intro& getIntro()
		{
			return *std::static_pointer_cast<Intro>(mKeys[8]);
		}
		Lives& getLives()
		{
			return *std::static_pointer_cast<Lives>(mKeys[9]);
		}
		Gems& getGems()
		{
			return *std::static_pointer_cast<Gems>(mKeys[10]);
		}
	};

	class File1 final : public FileBase
	{
	public:
		File1() : FileBase(name) {}
	private:
		static constexpr std::string_view name = "file1";
	};

	class File2 final : public FileBase
	{
	public:
		File2() : FileBase(name) {}
	private:
		static constexpr std::string_view name = "file2";
	};

	class File3 final : public FileBase
	{
	public:
		File3() : FileBase(name) {}
	private:
		static constexpr std::string_view name = "file3";
	};



	class Save final : public IIni
	{
	public:
		Save() : IIni({
			std::make_shared<Options>(),
			std::make_shared<File1>(),
			std::make_shared<File2>(),
			std::make_shared<File3>()
		}) { }
		std::shared_ptr<Options> getOptions()
		{
			return std::static_pointer_cast<Options>(mSections[0]);
		}
		std::shared_ptr<File1> getFile1()
		{
			return std::static_pointer_cast<File1>(mSections[1]);
		}
		std::shared_ptr<File2> getFile2()
		{
			return std::static_pointer_cast<File2>(mSections[2]);
		}
		std::shared_ptr<File3> getFile3()
		{
			return std::static_pointer_cast<File3>(mSections[3]);
		}
	};



	class CheatsEnabled final : public IKey
	{
	public:
		CheatsEnabled() : IKey(
			name,
			static_cast<float>(Types::CheatsEnabled::Disabled),
			Limits::EitherOr,
			Number::Int,
			static_cast<float>(Types::CheatsEnabled::Disabled),
			static_cast<float>(Types::CheatsEnabled::Enabled)
		) {}
		CheatsEnabled& operator=(Types::CheatsEnabled value)
		{
			mValue = static_cast<float>(value);
			return *this;
		}
		operator Types::CheatsEnabled() const
		{
			return static_cast<Types::CheatsEnabled>(mValue);
		}
	private:
		static constexpr std::string_view name = "cheats_enabled";
	};



	class Cheats final : public ISection
	{
	public:
		Cheats() : ISection(
			name,
			{
				std::make_shared<CheatsEnabled>()
			}
		) {}
		CheatsEnabled& getCheatsEnabled()
		{
			return *std::static_pointer_cast<CheatsEnabled>(mKeys[0]);
		}
	private:
		static constexpr std::string_view name = "Cheats";
	};



	class Settings final : public IIni
	{
	public:
		Settings() : IIni(
			{
				std::make_shared<Cheats>()
			}
		) {}
		Cheats& getCheats()
		{
			return *std::static_pointer_cast<Cheats>(mSections[0]);
		}
	};
} // namespace WhispseeySaveManager::INI
