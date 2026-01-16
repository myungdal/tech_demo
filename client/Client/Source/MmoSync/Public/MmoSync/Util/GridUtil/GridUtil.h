// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/GridFlow/Internal/GridFlowDef.h"
#include "MmoSync/Grid/MultiGridDef.h"
#include "MmoSync/Grid/MultiGrid.h"


struct PositionComponent;
struct PhysicsComponent;

namespace GridUtil
{
	inline Scalar GetCenterX(GridCol c) noexcept { return (static_cast<Scalar>(c) << CELL_SIZE_FACTOR) | CELL_SIZE_HALF; }
	inline Scalar GetCenterY(GridRow r) noexcept { return (static_cast<Scalar>(r) << CELL_SIZE_FACTOR) | CELL_SIZE_HALF; }
	inline GridCol ConvToCol(Scalar x) noexcept { return static_cast<GridCol>(x >> CELL_SIZE_FACTOR); }
	inline GridRow ConvToRow(Scalar y) noexcept { return static_cast<GridRow>(y >> CELL_SIZE_FACTOR); }

	inline std::pair<int64_t, int64_t> GetVec1024FromFlowDir(GridFlowDir flowDir) noexcept;

	template <typename _IsBlockFnc>
	bool IsRayBlocked(
		GridCol lastCol, GridRow lastRow,
		GridCol c0, GridRow r0, GridCol c1, GridRow r1,
		int32_t rayLength, _IsBlockFnc IsBlockFnc);

	template <typename _SetBlockFnc>
	void BlockRayCells(
		GridCol lastCol, GridRow lastRow,
		GridCol c0, GridRow r0, GridCol c1, GridRow r1,
		int32_t rayLength, _SetBlockFnc SetBlockFnc);

	inline void CheckBoundsAndAdjust(MultiGrid& grid, PositionComponent& position, PhysicsComponent& physics, Scalar dx, Scalar dy) noexcept;
}


#include "MmoSync/Component/PositionComponent.h"
#include "MmoSync/Component/PhysicsComponent.h"


namespace GridUtil
{
	std::pair<int64_t, int64_t> GetVec1024FromFlowDir(GridFlowDir flowDir) noexcept
	{
		switch (flowDir)
		{
		case GRID_FLOW_DIR_8: return MathUtil::GetVec1024FromAngle(-16'388);
		case GRID_FLOW_DIR_9: return MathUtil::GetVec1024FromAngle(-8'198);
		case GRID_FLOW_DIR_6: return MathUtil::GetVec1024FromAngle(0);
		case GRID_FLOW_DIR_3: return MathUtil::GetVec1024FromAngle(8'192);
		case GRID_FLOW_DIR_2: return MathUtil::GetVec1024FromAngle(16'384);
		case GRID_FLOW_DIR_1: return MathUtil::GetVec1024FromAngle(24'576);
		case GRID_FLOW_DIR_4: return MathUtil::GetVec1024FromAngle(-32'768);
		case GRID_FLOW_DIR_7: return MathUtil::GetVec1024FromAngle(-24'578);
		default: return { 0, 0 };
		}
	}

	template <typename _IsBlockFnc>
	bool IsRayBlocked(
		GridCol lastCol, GridRow lastRow,
		GridCol c0, GridRow r0, GridCol c1, GridRow r1,
		int32_t rayLength, 
		_IsBlockFnc IsBlockFnc)
	{
		GridCoordinate dc, ic;
		if (c0 < c1) { dc = c1 - c0; ic = 1; }
		else { dc = c0 - c1; ic = -1; }

		GridCoordinate dr, ir;
		if (r0 < r1) { dr = r0 - r1; ir = 1; }
		else { dr = r1 - r0; ir = -1; }

		GridCoordinate err = dc + dr;

		while (true)
		{
			// 맵 임계를 만나면 완료
			if ((0 > r0) || (0 > c0) || (lastCol < c0) || (lastRow < r0))
				break;

			// 목적지 완료.
			if ((c0 == c1) && (r0 == r1))
				break;

			if (IsBlockFnc(c0, r0))
				return true;

			const GridCoordinate err2 = 2 * err;

			const bool increaseCol = (err2 > dr) ? true : false;
			const bool increaseRow = (err2 < dc) ? true : false;

			if (true == increaseCol)
			{
				err += dr;
				c0 += ic;

				if (IsBlockFnc(c0, r0))
					return true;
			}

			if (true == increaseRow)
			{
				err += dc;
				r0 += ir;

				if (IsBlockFnc(c0, r0))
					return true;
			}

			if (rayLength > 0)
			{
				--rayLength;
				if (0 == rayLength)
					break;
			}
		}

		return false;
	}

	template <typename _SetBlockFnc>
	void BlockRayCells(
		GridCol lastCol, GridRow lastRow,
		GridCol c0, GridRow r0, GridCol c1, GridRow r1,
		int32_t rayLength, 
		_SetBlockFnc SetBlockFnc)
	{
		IsRayBlocked(lastCol, lastRow, c0, r0, c1, r1, rayLength,
			[&SetBlockFnc](GridCol c, GridRow r) -> bool
			{
				SetBlockFnc(c, r);
				return false;
			}
		);
	}

	void CheckBoundsAndAdjust(MultiGrid& grid, PositionComponent& position, PhysicsComponent& physics, Scalar dx, Scalar dy) noexcept
	{
		const GridCol colCount = grid.GetColCount();
		const GridRow rowCount = grid.GetRowCount();

		if (position.x < 0)
		{
			physics.vx = -physics.vx;
			position.x = 0;
			position.x -= (dx << 1);
		}
		else if(position.x > (Scalar(colCount) << CELL_SIZE_FACTOR))
		{
			physics.vx = -physics.vx;
			position.x = (Scalar(colCount) << CELL_SIZE_FACTOR);
			position.x -= (dx << 1);
		}

		if (position.y < 0)
		{
			physics.vy = -physics.vy;
			position.y = 0;
			position.y -= (dy << 1);
		}
		else if(position.y > (Scalar(rowCount) << CELL_SIZE_FACTOR))
		{
			physics.vy = -physics.vy;
			position.y = (Scalar(rowCount) << CELL_SIZE_FACTOR);
			position.y -= (dy << 1);
		}
	}
}
