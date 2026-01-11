// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Map/Configuration/MapConfig.h"
#include "Common/PCH/pch_world.h"
#include <vector>
#include <string>

class MultiGrid;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 통합 맵 생성 설정
// 필드와 던전 맵 생성을 위한 통합 인터페이스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MapKind는 MapKind.h에서 정의됨

// 맵 생성 결과 정보
struct MapGenerationResult
{
	bool success = false;
	std::string errorMessage;

	// 생성된 그리드 정보
	std::shared_ptr<MultiGrid> multiGrid;

	// 생성된 엔티티 정보
	std::vector<ColRowPair> castlePositions;
	std::vector<ColRowPair> nestPositions;

	// 생성 통계
	int32_t totalEntities = 0;
	int32_t castleCount = 0;
	int32_t nestCount = 0;

	// 사용된 설정
	uint64_t randomSeed = 0;

	// ASCII 맵 출력 (옵션)
	std::vector<std::wstring> asciiMap;
	bool hasAsciiMap = false;
};

// 엔티티 배치 설정
struct EntityPlacementSettings
{
	int32_t castleCount = 0;
	int32_t nestCount = 0;
};

// 통합 맵 생성 설정
struct MMOSYNC_API MapGenerationSettings
{

	// 통합 맵 설정
	MapConfig mapConfig;

	// 엔티티 배치 설정
	EntityPlacementSettings entitySettings;

	// 기본 생성자
	MapGenerationSettings() = default;


	// 검증 메서드
	bool IsValid() const;
	std::string GetValidationError() const;

	// 통합 MapConfig 접근자
	MapConfig& GetMapConfig() { return mapConfig; }
	const MapConfig& GetMapConfig() const { return mapConfig; }

public:
	// 편의 메서드 (MapConfig를 통해 접근)
	MapKind GetMapKind() const { return mapConfig.mapKind; }
	bool IsField() const { return mapConfig.IsField(); }
	bool IsDungeon() const { return mapConfig.IsDungeon(); }

	// MapKind 설정 편의 메서드
	void SetMapKind(MapKind kind) { mapConfig.mapKind = kind; }
};
