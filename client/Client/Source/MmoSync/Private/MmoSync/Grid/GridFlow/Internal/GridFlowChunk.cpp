// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Grid/GridFlow/Internal/GridFlowChunk.h"

#include "Common/MemPool/FixedPool.h"

#include "MmoSync/Grid/GridFlow/GridFlowBuilder.h"
#include "MmoSync/Grid/MultiGrid.h"
#include "MmoSync/Util/GridUtil/GridUtil.h"


namespace
{
	using GridFlowDirMapInfoPool = FixedPool<GridFlowDirMapInfo, 14>;
	inline GridFlowDirMapInfoPool& GetFlowDirMapInfoPool()
	{
		static GridFlowDirMapInfoPool pool{};
		return pool;
	}
}

GridFlowChunk::GridFlowChunk(int64_t width, int64_t height, int64_t offsetU, int64_t offsetV)
	: 
	mWidth(width), 
	mHeight(height),
	mOffsetU(offsetU),
	mOffsetV(offsetV)
{
	//mGridBound = GridBound{
	//    .cMin = (GridCol)mOffsetU,
	//    .cMax = (GridCol)(mOffsetU + mWidth - 1),
	//    .rMin = (GridRow)mOffsetV,
	//    .rMax = (GridRow)(mOffsetV + mHeight - 1)
	//};

	mDirTable.resize(width * height, nullptr);

	const int64_t size = (width * height);
	mTaskMapDraft.resize(size);
	mTaskMap.resize(size);
}

GridFlowChunk::~GridFlowChunk()
{
	for (GridFlowDirMapInfo* dirMapInfo : mDirTable)
	{
		if (dirMapInfo)
			GetFlowDirMapInfoPool().Push(dirMapInfo);
	}
}

std::pair<GridFlowMode, GridFlowDir> GridFlowChunk::CalcFlowDir(Grid& grid, int64_t fromU, int64_t fromV, int64_t toU, int64_t toV)
{
	if (mDraftVersion != mVersion)
	{
		mDraftVersion = mVersion;

		grid.BuildGridFlow(*this);
	}

	fromU -= mOffsetU;
	fromV -= mOffsetV;
	toU -= mOffsetU;
	toV -= mOffsetV;

	const int64_t u0 = (fromU + mOffsetU);
	const int64_t v0 = (fromV + mOffsetV);
	const int64_t u1 = (toU + mOffsetU);
	const int64_t v1 = (toV + mOffsetV);

	const size_t toIndex = GetIndex(toU, toV);
	GridFlowDirMapInfo*& dirMap = mDirTable.at(toIndex);
	if (nullptr == dirMap)
	{
		dirMap = GetFlowDirMapInfoPool().Pop();

		dirMap->mVersion = mVersion;
		GridFlowBuilder builder(*this);
		builder.UpdateFlowField(u1, v1);
	}
	else if (dirMap->mVersion != mVersion)
	{
		dirMap->mVersion = mVersion;
		GridFlowBuilder builder(*this);
		builder.UpdateFlowField(u1, v1);
	}

	const size_t offset = GetIndex(fromU, fromV) * 5;
	const size_t bit0 = dirMap->mFlowDirMap.test(offset + 0) ? 1 : 0;
	const size_t bit1 = dirMap->mFlowDirMap.test(offset + 1) ? 1 : 0;
	GridFlowMode mode = static_cast<GridFlowMode>((bit1 << 1) | bit0);
	if (mode == GRID_FLOW_MODE_RAY_TEST_NEEDED)
	{
		const GridCol lastGridCol = (grid.GetColCount() - 1);
		const GridRow lastGridRow = (grid.GetRowCount() - 1);
		const bool isRayBlocked = GridUtil::IsRayBlocked(lastGridCol, lastGridRow, (GridCol)u0, (GridRow)v0, (GridCol)u1, (GridRow)v1, 0,
			[&grid, u0, v0, u1, v1](GridCol c, GridRow r) -> bool
			{
				if (c == u0 && r == v0)
					return false;

				if (c == u1 && r == v1)
					return false;

				return grid.IsBlocked(c, r);
			}
		);
		 
		mode = isRayBlocked ? GRID_FLOW_MODE_USE_FLOW_DIR : GRID_FLOW_MODE_DIRECT;
		dirMap->mFlowDirMap.set(offset + 0, mode & 0b01);
		dirMap->mFlowDirMap.set(offset + 1, mode & 0b10);
	}

	const size_t bit2 = dirMap->mFlowDirMap.test(offset + 2) ? 1 : 0;
	const size_t bit3 = dirMap->mFlowDirMap.test(offset + 3) ? 1 : 0;
	const size_t bit4 = dirMap->mFlowDirMap.test(offset + 4) ? 1 : 0;
	const GridFlowDir dir = static_cast<GridFlowDir>((bit4 << 2) | (bit3 << 1) | bit2);

	return std::make_pair(mode, dir);
}

std::pair<GridFlowMode, GridFlowDir> GridFlowChunk::GetFlowDir(int64_t fromU, int64_t fromV, int64_t toU, int64_t toV) const
{
	fromU -= mOffsetU;
	fromV -= mOffsetV;
	toU -= mOffsetU;
	toV -= mOffsetV;

	const size_t toIndex = GetIndex(toU, toV);
	const GridFlowDirMapInfo* dirMap = mDirTable.at(toIndex);
	if (nullptr == dirMap)
		return std::make_pair(GRID_FLOW_MODE_INACESSIBLE, GRID_FLOW_NONE);

	const size_t offset = GetIndex(fromU, fromV) * 5;
	const size_t bit0 = dirMap->mFlowDirMap.test(offset + 0) ? 1 : 0;
	const size_t bit1 = dirMap->mFlowDirMap.test(offset + 1) ? 1 : 0;
	const GridFlowMode mode = (GridFlowMode)((bit1 << 1) | bit0);

	const size_t bit2 = dirMap->mFlowDirMap.test(offset + 2) ? 1 : 0;
	const size_t bit3 = dirMap->mFlowDirMap.test(offset + 3) ? 1 : 0;
	const size_t bit4 = dirMap->mFlowDirMap.test(offset + 4) ? 1 : 0;
	const GridFlowDir dir = (GridFlowDir)((bit4 << 2) | (bit3 << 1) | bit2);

	return std::make_pair(mode, dir);
}

void GridFlowChunk::ClearFlowDirs(int64_t toU, int64_t toV)
{
	toU -= mOffsetU;
	toV -= mOffsetV;
	GridFlowDirMap& dirMap = mDirTable.at(GetIndex(toU, toV))->mFlowDirMap;
	dirMap.reset();
}

void GridFlowChunk::SetFlowDir(int64_t fromU, int64_t fromV, int64_t toU, int64_t toV, GridFlowDir dir)
{
	const size_t offset = GetIndex(fromU, fromV) * 5;

	GridFlowDirMap& dirMap = mDirTable.at(GetIndex(toU, toV))->mFlowDirMap;
	dirMap.set(offset + 0, GRID_FLOW_MODE_RAY_TEST_NEEDED & 0b01);
	dirMap.set(offset + 1, GRID_FLOW_MODE_RAY_TEST_NEEDED & 0b10);
	dirMap.set(offset + 2, dir & 0b001);
	dirMap.set(offset + 3, dir & 0b010);
	dirMap.set(offset + 4, dir & 0b100);
}
