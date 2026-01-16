// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Grid/MultiGrid.h"

#include "MmoSync/Grid/MultiGridDef.h"
#include "MmoSync/Grid/GridFlow/GridFlowBuilder.h"
#include "MmoSync/Grid/GridFlow/Internal/GridFlowChunk.h"
#include "MmoSync/Grid/GridFlow/GridFlowChunkManager.h"
#include "MmoSync/Grid/AStarSearch/AStarSearch.h"
#include "MmoSync/Grid/GridLod/GridLod.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"
#include "MmoSync/Util/GridUtil/GridUtil.h"


MultiGrid::MultiGrid(uint8_t colCountFactor, uint8_t rowCountFactor)
{
	// 각 GridMoverType별로 별도의 Grid 생성
	for (size_t i = 0; i < static_cast<size_t>(GridMoverType::MAX); ++i)
	{
		mGrids[i] = std::make_unique<Grid>(colCountFactor, rowCountFactor);
	}
	
	// BlockType 배열 초기화 (GROUND로 초기화)
	const size_t totalCells = (1ULL << colCountFactor) * (1ULL << rowCountFactor);
	mBlockTypes.resize(totalCells, GridCellBlockType::GROUND);
}

MultiGrid::~MultiGrid()
{
}

void MultiGrid::BuildGridLod()
{
	// 모든 Grid 초기화
	for (auto& grid : mGrids)
	{
		grid->BuildGridLod();
	}
}

bool MultiGrid::IsBlockedAny(GridCol c, GridRow r) const
{
	for (size_t i = 0; i < static_cast<size_t>(GridMoverType::MAX); ++i)
	{
		if (mGrids[i]->IsBlocked(c, r))
			return true;
	}

	return false;
}

bool MultiGrid::IsBlocked(GridCol c, GridRow r, GridMoverType moverType) const
{
	if (!IsValid(c, r))
		return true;

	const size_t moverTypeIdx = static_cast<size_t>(moverType);
	return mGrids[moverTypeIdx]->IsBlocked(c, r);
}

void MultiGrid::SetBlocked(GridCol c, GridRow r, GridCellBlockType blockType)
{
	SetGridCellBlockType(c, r, blockType, true);
}

void MultiGrid::SetUnblocked(GridCol c, GridRow r, GridCellBlockType blockType)
{
	SetGridCellBlockType(c, r, blockType, false);
}

void MultiGrid::SetGridCellBlockType(GridCol c, GridRow r, GridCellBlockType blockType, bool active)
{
	if (!IsValid(c, r))
		return;

	// BlockType 저장
	const size_t idx = GetCellIndex(c, r);
	if (active)
		mBlockTypes[idx] = blockType;
	else
		mBlockTypes[idx] = GridCellBlockType::GROUND; // 비활성화 시 GROUND로 복구

	UpdateGrids(c, r, blockType, active);
}

void MultiGrid::UpdateGrids(GridCol c, GridRow r, GridCellBlockType blockType, bool active)
{
	const size_t blockTypeIdx = static_cast<size_t>(blockType);
	const size_t requiredAbility = gGridCellBlockCollisions[blockTypeIdx];
	
	if (active)
	{
		// 지형 설치: 능력이 없으면 블록, 있으면 언블록
		// (WALL은 requiredAbility=0이므로 모든 moverType이 hasAbility=false → 블록)
		for (size_t moverTypeIdx = 0; moverTypeIdx < static_cast<size_t>(GridMoverType::MAX); ++moverTypeIdx)
		{
			const size_t moverAbilities = gMoverTypeToCollisionBit[moverTypeIdx];
			const bool hasAbility = (moverAbilities & requiredAbility) != 0;
			
			if (!hasAbility)
				mGrids[moverTypeIdx]->SetBlocked(c, r);
		}
	}
	else
	{
		// [WHY] 제거 시 이전 blockType을 복원할 정보가 없으므로 전체 moverType을 언블록한다.
		// (정의되지 않은 collision 상태/잔여 블록 방지). 필요하면 이전 상태를 저장해 정확히 복원한다.
		for (size_t moverTypeIdx = 0; moverTypeIdx < static_cast<size_t>(GridMoverType::MAX); ++moverTypeIdx)
		{
			mGrids[moverTypeIdx]->SetUnblocked(c, r);
		}
	}
}

ColRowPair MultiGrid::GetExitCell(GridCol c, GridRow r, GridCellDir firstDir, GridMoverType moverType) const
{
	return GetGrid(moverType).GetExitCell(c, r, firstDir);
}

ColRowPair MultiGrid::GetExitCellAny(GridCol c, GridRow r, GridCellDir firstDir) const
{
	if (!IsValid(c, r))
		return GetCell(c, r);

	size_t step = 0;

	while (IsBlockedAny(c, r))
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

ColRowPair MultiGrid::GetNextCell(ColRowPair& cell, GridCellDir dir, GridMoverType moverType) const
{
	return GetGrid(moverType).GetNextCell(cell, dir);
}

std::pair<GridFlowMode, GridFlowDir> MultiGrid::CalcFlowDir(int64_t fromU, int64_t fromV, int64_t toU, int64_t toV, GridMoverType moverType)
{
	return GetGrid(moverType).CalcFlowDir(fromU, fromV, toU, toV);
}

std::pair<GridFlowMode, GridFlowDir> MultiGrid::CalcFlowDirByPos(Scalar fromX, Scalar fromY, Scalar toX, Scalar toY, GridMoverType moverType)
{
	return GetGrid(moverType).CalcFlowDirByPos(fromX, fromY, toX, toY);
}

const GridCellData& MultiGrid::GetCellData(GridMoverType moverType) const
{
	return GetGrid(moverType).GetCellData();
}

GridCellBlockType MultiGrid::GetGridCellBlockType(GridCol c, GridRow r) const
{
	if (!IsValid(c, r))
		return GridCellBlockType::GROUND;

	const size_t idx = GetCellIndex(c, r);
	return mBlockTypes[idx];
}

bool MultiGrid::IsValid(GridCol c, GridRow r) const
{
	return GetBaseGrid().IsValid(c, r);
}

ColRowPair MultiGrid::GetCell(GridCol c, GridRow r) const
{
	return GetBaseGrid().GetCell(c, r);
}

ColRowPair MultiGrid::PickCell(Scalar x, Scalar y) const
{
	return GetBaseGrid().PickCell(x, y);
}

inline size_t MultiGrid::GetCellIndex(GridCol c, GridRow r) const noexcept
{
	return GetBaseGrid().GetCellIndex(c, r);
}

void MultiGrid::CalcAStarSearch(const ColRowPair& from, const ColRowPair& to, int limit, OUT Scalar& resultX, OUT Scalar& resultY)
{
	GetBaseGrid().CalcAStarSearch(from, to, limit, resultX, resultY);
}

void MultiGrid::ResetDebugAStar()
{
	GetBaseGrid().ResetDebugAStar();
}

std::deque<std::pair<int, int>> MultiGrid::DebugAStar()
{
	return GetBaseGrid().DebugAStar();
}

size_t MultiGrid::GetCurrentLodForDebug() const
{
	return GetBaseGrid().GetCurrentLodForDebug();
}
