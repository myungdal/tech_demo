// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Entity/Description/EntityDescription.h"
#include "MmoSync/Map/EntityGeneration/CastlePosGenerator.h"
#include "MmoSync/Map/EntityGeneration/NestPosGenerator.h"
#include "MmoSync/Map/Types/AsciiMap.h"
#include "Common/PCH/pch_world.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Map Entity 생성을 위한 통합 관리자 클래스
// 다양한 맵 엔티티 생성 방식을 통합하여 제공하는 중앙 인터페이스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MultiGrid;
struct FieldMapConfig;
struct DungeonMapConfig;

class MMOSYNC_API EntityGenerationManager
{
public:
	// 통합된 엔티티 생성 메서드들
	static EntityDescriptionList CreateCastles(const MultiGrid& grid, int castlePairCount, std::deque<ColRowPair>& posList, const AsciiMap& asciiMap, uint64_t randomSeed);
	static EntityDescriptionList CreateNestLeaders(const MultiGrid& grid, int nestCount, std::deque<ColRowPair>& posList, const AsciiMap& asciiMap, uint64_t randomSeed);
	static EntityDescriptionList CreateNestFollowers(int followerCount, Scalar centerX, Scalar centerY, ItemSid characterSid, EntityIdp leaderEntityIdp);
};
