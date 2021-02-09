#pragma once

#include <cstdint>

namespace WhipseeySaveManager
{
namespace Data
{
	enum class Toggle : bool
	{
		Disabled = false,
		Enabled = true
	};

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

	struct Noise
	{
		Volume volume = Volume::V100;
		Toggle toggle = Toggle::Enabled;
	};

	struct Options
	{
		Language language = Language::English;
		Scale scale = Scale::R768x432;
		Toggle fullScreen = Toggle::Enabled;
		Toggle leftHanded = Toggle::Disabled;
		Noise sound;
		Noise music;
	};

	struct File
	{
		BossNoDamage noDamage = BossNoDamage::None;
		uint32_t defeated = 0;
		Level progress = Level::Beach;
		Toggle ending = Toggle::Disabled;
		Toggle intro = Toggle::Disabled;
		uint16_t lives = 5;
		uint8_t gems = 0;
	};

	struct Save
	{
		Options mOptions;
		File mFile1;
		File mFile2;
		File mFile3;
	};

	struct Settings
	{
		Toggle cheats = Toggle::Disabled;
	};
	
} // namespace Data
} // namespace WhipseeySaveManager
