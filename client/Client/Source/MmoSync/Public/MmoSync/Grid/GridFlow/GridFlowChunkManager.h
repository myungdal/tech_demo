// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/GridFlow/Internal/GridFlowDef.h"


class GridFlowChunk;
class Grid;

class GridFlowChunkManager
{
private:
	int64_t mGridColCount = 0;
	int64_t mGridRowCount = 0;

	int64_t mChunkCellWidth = 0;
	int64_t mChunkCellHeight = 0;

private:
	using GridFlowChunkMap = std::vector<GridFlowChunk*>;
	using GridFlowChunkTable = std::array<GridFlowChunkMap, 4>;

	GridFlowChunkTable mGridFlowChunkTable;

public:
	explicit GridFlowChunkManager(int64_t gridCellWidth, int64_t gridCellHeight);
	virtual ~GridFlowChunkManager();

public:
	void IncreaseVersion(int64_t u, int64_t v);

public:
	std::pair<GridFlowMode, GridFlowDir> CalcFlowDir(Grid& grid, int64_t fromU, int64_t fromV, int64_t toU, int64_t toV, int tableIdx);
};
