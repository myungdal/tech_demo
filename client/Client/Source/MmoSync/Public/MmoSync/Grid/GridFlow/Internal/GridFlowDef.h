// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

using GridFlowCost = int64_t;
constexpr GridFlowCost GRID_FLOW_COST_STRAIT = 1024; // 직선 이동 비용
constexpr GridFlowCost GRID_FLOW_COST_DIAGONAL = 1448; // 대각선 이동 비용
constexpr GridFlowCost GRID_FLOW_COST_MAX = INT64_MAX;
//constexpr GridFlowCost GRID_FLOW_COST_AVOIDANCE = 0;

// 접근 가능 여부와 직진 가능한지 여부 (상위 2비트)
using GridFlowMode = uint8_t;
constexpr GridFlowMode GRID_FLOW_MODE_INACESSIBLE = 0; // 도달 불가
constexpr GridFlowMode GRID_FLOW_MODE_RAY_TEST_NEEDED = 1; // 우회해야하는지 테스트 필요
constexpr GridFlowMode GRID_FLOW_MODE_DIRECT = 2; // 직진 가능
constexpr GridFlowMode GRID_FLOW_MODE_USE_FLOW_DIR = 3; // 우회해야 함

// 흐름 방향 (하위 3비트)
using GridFlowDir = uint8_t;
constexpr GridFlowDir GRID_FLOW_DIR_8 = 0; // T
constexpr GridFlowDir GRID_FLOW_DIR_9 = 1; // RT 
constexpr GridFlowDir GRID_FLOW_DIR_6 = 2; // R
constexpr GridFlowDir GRID_FLOW_DIR_3 = 3; // RB
constexpr GridFlowDir GRID_FLOW_DIR_2 = 4; // B
constexpr GridFlowDir GRID_FLOW_DIR_1 = 5; // LB
constexpr GridFlowDir GRID_FLOW_DIR_4 = 6; // L
constexpr GridFlowDir GRID_FLOW_DIR_7 = 7; // LT
// 흐름 방향 없음
constexpr GridFlowDir GRID_FLOW_NONE = 8;

constexpr size_t GRID_FLOW_CHUNK_MAP_SIZE_FACTOR = 4;
constexpr size_t GRID_FLOW_CHUNK_MAP_SIZE = (1 << GRID_FLOW_CHUNK_MAP_SIZE_FACTOR);
constexpr size_t GRID_FLOW_CHUNK_MAP_SIZE_HALF = (GRID_FLOW_CHUNK_MAP_SIZE >> 1);
using GridFlowDirMap = std::bitset<GRID_FLOW_CHUNK_MAP_SIZE * GRID_FLOW_CHUNK_MAP_SIZE * (2 + 3)>; // 접근 가능 여부와 직진 가능한지 여부(2비트), 흐름 방향(3비트)

using GridFlowMapOffset = std::pair<int64_t, int64_t>;
using GridFlowMapOffsets = std::array<GridFlowMapOffset, 4>;
constexpr GridFlowMapOffsets GRID_FLOW_MAP_OFFSETS
{
	{
		{ 0, 0 },
		{ GRID_FLOW_CHUNK_MAP_SIZE_HALF, 0 },
		{ 0, GRID_FLOW_CHUNK_MAP_SIZE_HALF },
		{ GRID_FLOW_CHUNK_MAP_SIZE_HALF, GRID_FLOW_CHUNK_MAP_SIZE_HALF }
	}
};

struct GridFlowDirMapInfo
{
	GridFlowDirMap mFlowDirMap;
	uint64_t mVersion = 0;
};
using GridFlowDirTable = std::vector<GridFlowDirMapInfo*>; // 흐름 테이블

struct GridFlowTask
{
	GridFlowCost cost = 0;
	//GridFlowCost avoidanceCost = 0;
	bool visited = false;
};

using GridFlowTaskMap = std::vector<GridFlowTask>;
