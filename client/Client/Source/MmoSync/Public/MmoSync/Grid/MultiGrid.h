// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/Internal/Grid.h"
#include "MmoSync/Grid/MultiGridDef.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Multi-layer collision을 지원하는 MultiGrid 클래스
// 각 GridMoverType별로 별도의 Grid를 관리하여 성능 최적화
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MMOSYNC_API MultiGrid final
{
private:
	// 각 GridMoverType별로 별도의 Grid 관리
	std::array<std::unique_ptr<Grid>, static_cast<size_t>(GridMoverType::MAX)> mGrids;
	
	// 각 셀의 GridCellBlockType 정보 저장
	std::vector<GridCellBlockType> mBlockTypes;

public:
	explicit MultiGrid(uint8_t colCountFactor, uint8_t rowCountFactor);
	virtual ~MultiGrid();

public:
	void BuildGridLod();

public:
	bool IsBlockedAny(GridCol c, GridRow r) const; // 어느 하나라도...
	bool IsBlocked(GridCol c, GridRow r, GridMoverType moverType) const;
	void SetBlocked(GridCol c, GridRow r, GridCellBlockType blockType);
	void SetUnblocked(GridCol c, GridRow r, GridCellBlockType blockType);

	friend class FieldGridGenerator;
	friend class DungeonGridGenerator;	
private:
	void SetGridCellBlockType(GridCol c, GridRow r, GridCellBlockType blockType, bool active);
	void UpdateGrids(GridCol c, GridRow r, GridCellBlockType blockType, bool active);

private:
	// Grid 접근자들 (internal use only)
	Grid& GetGrid(GridMoverType moverType) { return *mGrids[static_cast<size_t>(moverType)]; }
	const Grid& GetGrid(GridMoverType moverType) const { return *mGrids[static_cast<size_t>(moverType)]; }
	
public:
	ColRowPair GetExitCell(GridCol c, GridRow r, GridCellDir firstDir, GridMoverType moverType) const;
	ColRowPair GetExitCellAny(GridCol c, GridRow r, GridCellDir firstDir) const;
	ColRowPair GetNextCell(ColRowPair& cell, GridCellDir dir, GridMoverType moverType) const;
	std::pair<GridFlowMode, GridFlowDir> CalcFlowDir(int64_t fromU, int64_t fromV, int64_t toU, int64_t toV, GridMoverType moverType);
	std::pair<GridFlowMode, GridFlowDir> CalcFlowDirByPos(Scalar fromX, Scalar fromY, Scalar toX, Scalar toY, GridMoverType moverType);
	const GridCellData& GetCellData(GridMoverType moverType) const;
	GridCellBlockType GetGridCellBlockType(GridCol c, GridRow r) const;

public:
	bool IsValid(GridCol c, GridRow r) const;
	ColRowPair GetCell(GridCol c, GridRow r) const;
	ColRowPair PickCell(Scalar x, Scalar y) const;
	size_t GetCellIndex(GridCol c, GridRow r) const noexcept;
	GridCol GetColCountFactor() const noexcept { return GetBaseGrid().GetColCountFactor(); }
	GridRow GetRowCountFactor() const noexcept { return GetBaseGrid().GetRowCountFactor(); }
	GridCol GetColCount() const noexcept { return GetBaseGrid().GetColCount(); }
	GridRow GetRowCount() const noexcept { return GetBaseGrid().GetRowCount(); }

private:
	// 기본 Grid 접근자 (WALKER 타입, internal use only)
	Grid& GetBaseGrid() { return GetGrid(GridMoverType::WALKER); }
	const Grid& GetBaseGrid() const { return GetGrid(GridMoverType::WALKER); }

public:
	void CalcAStarSearch(const ColRowPair& from, const ColRowPair& to, int limit, OUT Scalar& resultX, OUT Scalar& resultY);
	void ResetDebugAStar();
	std::deque<std::pair<int, int>> DebugAStar();
	size_t GetCurrentLodForDebug() const;
};
