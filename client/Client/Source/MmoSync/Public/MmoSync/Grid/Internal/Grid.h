// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/GridCell/GridCellDef.h"
#include "MmoSync/Grid/GridCell/GridCellData.h"
#include "MmoSync/Grid/GridFlow/Internal/GridFlowDef.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 뚫린 Cell과 막힌 Cell을 구분하여 처리하고, 길찾기 흐름 데이터를 캐싱하여 고속 처리하는 기능을 제공하는 Grid 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GridFlowChunkManager;
class GridFlowChunk;
class AStarSearch;
class GridLod;

class Grid final
{
private:
	GridCellData mGridCellData;
	std::unique_ptr<GridFlowChunkManager> mGridFlowChunkManager;
	std::unique_ptr<AStarSearch> mAStarSearch;
	std::unique_ptr<GridLod> mGridLod;
	size_t mCurrentLodForDebug = 0;

public:
	explicit Grid(uint8_t colCountFactor, uint8_t rowCountFactor);
	virtual ~Grid();

public:
	void BuildGridLod();

private:
	friend class GridFlowChunk;
	void BuildGridFlow(GridFlowChunk& gridFlowChunk);

public:
	bool IsValid(GridCol c, GridRow r) const;
	bool IsBlocked(GridCol c, GridRow r) const;

	void SetBlocked(GridCol c, GridRow r);
	void SetUnblocked(GridCol c, GridRow r);


	ColRowPair GetExitCell(GridCol c, GridRow r, GridCellDir firstDir) const;
	ColRowPair GetNextCell(ColRowPair& cell, GridCellDir dir) const;

public:
	// ========================================================================
	// 출발 셀(fromU, fromV)에서 도착 셀(toU, toV)로 이동할 때의 흐름 방향 계산
	// ------------------------------------------------------------------------
	// @param fromU, fromV: 출발 셀의 열(Col), 행(Row) 좌표
	// @param toU, toV: 도착 셀의 열(Col), 행(Row) 좌표
	// @return pair<GridFlowMode, GridFlowDir>:
	//   - GridFlowMode: 이동 가능 여부 및 방식
	//       GRID_FLOW_MODE_INACESSIBLE (0): 도달 불가
	//       GRID_FLOW_MODE_RAY_TEST_NEEDED (1): 우회 필요 여부 테스트 필요
	//       GRID_FLOW_MODE_DIRECT (2): 직진 가능 (장애물 없음)
	//       GRID_FLOW_MODE_USE_FLOW_DIR (3): 우회 필요 (FlowDir 사용)
	//   - GridFlowDir: 8방향 이동 방향 (숫자패드 방식: 8=위, 6=오른쪽, ...)
	// ------------------------------------------------------------------------
	// 내부적으로 GridFlowChunkManager를 통해 사전 계산된 Flow Field를 조회
	// ========================================================================
	std::pair<GridFlowMode, GridFlowDir> CalcFlowDir(int64_t fromU, int64_t fromV, int64_t toU, int64_t toV);

	// ========================================================================
	// 월드 좌표(fromX, fromY)에서 (toX, toY)로 이동할 때의 흐름 방향 계산
	// ------------------------------------------------------------------------
	// @param fromX, fromY: 출발 위치의 월드 좌표 (Scalar, 고정소수점)
	// @param toX, toY: 도착 위치의 월드 좌표 (Scalar, 고정소수점)
	// @return: CalcFlowDir과 동일
	// ------------------------------------------------------------------------
	// 월드 좌표를 셀 좌표로 변환 후 CalcFlowDir 호출
	// ========================================================================
	std::pair<GridFlowMode, GridFlowDir> CalcFlowDirByPos(Scalar fromX, Scalar fromY, Scalar toX, Scalar toY);

public:
	GridCellData& GetCellData() { return mGridCellData; }
	const GridCellData& GetCellData() const { return mGridCellData; }

public:
	inline size_t GetCellIndex(GridCol c, GridRow r) const noexcept { return (r * mGridCellData.mColCount + c); }

public:
	ColRowPair GetCell(GridCol c, GridRow r) const;
	ColRowPair PickCell(Scalar x, Scalar y) const;

	inline GridCol GetColCountFactor() const noexcept { return mGridCellData.mColCountFactor; }
	inline GridRow GetRowCountFactor() const noexcept { return mGridCellData.mRowCountFactor; }
	inline GridCol GetColCount() const noexcept { return mGridCellData.mColCount; }
	inline GridRow GetRowCount() const noexcept { return mGridCellData.mRowCount; }

public:
	void CalcAStarSearch(const ColRowPair& from, const ColRowPair& to, int limit, OUT Scalar& resultX, OUT Scalar& resultY);

public:
	void ResetDebugAStar();
	std::deque<std::pair<int, int>> DebugAStar();
	size_t GetCurrentLodForDebug() const { return mCurrentLodForDebug; }
};
