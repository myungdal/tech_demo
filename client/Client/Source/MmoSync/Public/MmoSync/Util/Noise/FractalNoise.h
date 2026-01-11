// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Util/Noise/FractalNoiseDef.h"


namespace Noise
{
	class MMOSYNC_API FractalNoise
	{
	private:
		uint32_t mWidth = 0;
		uint32_t mHeight = 0;

		LevelDivisionList mLevelDivisionList;
		FractalScaleList mFractalScaleList;

		uint64_t mLevelDivisionMin = 0;
		uint64_t mLevelDivisionMax = 0;
		uint64_t mLevelDivisionRange = 0;

		uint64_t mNoiseMin = UINT16_MAX;
		uint64_t mNoiseMax = 0;
		uint64_t mNoiseRange = 0;

	public:
		DISABLE_COPY(FractalNoise);
		FractalNoise() = delete;
		explicit FractalNoise(
			uint32_t w, uint32_t h, 
			LevelDivisionList&& levelDivisionList, 
			FractalScaleList&& fractalScaleList
		);

	public:
		uint8_t GetLevel(uint32_t x, uint32_t y) const;

		// 크기 관련 Get 함수
		uint32_t GetWidth() const { return mWidth; }
		uint32_t GetHeight() const { return mHeight; }

		// Level Division 관련 Get 함수
		uint8_t GetLevelDivisionSize() const { return static_cast<uint8_t>(mLevelDivisionList.size()); }

	private:
		uint16_t NoiseDistorted(uint32_t x, uint32_t y) const;
	};
}
