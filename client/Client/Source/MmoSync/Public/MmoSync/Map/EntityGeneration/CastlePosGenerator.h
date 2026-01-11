// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <vector>
#include <deque>
#include <utility>
#include "MmoSync/Map/Types/AsciiMap.h"
#include "Common/PCH/pch_world.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Castle 위치 생성을 위한 클래스
// 캐슬 위치만 생성하고, 엔티티 생성은 MapEntityFactory에서 담당
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Engine;
class MultiGrid;

class MMOSYNC_API CastlePosGenerator
{
private:
	const MultiGrid& mGrid;
	std::deque<ColRowPair>& mPosList;

	// 위치 정보 저장용
	std::vector<ColRowPair> mCastlePositions;

	// 생성된 경로들을 저장 (교차 검사용)
	std::vector<std::pair<ColRowPair, ColRowPair>> mCastlePaths;

public:
	explicit CastlePosGenerator(const MultiGrid& grid, std::deque<ColRowPair>& posList);

public:
	void InitCastlePositions(int castlePairCount, const AsciiMap& asciiMap, uint64_t randomSeed);

	// 생성된 위치 정보 반환
	const std::vector<ColRowPair>& GetCastlePositions() const { return mCastlePositions; }

private:
	// 레벨 검사
	bool IsLowLevelTerrain(GridCol col, GridRow row, const AsciiMap& asciiMap);

	// 경로 찾기 및 검증
	bool HasValidPath(GridCol fromCol, GridRow fromRow, GridCol toCol, GridRow toRow, const AsciiMap& asciiMap);
	bool PathsIntersect(
		const ColRowPair& path1Start, const ColRowPair& path1End,
		const ColRowPair& path2Start, const ColRowPair& path2End
	);

	// 캐슬 페어 검증
	bool IsValidCastlePair(GridCol c0, GridRow r0, GridCol c1, GridRow r1, const AsciiMap& asciiMap);
};
