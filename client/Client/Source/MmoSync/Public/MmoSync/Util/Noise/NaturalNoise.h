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
	class MMOSYNC_API NaturalNoise
	{
	private:
		int32_t mWidth = 0;
		int32_t mHeight = 0;

		std::vector<std::vector<uint8_t>> mLevelTable;
		
		// FractalNoise로부터 계산된 값들을 저장
		uint8_t mLevelDivisionSize = 0;

	public:
		DISABLE_COPY(NaturalNoise);
		explicit NaturalNoise(
			uint32_t w, uint32_t h, 
			LevelDivisionList&& levelDivisionList,
			FractalScaleList&& fractalScaleList,
			uint64_t randomSeed
		);

	public:
		uint8_t GetLevel(int32_t x, int32_t y) const;

		// FractalNoise로부터 계산된 값들을 반환하는 함수들
		uint8_t GetLevelDivisionSize() const { return mLevelDivisionSize; }
	};
}
