// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Random/RatioTable/RatioTable.h"
#include "MmoSync/Map/Types/AsciiMap.h"
#include "Common/PCH/pch_world.h"
#include <vector>
#include <deque>
#include <utility>

// ItemGrade는 다른 파일에 정의되어 있으므로 forward declaration 사용
enum class ItemGrade : int64_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Nest 위치 생성을 위한 클래스
// 둥지 위치만 생성하고, 엔티티 생성은 MapEntityFactory에서 담당
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FieldMapConfig;
struct DungeonMapConfig;
class MultiGrid;
struct GridCell;
class DeterministicRandom64;

class MMOSYNC_API NestPosGenerator
{
private:
	const MultiGrid& mGrid;
	std::deque<ColRowPair>& mPosList;
	std::vector<std::tuple<ColRowPair, ItemGrade>> mNestPositions; // Generated nest positions with ItemGrade

public:
	explicit NestPosGenerator(const MultiGrid& grid, std::deque<ColRowPair>& posList);

public:
	void InitNestPositions(int nestCount, const AsciiMap& asciiMap, uint64_t randomSeed);

	// 생성된 위치 정보 반환
	const std::vector<std::tuple<ColRowPair, ItemGrade>>& GetNestPositions() const { return mNestPositions; }

private:
	bool CheckNestPos(int c0, int r0, GridCoordinate minDistanceFromCastle);
};
