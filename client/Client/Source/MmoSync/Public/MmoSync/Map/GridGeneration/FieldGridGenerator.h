// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Map/Configuration/FieldMapConfig.h"
#include "MmoSync/Map/Configuration/MapConfig.h"
#include "MmoSync/Map/Types/AsciiMap.h"
#include "MmoSync/Grid/MultiGrid.h"
#include "MmoSync/Map/GridGeneration/GridGeneratorBase.h"
#include "MmoSync/Map/Visualization/AsciiConstants.h"
#include <vector>


using namespace MmoSync::Map;
using namespace MmoSync::Map::TerrainLevel;

struct MapGenerationSettings;
class Engine;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Perlin Noise를 이용한 자연 지형 맵 생성기
// ASCII 출력을 기본으로 하는 자연 지형 생성기
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MMOSYNC_API FieldGridGenerator : public GridGeneratorBase
{
private:
	MapConfig mMapConfig;

public:
	// Engine 없이 생성 (맵 생성에는 Engine 불필요)
	explicit FieldGridGenerator(const MapConfig& mapConfig);
	
	virtual ~FieldGridGenerator() = default;

public:
	
	// MapGeneratorBase 구현 (새로운 인터페이스)
	std::unique_ptr<AsciiMap>  CreateMultiGrid(OUT MultiGrid& multiGrid, const MapGenerationSettings& settings) override;
	bool IsConfigValid(const MapGenerationSettings& settings) const override;

private:	
	/// MultiGrid에 대해 Perlin Noise를 사용하여 자연스러운 지형을 생성합니다
	std::unique_ptr<AsciiMap> GenerateField(OUT MultiGrid& multiGrid, const MapGenerationSettings& settings);

	/// ASCII 필드 맵을 MultiGrid에 적용합니다
	void ApplyFieldMapToMultiGrid(OUT MultiGrid& multiGrid, const AsciiMap& fieldMap);

	/// 필드 맵 생성 (내부 구현)
	bool GenerateFieldMapInternal(AsciiMap& out, const MapGenerationSettings& settings);
	
	/// 필드 맵에 START와 EXIT 포인트 배치
	void PlaceFieldStartAndExit(AsciiMap& out, const FieldMapConfig& config);
	
	/// 필드 맵의 연결성 보장 - 작은 고립 영역 제거
	void EnsureFieldConnectivity(AsciiMap& out);
};
