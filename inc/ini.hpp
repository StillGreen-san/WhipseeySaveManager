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
		virtual std::string_view key() = 0;
		virtual Types::Error fromString(std::string_view /*value*/)
		{
			return {};
		}//default for stringyfied float
		virtual std::string toString()
		{
			return {};
		}//default for stringyfied float
		bool operator==(const IKey& other)
		{
			return mValue == other.mValue;
		}
		virtual ~IKey() = default;
	protected:
		explicit IKey(float value) : mValue{value} {}
		float mValue;
	};
	struct ISection
	{
		virtual std::string_view section() = 0;
		virtual const std::vector<std::unique_ptr<IKey>>& keys() = 0;
		virtual ~ISection() = default;
		std::vector<std::unique_ptr<IKey>> mKeys;
	};
	struct IIni
	{
		virtual std::vector<std::unique_ptr<IKey>> keys() = 0;
		virtual std::vector<std::unique_ptr<ISection>> sections() = 0;
		virtual ~IIni() = default;
	};
	struct Language : public IKey
	{
		static constexpr std::string_view name = "language";
		enum Languages { TEMP1=0, TEMP2=1 };
		Language() : IKey(0.f) {}
		bool operator==(Languages arg){ return mValue == arg; }
		std::string_view key() override
		{
			return name;
		}
	};
	struct Scale : IKey
	{
		static constexpr std::string_view name = "scale";
		std::string_view key() override
		{
			return name;
		}
		Types::Error fromString(std::string_view) override
		{
			return {};
		}
		std::string toString() override
		{
			return "2.000000";
		}
	};
	struct Options : ISection
	{
		static constexpr std::string_view name = "options";
		std::string_view section() override
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
		
	}
} // namespace INI
} // namespace WhispseeySaveManager
