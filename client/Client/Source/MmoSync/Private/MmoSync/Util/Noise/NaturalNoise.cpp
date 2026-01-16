// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Util/Noise/NaturalNoise.h"

#include "Common/Random/DeterministicRandom/DeterministicRandom32.h"

#include "MmoSync/Util/Noise/FractalNoise.h"


using namespace Noise;

Noise::NaturalNoise::NaturalNoise(
	uint32_t w, uint32_t h, 
	LevelDivisionList&& levelDivisionList,
	FractalScaleList&& fractalScaleList,
	MAYBE_UNUSED uint64_t randomSeed
)
	:
	mWidth(w),
	mHeight(h)
{
	FractalNoise fractalNoise(w, h, std::move(levelDivisionList), std::move(fractalScaleList));

	// FractalNoise로부터 계산된 값들을 저장
	mLevelDivisionSize = fractalNoise.GetLevelDivisionSize();

	mLevelTable.resize(w + 1);
	for (uint32_t x = 0; x < (w + 1); ++x)
	{
		mLevelTable[x].resize(h + 1);
	}
	
	for (uint32_t y = 0; y < (h + 1); ++y)
	{
		for (uint32_t x = 0; x < (w + 1); ++x)
		{
			mLevelTable[x][y] = fractalNoise.GetLevel(x, y);
		}
	}

	// 물 만들기...
#if 0
	DeterministicRandom32 randomEngine(randomSeed);
	uint8_t waterCnt = static_cast<uint8_t>(randomEngine.Generate(1, 5));
	for (uint8_t i = 0 ; i < waterCnt; ++i)
	{
		int32_t x = randomEngine.Generate(0, w);
		int32_t y = randomEngine.Generate(0, h);
		int32_t power = randomEngine.Generate((w * h) >> 5, (w * h) >> 6);

		while (0 < --power)
		{
			uint8_t& level = GetLevel(x, y);
			if (0 < level)
				level = 0;

			uint8_t neighbor[8];
			neighbor[0] = GetLevel(x - 1, y - 1);
			neighbor[1] = GetLevel(x + 0, y - 1);
			neighbor[2] = GetLevel(x + 1, y - 1);
			neighbor[3] = GetLevel(x - 1, y + 0);
			neighbor[4] = GetLevel(x + 1, y + 0);
			neighbor[5] = GetLevel(x - 1, y + 1);
			neighbor[6] = GetLevel(x + 0, y + 1);
			neighbor[7] = GetLevel(x + 1, y + 1);

			size_t minIdx = SIZE_MAX;
			const int32_t randIdx = randomEngine.Generate(0, 7);
			for (int32_t offset = 0; offset < 8; ++offset)
			{
				int32_t idx = (randIdx + offset) & 7;
				if (0 == neighbor[idx]) continue;
				if ((SIZE_MAX != minIdx) && neighbor[minIdx] < neighbor[idx])
					continue;
				minIdx = idx;
			}
			if (SIZE_MAX == minIdx)
				minIdx = randIdx;

			switch(minIdx)
			{
			case 0: { x = x - 1; y = y - 1; } break;
			case 1: { x = x - 0; y = y - 1; } break;
			case 2: { x = x + 1; y = y - 1; } break;
			case 3: { x = x - 1; y = y + 0; } break;
			case 4: { x = x + 1; y = y + 0; } break;
			case 5: { x = x - 1; y = y + 1; } break;
			case 6: { x = x + 0; y = y + 1; } break;
			case 7: { x = x + 1; y = y + 1; } break;
			}

			for (int32_t offset = 0; offset < 8; ++offset)
			{
				if (0 == neighbor[offset]) continue;
				--neighbor[offset];
			}
		};
	}
#endif
}

uint8_t NaturalNoise::GetLevel(int32_t x, int32_t y) const
{
	if (0 > x) x = 0; else if (mWidth < x) x = mWidth;
	if (0 > y) y = 0; else if (mHeight < y) y = mHeight;
	return mLevelTable[x][y];
}
