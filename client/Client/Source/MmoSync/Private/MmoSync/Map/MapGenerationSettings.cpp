// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Map/Types/MapGenerationSettings.h"
#include "MmoSync/Map/GridGeneration/DungeonGridGenerator.h"
#include "MmoSync/Map/Configuration/DungeonMapConfig.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"
#include <random>

bool MapGenerationSettings::IsValid() const
{
	// 기본 검증
	if (mapConfig.GetSeed() == 0)
	{
		// 시드가 0이면 자동 생성이므로 유효
	}
	
	// 통합 MapConfig 검증
	if (!mapConfig.IsValid())
		return false;
	
	// 엔티티 설정 검증
	if (entitySettings.castleCount < 0 || entitySettings.nestCount < 0)
		return false;
	
	return true;
}

std::string MapGenerationSettings::GetValidationError() const
{
	if (GetMapKind() == MapKind::DUNGEON && mapConfig.IsDungeon())
	{
		const auto& dungeonConfig = mapConfig.GetDungeonConfig();
		const int width = dungeonConfig.GetWidth();
		const int height = dungeonConfig.GetHeight();
		if (width <= 0 || height <= 0)
			return "Dungeon width and height must be positive";
	}
	
	if (entitySettings.castleCount < 0)
		return "Castle count cannot be negative";
	if (entitySettings.nestCount < 0)
		return "Nest count cannot be negative";
	
	return "";
}
