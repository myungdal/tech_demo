// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Util/Noise/FractalNoiseDef.h"


#pragma pack(push, 1)
struct FieldMapConfig
{
	uint8_t mGridColCountFactor = 5;  // 32x32 그리드 (1 << 5 = 32)
	uint8_t mGridRowCountFactor = 5;  // 32x32 그리드 (1 << 5 = 32)

	//uint8_t mLevelDivision[7]{0, 40, 60, 70, 80, 120, 130};
	//Noise::FractalScale mFractalScale[4];

	uint64_t mRandomSeed = 0;

	GridCol GetWidth() const { return (1 << mGridColCountFactor); }
	GridRow GetHeight() const { return (1 << mGridRowCountFactor); }
};
#pragma pack(pop)
