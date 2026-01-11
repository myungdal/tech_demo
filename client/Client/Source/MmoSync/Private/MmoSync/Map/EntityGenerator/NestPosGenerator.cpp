// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Map/EntityGeneration/NestPosGenerator.h"

#include "Common/Random/DeterministicRandom/DeterministicRandom64.h"
#include "Common/Random/RatioTable/RatioTable.h"

#include "MmoSync/Map/Configuration/FieldMapConfig.h"
#include "MmoSync/Map/Configuration/DungeonMapConfig.h"
#include "MmoSync/Grid/MultiGrid.h"
#include "MmoSync/Util/GridUtil/GridUtil.h"
#include "MmoSync/Util/Noise/NaturalNoise.h"
#include "MmoSync/Util/EntityUtil/EntityUtil.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"
#include "MmoSync/Map/GridGeneration/DungeonGridGenerator.h"
#include "MmoSync/Map/Visualization/AsciiConstants.h"

using namespace MmoSync::Map::TerrainLevel;


NestPosGenerator::NestPosGenerator(const MultiGrid& grid, std::deque<ColRowPair>& posList)
	: mGrid(grid), mPosList(posList)
{
	mNestPositions.clear();
}

// 캐슬 위치를 고려한 위치 생성
void NestPosGenerator::InitNestPositions(int nestCount, const AsciiMap& asciiMap, uint64_t randomSeed)
{
	// Clear existing positions
	mNestPositions.clear();
	
	// Get the no-nest zone distance from debug info
	const GridCoordinate DEFAULT_MAP_NO_NEST_ZONE_FROM_CASTLE = static_cast<GridCoordinate>(DebugInfo::DEFAULT_MAP_NO_NEST_ZONE_FROM_CASTLE);
	
	const int mapWidth = asciiMap.Width();
	const int mapHeight = asciiMap.Height();
	
	const int centerX = mapWidth >> 1;
	const int centerY = mapHeight >> 1;
	
	RatioTable<std::tuple<ColRowPair, ItemGrade>> ratioTable;
	
	// Create position ratio table based on terrain levels from fieldMap
	for (int r = DebugInfo::DEFAULT_MAP_NO_NEST_ZONE_FROM_EDGE; r < mapHeight - 1 - DebugInfo::DEFAULT_MAP_NO_NEST_ZONE_FROM_EDGE; ++r)
	{
		for (int c = DebugInfo::DEFAULT_MAP_NO_NEST_ZONE_FROM_EDGE; c < mapWidth - 1 - DebugInfo::DEFAULT_MAP_NO_NEST_ZONE_FROM_EDGE; ++c)
		{
			const int deltaFromCenterX = abs(centerX - c);
			const int deltaFromCenterY = abs(centerY - r);
			
			// 중심부 제외
			if (deltaFromCenterX <= DebugInfo::DEFAULT_MAP_NO_NEST_ZONE_FROM_CENTER && deltaFromCenterY <= DebugInfo::DEFAULT_MAP_NO_NEST_ZONE_FROM_CENTER)
				continue;
			
			if (false == CheckNestPos(c, r, DEFAULT_MAP_NO_NEST_ZONE_FROM_CASTLE))
				continue;
			
			const wchar_t tile = asciiMap.Get(c, r);
			ColRowPair cell = {static_cast<GridCol>(c), static_cast<GridRow>(r)};
			
			if (tile == LEVEL0)
			{
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(0)), 1);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(1)), 1);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(2)), 1);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(3)), 10);
			}
			else if (tile == LEVEL1)
			{
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(0)), 2);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(1)), 2);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(2)), 10);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(3)), 2);
			}
			else if (tile == LEVEL2)
			{
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(0)), 3);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(1)), 10);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(2)), 3);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(3)), 3);
			}
			else if (tile == LEVEL3)
			{
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(0)), 10);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(1)), 4);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(2)), 4);
				ratioTable.Add(std::make_tuple(cell, static_cast<ItemGrade>(3)), 4);
			}
		}
	}
	
	DeterministicRandom64 random(randomSeed);
	
	// 이미 선택된 nest 위치 추적 (중복 선택 방지)
	std::set<ColRowPair> usedPositions;
	
	for (int i = 0; i < nestCount; ++i)
	{
		std::tuple<ColRowPair, ItemGrade> picked;
		int attempts = 0;
		const int maxAttempts = 1000; // 충분한 시도 횟수
		bool found = false;
		
		// 사용되지 않은 위치를 찾을 때까지 반복
		while (attempts < maxAttempts && !found)
		{
			if (!ratioTable.Pick(picked, random))
				break; // ratioTable이 비어있으면 종료
			
			const ColRowPair cell = std::get<0>(picked);
			
			// 이미 사용된 위치인지 체크
			if (usedPositions.find(cell) != usedPositions.end())
			{
				attempts++;
				continue;
			}
			
			// 사용 가능한 위치 발견
			const ItemGrade grade = std::get<1>(picked);
			mNestPositions.push_back({cell, grade});
			usedPositions.insert(cell);
			found = true;
		}
		
		// maxAttempts 내에 사용 가능한 위치를 찾지 못하면 건너뛰기
		if (!found)
			continue;
	}
}

bool NestPosGenerator::CheckNestPos(int c0, int r0, GridCoordinate minDistanceFromCastle)
{
	const int64_t limitSq = MathUtil::Pow(int64_t(minDistanceFromCastle) << CELL_SIZE_FACTOR);

	for (auto [c1, r1] : mPosList)
	{
		const Scalar x = (Scalar(c0 - c1) << CELL_SIZE_FACTOR) + CELL_SIZE_HALF;
		const Scalar y = (Scalar(r0 - r1) << CELL_SIZE_FACTOR) + CELL_SIZE_HALF;
		const int64_t distanceSq = MathUtil::DistanceSq(x, y);

		if (distanceSq < limitSq)
			return false;
	}

	return true;
}

