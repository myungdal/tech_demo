// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Grid/Internal/Grid.h"

#include "MmoSync/Grid/GridFlow/GridFlowBuilder.h"
#include "MmoSync/Grid/GridFlow/Internal/GridFlowChunk.h"
#include "MmoSync/Grid/GridFlow/GridFlowChunkManager.h"
#include "MmoSync/Grid/AStarSearch/AStarSearch.h"
#include "MmoSync/Grid/GridLod/GridLod.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"
#include "MmoSync/Util/GridUtil/GridUtil.h"


Grid::Grid(uint8_t colCountFactor, uint8_t rowCountFactor)
	:
	mGridCellData(colCountFactor, rowCountFactor),
	mGridFlowChunkManager(std::make_unique<GridFlowChunkManager>(mGridCellData.mColCount, mGridCellData.mRowCount)),
	mAStarSearch(std::make_unique<AStarSearch>()),
	mGridLod(std::make_unique<GridLod>())
{
}
Grid::~Grid()
{
}
void Grid::BuildGridLod()
{
	mGridLod->Build(static_cast<uint32_t>(mGridCellData.mColCount), static_cast<uint32_t>(mGridCellData.mRowCount),
		[this](uint32_t u, uint32_t v)->bool
		{
			return this->IsBlocked(static_cast<GridCol>(u), static_cast<GridRow>(v));
		}
	);
}

void Grid::BuildGridFlow(GridFlowChunk& gridFlowChunk)
{
	GridFlowBuilder gridFlowBuilder(gridFlowChunk);
	gridFlowBuilder.InitializeTaskMap(
		[this](int64_t u, int64_t v)->bool
		{
			return this->IsBlocked(static_cast<GridCol>(u), static_cast<GridRow>(v));
		}
	);
}

bool Grid::IsValid(GridCol c, GridRow r) const
{
	if (c < 0) return false;
	if (r < 0) return false;
	if (c >= mGridCellData.mColCount) return false;
	if (r >= mGridCellData.mRowCount) return false;
	return true;
}

bool Grid::IsBlocked(GridCol c, GridRow r) const
{
	if (!IsValid(c, r))
		return true;

	const size_t cellIndex = GetCellIndex(c, r);
	const bool isBlocked = mGridCellData.mCellBlockTable.test(cellIndex);
	return isBlocked;
}

void Grid::SetBlocked(GridCol c, GridRow r)
{
	if (!IsValid(c, r))
		return;

	if (IsBlocked(c, r))
		return;

	const size_t cellIndex = GetCellIndex(c, r);
	mGridCellData.mCellBlockTable.set(cellIndex, true);
	mGridFlowChunkManager->IncreaseVersion(c, r);
}

void Grid::SetUnblocked(GridCol c, GridRow r)
{
	if (!IsValid(c, r))
		return;

	if (!IsBlocked(c, r))
		return;

	const size_t cellIndex = GetCellIndex(c, r);
	mGridCellData.mCellBlockTable.set(cellIndex, false);
	mGridFlowChunkManager->IncreaseVersion(c, r);
}

ColRowPair Grid::GetExitCell(GridCol c, GridRow r, GridCellDir firstDir) const
{
	if (!IsValid(c, r))
		return GetCell(c, r);

	size_t step = 0;

	while (IsBlocked(c, r))
	{
		do
		{
			size_t dist = 0;
			while ((dist + 1) * (dist + 2) <= step)
				++dist;

			static constexpr GridCoordinate colDelta[4] = { 0,  1,  0, -1 }, rowDelta[4] = { -1,  0,  1,  0 };

			const size_t boundary = (dist + 1) * (dist + 1);
			size_t idx = (step < boundary) ? (2 * dist) : (2 * dist + 1);
			idx = (idx + firstDir) & 3;
			c += colDelta[idx];
			r += rowDelta[idx];
			++step;

		} while (!IsValid(c, r));
	}

	return GetCell(c, r);
}

ColRowPair Grid::GetNextCell(ColRowPair& cell, GridCellDir dir) const
{
	switch (dir)
	{
	case GRID_CELL_DIR_8:
		return GetCell(cell.first, cell.second - 1);
	case GRID_CELL_DIR_6:
		return GetCell(cell.first + 1, cell.second);
	case GRID_CELL_DIR_2:
		return GetCell(cell.first, cell.second + 1);
	case GRID_CELL_DIR_4:
		return GetCell(cell.first - 1, cell.second);
	default:
		return GetCell(cell.first, cell.second);
	}
}

std::pair<GridFlowMode, GridFlowDir> Grid::CalcFlowDir(int64_t fromU, int64_t fromV, int64_t toU, int64_t toV)
{
	fromU = MathUtil::Clamp(fromU, static_cast<int64_t>(0), static_cast<int64_t>(mGridCellData.mColCount - 1));
	fromV = MathUtil::Clamp(fromV, static_cast<int64_t>(0), static_cast<int64_t>(mGridCellData.mRowCount - 1));
	toU = MathUtil::Clamp(toU, static_cast<int64_t>(0), static_cast<int64_t>(mGridCellData.mColCount - 1));
	toV = MathUtil::Clamp(toV, static_cast<int64_t>(0), static_cast<int64_t>(mGridCellData.mRowCount - 1));

	if ((fromU == toU) && (fromV == toV))
		return std::make_pair(GRID_FLOW_MODE_DIRECT, GRID_FLOW_NONE);

	for (int tableIdx = 0; tableIdx < 4; ++tableIdx)
	{
		std::pair<GridFlowMode, GridFlowDir> result = mGridFlowChunkManager->CalcFlowDir(*this, fromU, fromV, toU, toV, tableIdx);
		if (result.first != GRID_FLOW_MODE_INACESSIBLE)
			return result;
	}

	return std::make_pair(GRID_FLOW_MODE_INACESSIBLE, GRID_FLOW_NONE);
}

std::pair<GridFlowMode, GridFlowDir> Grid::CalcFlowDirByPos(Scalar fromX, Scalar fromY, Scalar toX, Scalar toY)
{
	const int64_t fromU = GridUtil::ConvToCol(fromX);
	const int64_t fromV = GridUtil::ConvToRow(fromY);
	const int64_t toU = GridUtil::ConvToCol(toX);
	const int64_t toV = GridUtil::ConvToRow(toY);

	std::pair<GridFlowMode, GridFlowDir> flowDir = CalcFlowDir(fromU, fromV, toU, toV);
	return flowDir;
}

ColRowPair Grid::GetCell(GridCol c, GridRow r) const
{
	c = MathUtil::Clamp(c, static_cast<GridCol>(0), static_cast<GridCol>(mGridCellData.mColCount - 1));
	r = MathUtil::Clamp(r, static_cast<GridRow>(0), static_cast<GridRow>(mGridCellData.mRowCount - 1));
	return ColRowPair{ c, r };
}

ColRowPair Grid::PickCell(Scalar x, Scalar y) const
{
	const GridCol c = GridUtil::ConvToCol(x);
	const GridRow r = GridUtil::ConvToRow(y);
	ColRowPair cell = GetCell(c, r);
	return cell;
}

void Grid::CalcAStarSearch(const ColRowPair& from, const ColRowPair& to, int limit, OUT Scalar& resultX, OUT Scalar& resultY)
{
	GridCol fromCol = from.first;
	GridRow fromRow = from.second;
	GridCol toCol = to.first;
	GridRow toRow = to.second;

	auto [flowMode, flowDir] = CalcFlowDir(fromCol, fromRow, toCol, toRow);
	if (flowMode != GRID_FLOW_MODE_INACESSIBLE)
		return;

	constexpr int64_t base = GRID_FLOW_CHUNK_MAP_SIZE_HALF;

	size_t lod = 0;
	int64_t distance = MathUtil::Distance(fromCol - toCol, fromRow - toRow);
	if (distance >= base)
	{
		const size_t MOVE_TO_ASTAR_LOD = static_cast<size_t>(DebugInfo::MOVE_TO_ASTAR_LOD);

		distance -= base;
		while (distance > base && lod < MOVE_TO_ASTAR_LOD)
		{
			distance -= base;
			++lod;
		}
		lod = std::min(mGridLod->GetLevel(), lod);
	}

	AStarResult result = AStarResult::SEARCHING;
	while (true)
	{
		fromCol = from.first >> lod;
		fromRow = from.second >> lod;
		toCol = to.first >> lod;
		toRow = to.second >> lod;

		mAStarSearch->SetStartAndGoalStates(AStarState{ fromCol, fromRow }, AStarState{ toCol, toRow });

		do
		{
			result = mAStarSearch->SearchStep(
				[this, lod](int64_t u, int64_t v)->bool
				{
					return mGridLod->IsBlocked(lod, static_cast<GridCol>(u), static_cast<GridRow>(v));
				}
			);

		} while (result == AStarResult::SEARCHING);

		if (result == AStarResult::SUCCEEDED)
			break;

		if (result == AStarResult::OUT_OF_MEMORY)
			break;

		if (lod == 0)
			break;

		--lod;
	}

	limit >>= lod;
	const int16_t add = (lod > 0 ? static_cast<int16_t>(1 << (lod - 1)) : 0);
	mCurrentLodForDebug = lod;

	AStarState* state = mAStarSearch->GetSolutionStart();
	while (state)
	{
		GridCol c = (GridCol)state->x << lod;
		GridRow r = (GridRow)state->y << lod;

		const ColRowPair stateCell = GetCell(c + add, r + add);
		const Scalar stateX = GridUtil::GetCenterX(stateCell.first);
		const Scalar stateY = GridUtil::GetCenterY(stateCell.second);

		resultX = stateX;
		resultY = stateY;

		state = mAStarSearch->GetSolutionNext();

		--limit;
		if (limit <= 0)
			break;
	}
}

void Grid::ResetDebugAStar()
{
	mAStarSearch->FreeAllNodes();
}

std::deque<std::pair<int, int>> Grid::DebugAStar()
{
	std::deque<std::pair<int, int>> result;

	const size_t lod = mCurrentLodForDebug;
	const int add = (lod > 0 ? static_cast<int>(1 << (lod - 1)) : 0);

	AStarState* state = mAStarSearch->GetSolutionStart();
	while (state)
	{
		GridCol c = (GridCol)state->x << lod;
		GridRow r = (GridRow)state->y << lod;

		result.emplace_back(std::pair{ c + add,  r + add });

		state = mAStarSearch->GetSolutionNext();
	}

	return result;
}
