// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Map/Configuration/DungeonMapConfig.h"
#include "MmoSync/Map/Configuration/MapConfig.h"
#include "MmoSync/Map/Types/AsciiMap.h"
#include "MmoSync/Grid/MultiGrid.h"
#include "MmoSync/Map/GridGeneration/GridGeneratorBase.h"
#include "MmoSync/Map/Visualization/AsciiConstants.h"
#include <set>

#include "MmoSync/Map/Visualization/MapRegistry.h"
#include <cstdint>
#include <queue>
#include <vector>
#include <memory>


using namespace MmoSync::Map;
using namespace MmoSync::Map::TerrainLevel;

struct MapGenerationSettings;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cellular Automata를 이용한 던전/동굴 맵 생성기
// ASCII 출력을 기본으로 하는 맵 생성기
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Engine;
class AsciiMAp;

class MMOSYNC_API DungeonGridGenerator : public GridGeneratorBase
{
private:
	MapConfig mMapConfig;

public:
	explicit DungeonGridGenerator(const MapConfig& mapConfig);
	
	// Helper accessor
	const DungeonMapConfig& GetDungeonConfig() const { return mMapConfig.GetDungeonConfig(); }
	
	std::unique_ptr<AsciiMap> CreateMultiGrid(OUT MultiGrid& multiGrid, const MapGenerationSettings& settings) override;

	bool IsConfigValid(const MapGenerationSettings& settings) const override;

private:
	/// MultiGrid에 대해 Cellular Automata를 사용하여 던전/동굴을 생성합니다
	std::unique_ptr<AsciiMap> GenerateDungeon(OUT MultiGrid& multiGrid);

	/// ASCII 던전 맵을 MultiGrid에 적용합니다
	void ApplyDungeonMapToMultiGrid(OUT MultiGrid& multiGrid, const AsciiMap& dungeonMap);
	
	/// 시작점에서의 거리에 따라 TerrainLevel을 생성합니다
	void GenerateDistanceBasedTerrainLevels(AsciiMap& out, const std::vector<std::pair<int, int>>& startPositions, const std::pair<int, int>& exitPos);

	/// 가장자리로부터 거리에 따라 벽을 보간합니다
	void ApplyBorderInterpolation(AsciiMap& out);

	/// 던전 맵 생성 (내부 구현)
	bool GenerateDungeonMapInternal(AsciiMap& out);

	/// RNG 클래스
	struct Rng
	{
		uint64_t state;
		explicit Rng(uint64_t s) : state(s ? s : 0x9e3779b97f4a7c15ull) {}
		uint64_t Next()
		{
			// xorshift64*
			state ^= state >> 12; state ^= state << 25; state ^= state >> 27;
			return state * 2685821657736338717ull;
		}
		int NextRange(int minIncl, int maxIncl)
		{
			if (minIncl > maxIncl) std::swap(minIncl, maxIncl);
			uint64_t span = static_cast<uint64_t>(maxIncl - minIncl + 1);
			uint64_t v = Next();
			return minIncl + static_cast<int>(v % span);
		}
	};
};
