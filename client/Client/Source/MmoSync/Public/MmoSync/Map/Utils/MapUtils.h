// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <cstdint>
#include <cstdlib>
#include <utility>
#include "Common/Math/MathUtil.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Map 관련 공통 유틸리티 함수들
// 좌표 변환, 경계 검사, 거리 계산, 맵 검증 등의 공통 기능 제공
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace MmoSync::Map::Utils
{
	// 좌표 변환 유틸리티
	inline size_t ToIndex(int x, int y, int width)
	{
		return static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x);
	}

	inline std::pair<int, int> FromIndex(size_t index, int width)
	{
		return { static_cast<int>(index % width), static_cast<int>(index / width) };
	}

	// 경계 검사 유틸리티
	inline bool InBounds(int x, int y, int width, int height)
	{
		return x >= 0 && y >= 0 && x < width && y < height;
	}

	inline bool IsValidPosition(int x, int y, int width, int height)
	{
		return InBounds(x, y, width, height);
	}

	// 거리 계산 유틸리티
	inline int64_t CalculateDistanceSquared(int x1, int y1, int x2, int y2)
	{
		int64_t dx = static_cast<int64_t>(x2 - x1);
		int64_t dy = static_cast<int64_t>(y2 - y1);
		return dx * dx + dy * dy;
	}

	inline int CalculateManhattanDistance(int x1, int y1, int x2, int y2)
	{
		return std::abs(x2 - x1) + std::abs(y2 - y1);
	}

	// 맵 크기 검증 유틸리티
	inline bool ValidateMapSize(int width, int height)
	{
		return width > 0 && height > 0 && width <= 1024 && height <= 1024;
	}

	// 맵 설정 검증 유틸리티 (전방 선언)
	class MapConfig;
	bool ValidateMapConfig(const MapConfig& config);

	// ASCII 문자 변환 유틸리티 (전방 선언)
	wchar_t GetTerrainLevelChar(int level);
	int GetTerrainLevelFromChar(wchar_t ch);

	// 맵 통계 계산 유틸리티 (전방 선언)
	class MapStats;
	template<typename MapType>
	MapStats CalculateMapStats(const MapType& map);
}

