// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


namespace Noise
{
	using LevelDivisionList = std::vector<uint8_t>;

#pragma pack(push, 1)
	struct FractalScale
	{
		uint8_t x = 0;
		uint8_t y = 0;
		uint8_t w = 0;
	};
#pragma pack(pop)

	using FractalScaleList = std::vector<FractalScale>;
}
