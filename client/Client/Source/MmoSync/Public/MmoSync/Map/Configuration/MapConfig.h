// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Map/Configuration/FieldMapConfig.h"
#include "MmoSync/Map/Configuration/DungeonMapConfig.h"
#include <cstdint>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 통합 맵 설정 구조체
// FieldMapConfig와 DungeonMapConfig를 하나의 구조체로 통합
// 1바이트 정렬로 메모리 그대로 다른 OS에 전송 가능
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct MapConfig
{
public:
	MapKind mapKind = MapKind::FIELD;

private:
	// 각 맵 타입별 설정 (하나만 사용됨)
	union
	{
		FieldMapConfig fieldConfig;
		DungeonMapConfig dungeonConfig;
	};

public:
	MapConfig() : mapKind(MapKind::FIELD), fieldConfig() {}

	// FieldMapConfig 생성자
	explicit MapConfig(const FieldMapConfig& config)
		: mapKind(MapKind::FIELD), fieldConfig(config) {
	}

	// DungeonMapConfig 생성자
	explicit MapConfig(const DungeonMapConfig& config)
		: mapKind(MapKind::DUNGEON), dungeonConfig(config) {
	}

	// 유틸리티 메서드
	bool IsField() const { return mapKind == MapKind::FIELD; }
	bool IsDungeon() const { return mapKind == MapKind::DUNGEON; }

	// 설정 접근 함수
	const FieldMapConfig& GetFieldConfig() const { return fieldConfig; }
	const DungeonMapConfig& GetDungeonConfig() const { return dungeonConfig; }

	// 그리드 팩터 반환
	uint8_t GetColFactor() const
	{
		if (IsField())
		{
			return fieldConfig.mGridColCountFactor;
		}
		else if (IsDungeon())
		{
			return dungeonConfig.mGridColCountFactor;
		}
		return 0;
	}

	uint8_t GetRowFactor() const
	{
		if (IsField())
		{
			return fieldConfig.mGridRowCountFactor;
		}
		else if (IsDungeon())
		{
			return dungeonConfig.mGridRowCountFactor;
		}
		return 0;
	}

	// 그리드 크기 정보 추출 (공통 인터페이스)
	ColRowPair GetGridSize() const
	{
		if (IsField())
		{
			return { fieldConfig.GetWidth(), fieldConfig.GetHeight() };
		}
		else if (IsDungeon())
		{
			return { dungeonConfig.GetWidth(), dungeonConfig.GetHeight() };
		}
		return { GridCol(0), GridRow(0) };
	}

	// 시드 정보 추출 (공통 인터페이스)
	uint64_t GetSeed() const
	{
		if (IsField())
		{
			return fieldConfig.mRandomSeed;
		}
		else if (IsDungeon())
		{
			return dungeonConfig.mRandomSeed;
		}
		return 0;
	}

	// 유효성 검사
	bool IsValid() const
	{
		if (IsField())
		{
			return fieldConfig.mGridColCountFactor > 0 && fieldConfig.mGridRowCountFactor > 0;
		}
		else if (IsDungeon())
		{
			return dungeonConfig.mGridColCountFactor > 0 && dungeonConfig.mGridRowCountFactor > 0;
		}
		return false;
	}
};
#pragma pack(pop)

// GridConfig는 더 이상 사용되지 않음 - MapConfig를 직접 사용하세요
