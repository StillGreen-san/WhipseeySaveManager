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
{
	namespace Sections
	{
		namespace Cheats
		{
			namespace Keys
			{
				namespace cheats_enabled
				{
					constexpr std::string_view name = "cheats_enabled";
					using type = Types::Toggle;
				} // namespace cheats_enabled
			} // namespace Keys
		} // namespace Cheats
		namespace options
		{
			constexpr std::string_view name = "options";
			namespace Keys
			{
				namespace language
				{
					constexpr std::string_view name = "language";
					using type = Types::Language;
				} // namespace Language
				namespace scale
				{
					constexpr std::string_view name = "scale";
					using type = Types::Scale;
				} // namespace scale
				namespace fullscreen
				{
					constexpr std::string_view name = "fullscreen";
					using type = Types::Toggle;
				} // namespace fullscreen
				namespace left_handed
				{
					constexpr std::string_view name = "left_handed";
					using type = Types::Toggle;
				} // namespace left_handed
				namespace sound_volume
				{
					constexpr std::string_view name = "sound_volume";
					using type = Types::Volume;
				} // namespace sound_volume
				namespace sound_toggle
				{
					constexpr std::string_view name = "sound_toggle";
					using type = Types::Toggle;
				} // namespace sound_toggle
				namespace music_volume
				{
					constexpr std::string_view name = "music_volume";
					using type = Types::Volume;
				} // namespace music_volume
				namespace music_toggle
				{
					constexpr std::string_view name = "music_toggle";
					using type = Types::Toggle;
				} // namespace music_toggle
			} // namespace Keys
		} // namespace options
		namespace file1
		{
			namespace Keys
			{
				namespace boss_no_damage_progress
				{
					constexpr std::string_view name = "boss_no_damage_progress";
					using type = Types::BossNoDamage;
				} // namespace boss_no_damage_progress
				namespace castle
				{
					constexpr std::string_view name = "castle";
					using type = Types::Level;
				} // namespace castle
			} // namespace Keys
			
		} // namespace file1
		namespace file2
		{
			using namespace file1;
		} // namespace file2
		namespace file3
		{
			using namespace file1;
		} // namespace file3
	} // namespace Sections
	
	class INI
	{
	public:
		void linkError(const Types::Error&);
		void unlinkError();
		bool loadFile();
		bool hasSection();
		template<typename T>
		bool readKey();
		template<typename T>
		bool readKey(Types::FileIndex);
	private:
		void testfunc()
		{
			Sections::file2::Keys::boss_no_damage_progress::name;
		}
	};

	class IKey
	{
	public:
		virtual std::string_view key() const = 0;
		virtual Types::Error fromString(std::string_view /*value*/)
		{
			return validate(0);
		}//default for stringyfied float
		virtual std::string toString() const
		{
			return {};
		}//default for stringyfied float
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
		virtual std::string_view section() const = 0;
		virtual const std::vector<std::unique_ptr<IKey>>& keys()
		{
			return mKeys;
		}
		virtual ~ISection() = default;
	protected:
		std::vector<std::unique_ptr<IKey>> mKeys;
	};

	class IIni
	{
	public:
		virtual const std::vector<std::unique_ptr<ISection>>& sections()
		{
			return mSections;
		}
		virtual ~IIni() = default;
	protected:
		std::vector<std::unique_ptr<ISection>> mSections;
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
		Types::Error validate(float /*value*/) const override
		{
			return {};
		}
	};

	struct Options : ISection
	{
		static constexpr std::string_view name = "options";
		std::string_view section() const override
		{
			return name;
		}
		const std::vector<std::unique_ptr<IKey>>& keys() override
		{
			return {};
		}
	};

	struct Save : IIni
	{
		
	};

	int test()
	{
		std::unique_ptr<IKey> spk = std::make_unique<Language>();
		return spk->key().front();//sizeof(Language);//
		// Language::name;
	}
} // namespace INI
} // namespace WhispseeySaveManager
