// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Grid/GridFlow/GridFlowChunkManager.h"

#include "MmoSync/Grid/GridFlow/Internal/GridFlowChunk.h"


GridFlowChunkManager::GridFlowChunkManager(int64_t width, int64_t height)
	:
	mGridColCount(width),
	mGridRowCount(height)
{
	mChunkCellWidth = (mGridColCount >> GRID_FLOW_CHUNK_MAP_SIZE_FACTOR);
	mChunkCellHeight = (mGridRowCount >> GRID_FLOW_CHUNK_MAP_SIZE_FACTOR);
	const size_t chunkSize = mChunkCellWidth * mChunkCellHeight;

	mGridFlowChunkTable[0].resize(chunkSize, nullptr);
	mGridFlowChunkTable[1].resize(chunkSize, nullptr);
	mGridFlowChunkTable[2].resize(chunkSize, nullptr);
	mGridFlowChunkTable[3].resize(chunkSize, nullptr);

	for (int64_t mapV = 0; mapV < mChunkCellHeight; ++mapV)
	{
		for (int64_t mapU = 0; mapU < mChunkCellWidth; ++mapU)
		{
			for (int i = 0; i < 4; ++i)
			{
				const size_t index = (mapV * mChunkCellWidth + mapU);
				mGridFlowChunkTable[i].at(index) = _NEW<GridFlowChunk>(
					GRID_FLOW_CHUNK_MAP_SIZE,
					GRID_FLOW_CHUNK_MAP_SIZE,
					(GRID_FLOW_CHUNK_MAP_SIZE * mapU + GRID_FLOW_MAP_OFFSETS[i].first),
					(GRID_FLOW_CHUNK_MAP_SIZE * mapV + GRID_FLOW_MAP_OFFSETS[i].second)
				);
			}
		}
	}
}

GridFlowChunkManager::~GridFlowChunkManager()
{
	for (GridFlowChunkMap& map : mGridFlowChunkTable)
	{
		for (GridFlowChunk*& data : map)
		{
			if (data)
				_DELETE(data);
		}
	}
}

void GridFlowChunkManager::IncreaseVersion(int64_t u, int64_t v)
{
	u = std::clamp(u, 0LL, (mGridColCount - 1));
	v = std::clamp(v, 0LL, (mGridRowCount - 1));

	for (int i = 0; i < 4; ++i)
	{
		if (u < GRID_FLOW_MAP_OFFSETS[i].first)
			continue;

		if (v < GRID_FLOW_MAP_OFFSETS[i].second)
			continue;

		const int64_t mapU = ((u - GRID_FLOW_MAP_OFFSETS[i].first) >> GRID_FLOW_CHUNK_MAP_SIZE_FACTOR);
		const int64_t mapV = ((v - GRID_FLOW_MAP_OFFSETS[i].second) >> GRID_FLOW_CHUNK_MAP_SIZE_FACTOR);

		if (GridFlowChunk* data = mGridFlowChunkTable[i].at(mapV * mChunkCellWidth + mapU))
			data->IncreaseVersion();
	}
}

std::pair<GridFlowMode, GridFlowDir> GridFlowChunkManager::CalcFlowDir(Grid& grid, int64_t fromU, int64_t fromV, int64_t toU, int64_t toV, int tableIdx)
{
	const GridFlowMapOffset& offset = GRID_FLOW_MAP_OFFSETS[tableIdx];

	const int64_t fromMapU = ((fromU - offset.first) >> GRID_FLOW_CHUNK_MAP_SIZE_FACTOR);
	const int64_t toMapU = ((toU - offset.first) >> GRID_FLOW_CHUNK_MAP_SIZE_FACTOR);
	if (fromMapU < 0 || fromMapU != toMapU)
		return std::make_pair(GRID_FLOW_MODE_INACESSIBLE, GRID_FLOW_NONE);

	const int64_t fromMapV = ((fromV - offset.second) >> GRID_FLOW_CHUNK_MAP_SIZE_FACTOR);
	const int64_t toMapV = ((toV - offset.second) >> GRID_FLOW_CHUNK_MAP_SIZE_FACTOR);
	if (fromMapV < 0 || fromMapV != toMapV)
		return std::make_pair(GRID_FLOW_MODE_INACESSIBLE, GRID_FLOW_NONE);

	if (GridFlowChunk* data = mGridFlowChunkTable[tableIdx].at(fromMapV * mChunkCellWidth + fromMapU))
		return data->CalcFlowDir(grid, fromU, fromV, toU, toV);
	else
		return std::make_pair(GRID_FLOW_MODE_INACESSIBLE, GRID_FLOW_NONE);
}
