// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <cstdint>

// 던전 맵 설정 구조체
#pragma pack(push, 1)
struct DungeonMapConfig
{
	// 맵 크기 설정 (Field와 동일한 방식)
	uint8_t mGridColCountFactor = 5;  // 32x32 그리드 (1 << 5 = 32)
	uint8_t mGridRowCountFactor = 5;  // 32x32 그리드 (1 << 5 = 32)

	// 동굴 생성 설정
	int caveInitialFloorPercent = 35;   // 초기 개방 공간 비율 (0-100)
	int caveSmoothingPasses = 5;        // 엄격한 스무딩 반복 횟수
	int caveRelaxedPasses = 4;          // 완화된 스무딩 반복 횟수
	bool caveEnsureConnected = true;    // 가장 큰 동굴 영역만 유지

	// 시드 값
	uint64_t mRandomSeed = 0;

	// Helper methods to get actual dimensions
	GridCol GetWidth() const { return (1 << mGridColCountFactor); }
	GridRow GetHeight() const { return (1 << mGridRowCountFactor); }
};
#pragma pack(pop)
