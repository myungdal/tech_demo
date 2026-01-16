// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Util/Noise/FractalNoise.h"

#include "MmoSync/Util/Noise/PerlinNoise.h"


using namespace Noise;

FractalNoise::FractalNoise(
	uint32_t w, uint32_t h,
	LevelDivisionList&& levelDivisionList,
	FractalScaleList&& fractalScaleList
)
	:
	mWidth(w),
	mHeight(h),
	mLevelDivisionList(std::move(levelDivisionList)),
	mFractalScaleList(std::move(fractalScaleList))
{
	mLevelDivisionMin = static_cast<uint64_t>(*mLevelDivisionList.begin());
	mLevelDivisionMax = static_cast<uint64_t>(*mLevelDivisionList.rbegin());
	mLevelDivisionRange = mLevelDivisionMax - mLevelDivisionMin;

	mNoiseMin = UINT64_MAX;
	mNoiseMax = 0;

	const size_t levelDivisionCount = mLevelDivisionList.size();
	if (2 > levelDivisionCount)
	{
		mLevelDivisionList.clear();
		return;
	}

	for (uint32_t y = 0; y < (mHeight + 1); ++y)
	{
		for (uint32_t x = 0; x < (mWidth + 1); ++x)
		{
			uint16_t noise = NoiseDistorted(x, y);
			if (noise > mNoiseMax) mNoiseMax = noise;
			if (noise < mNoiseMin) mNoiseMin = noise;
		}
	}

	if (mNoiseMin == mNoiseMax)
		++mNoiseMax;

	mNoiseRange = mNoiseMax - mNoiseMin;
}

uint8_t FractalNoise::GetLevel(uint32_t x, uint32_t y) const
{
	uint64_t noise = static_cast<uint64_t>(NoiseDistorted(x, y)) - mNoiseMin;
	uint64_t level = ((((mLevelDivisionRange * noise) << 32) / mNoiseRange) >> 32) + mLevelDivisionMin;

	for (uint8_t i = 1; i < mLevelDivisionList.size(); ++i)
	{
		if (mLevelDivisionList[i] >= level)
			return static_cast<uint8_t>(i - 1);
	}

	return 0;
}

uint16_t FractalNoise::NoiseDistorted(uint32_t x, uint32_t y) const
{
	uint64_t totalNoise = 0;
	uint64_t totalWeight = 0;
	for (size_t i = 0; i < mFractalScaleList.size(); ++i)
	{
		const uint8_t& scale_x = mFractalScaleList[i].x;
		const uint8_t& scale_y = mFractalScaleList[i].y;
		const uint8_t& scale_w = mFractalScaleList[i].w;
		totalNoise += PerlinNoise::GetNoise(x << scale_x, y << scale_y) * scale_w;
		totalWeight += scale_w;
	}
	if (0 == totalWeight)
		totalWeight = 1;
	return static_cast<uint16_t>((((totalNoise) << 32) / totalWeight) >> 32);
}
