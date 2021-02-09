#pragma once

#include <cstdint>

namespace WhipseeySaveManager
{
namespace Data
{
	enum class Language : uint8_t
	{
		English = 0,
		Spanish = 1,
		French = 2,
		Italian = 3,
		German = 4,
		Russian = 5,
		Swedish = 6,
		Japanese = 7,
		Chinese = 8,
		Portogese = 9
	};

	enum class Scale : uint8_t
	{
		R768x432 = 2,
		R1152x648 = 3,
		R1536x864 = 4
	};

	enum class Screen : uint8_t
	{
		Windowed = 0,
		Fullscreen = 1
	};

	enum class Handed : uint8_t
	{
		Right = 0,
		Left = 1
	};

	enum class Volume : uint8_t
	{
		V0 = 0,
		V10 = 1,
		V20 = 2,
		V30 = 3,
		V40 = 4,
		V50 = 5,
		V60 = 6,
		V70 = 7,
		V80 = 8,
		V90 = 9,
		V100 = 10
	};

	enum class BossNoDamage : uint8_t
	{
		None = 0,
		BossA = 1,
		BossB = 2,
		BossAB = 3,
		BossC = 4,
		BossAC = 5,
		BossBC = 6,
		All = 7
	};//TODO find correct values

	enum class Level : uint8_t
	{
		Beach = 1,
		Forest = 2,
		Desert = 4,
		Snow = 8,
		Moon = 16,
		Castle = 32
	};

	struct Options
	{
		Language language = Language::English;
		Scale scale = Scale::R768x432;
		Screen screen = Screen::Fullscreen;
		Handed handed = Handed::Right;
		Volume sound = Volume::V100;
		bool soundToggle = true;
		Volume music = Volume::V100;
		bool musicToggle = true;
	};

	struct File
	{
		BossNoDamage noDamage = BossNoDamage::None;
		uint32_t defeated = 0;
		Level progress = Level::Beach;
		bool ending = false;
		bool intro = false;
		uint16_t lives = 5;
		uint8_t gems = 0;
	};

	struct Data
	{
		Options mOptions;
		File mFile1;
		File mFile2;
		File mFile3;
	};
	
} // namespace Data
} // namespace WhipseeySaveManager
