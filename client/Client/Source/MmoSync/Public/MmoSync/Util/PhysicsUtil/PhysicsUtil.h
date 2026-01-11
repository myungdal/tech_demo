// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/MultiGrid.h"
#include "MmoSync/Util/GridUtil/GridUtil.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"
#include "MmoSync/System/Physics/PhysicsSystemVelocityGround.h"


namespace PhysicsUtil
{
	inline void ApplyForce(PhysicsComponent& physics, const PositionComponent& position, Scalar targetX, Scalar targetY) noexcept
	{
		Scalar dx = targetX - position.x;
		Scalar dy = targetY - position.y;

		if (dx == 0 && dy == 0)
			return;

		auto [nx, ny] = MathUtil::GetVec1024(dx, dy);
		physics.fx += physics.force * nx;
		physics.fy += physics.force * ny;

		physics.ex += physics.velocity * nx;
		physics.ey += physics.velocity * ny;
	}

	//inline void UpdateAngle(PhysicsComponent& physics, Scalar dx, Scalar dy)
	//{
	//	// 이동량이 미미하면 회전하지 않음 (떨림 방지)
	//	const int64_t MIN_MOVE_SQ = 100;
	//	if (MathUtil::DistanceSq(dx, dy) < MIN_MOVE_SQ)
	//		return;

	//	const Angle targetAngle = MathUtil::ArcTan(dx, dy);

	//	// 최단 회전 방향 계산
	//	int32_t diff = targetAngle - physics.angle;
	//	while (diff <= -32768) diff += 65536;
	//	while (diff > 32767) diff -= 65536;

	//	// 회전 속도 (기본값: 2048 = 약 11.25도/틱)
	//	const Angle turnSpeed = 2048;

	//	if (std::abs(diff) <= turnSpeed)
	//	{
	//		physics.angle = targetAngle;
	//	}
	//	else
	//	{
	//		physics.angle += (diff > 0) ? turnSpeed : -turnSpeed;
	//	}
	//}

	inline void HandleFriction(IN OUT PhysicsComponent& physics) noexcept
	{
		{
			const Scalar rx = MathUtil::Div(physics.ex, physics.friction);
			const Scalar ry = MathUtil::Div(physics.ey, physics.friction);

			if (physics.ex > 0)
				physics.ex = (physics.ex > rx) ? (physics.ex - rx) : 0;
			else
				physics.ex = (physics.ex < rx) ? (physics.ex - rx) : 0;

			if (physics.ey > 0)
				physics.ey = (physics.ey > ry) ? (physics.ey - ry) : 0;
			else
				physics.ey = (physics.ey < ry) ? (physics.ey - ry) : 0;

		}
		{
			const Scalar rx = MathUtil::Div(physics.vx, physics.friction);
			const Scalar ry = MathUtil::Div(physics.vy, physics.friction);

			if (physics.vx > 0)
				physics.vx = (physics.vx > rx) ? (physics.vx - rx) : 0;
			else
				physics.vx = (physics.vx < rx) ? (physics.vx - rx) : 0;

			if (physics.vy > 0)
				physics.vy = (physics.vy > ry) ? (physics.vy - ry) : 0;
			else
				physics.vy = (physics.vy < ry) ? (physics.vy - ry) : 0;
		}
	}

	// 엔티티가 블록된 셀에 위치한 경우 탈출 처리
	// 반환값: true = 탈출 처리 수행됨, false = 블록되지 않은 셀에 위치 (정상)
	inline bool ProcessExitCells(const MultiGrid& grid, const PhysicsComponent& physics, IN OUT PositionComponent& position, GridCellDir firstDir, GridMoverType moverType = GridMoverType::WALKER)
	{
		// 현재 위치의 셀 좌표 확인
		ColRowPair thisCell = grid.PickCell(position.x, position.y);
		
		// 현재 셀이 블록되어 있는지 체크
		if (grid.IsBlocked(thisCell.first, thisCell.second, moverType))
		{
			// 목표 좌표 결정: 활성 슬롯 우선, 없으면 home 사용
			Scalar sx = 0, sy = 0;
			if (physics.activeLayerIndex >= 0)
			{
				const MoveCommand& activeCmd = physics.commandSlots[physics.activeLayerIndex];
				sx = activeCmd.targetX;
				sy = activeCmd.targetY;
			}
			// 활성 슬롯이 없거나 목표가 원점이면 home으로 폴백
			if ((sx == 0 && sy == 0))
			{
				sx = physics.homeX;
				sy = physics.homeY;
			}
			
			// 목표 좌표가 유효한 경우
			if (sx && sy)
			{
				// 현재 위치에서 목표까지의 방향 벡터 계산
				const Scalar dx = (sx - position.x);
				const Scalar dy = (sy - position.y);

				// 주 이동 방향 결정 (상하좌우 중 하나)
				GridCellDir flowDir = GRID_CELL_DIR_NONE;
				if (std::abs(dx) > std::abs(dy)) { flowDir = (dx > 0) ? GRID_CELL_DIR_6 : GRID_CELL_DIR_4; } // 좌우
				else { flowDir = (dy > 0) ? GRID_CELL_DIR_2 : GRID_CELL_DIR_8; } // 상하

				// 목표 방향의 인접 셀 확인
				const ColRowPair nextCell = grid.GetNextCell(thisCell, flowDir, moverType);
				
				// 인접 셀이 블록되지 않았으면 해당 셀로 이동
				if (!grid.IsBlocked(nextCell.first, nextCell.second, moverType))
				{
					position.x = GridUtil::GetCenterX(nextCell.first);
					position.y = GridUtil::GetCenterY(nextCell.second);
					return true;
				}
			}

			// 목표 좌표가 없거나 목표 방향이 막힌 경우: 나선형 탐색으로 가장 가까운 빈 셀 찾기
			const ColRowPair exitCell = grid.GetExitCell(thisCell.first, thisCell.second, firstDir, moverType);
			position.x = GridUtil::GetCenterX(exitCell.first);
			position.y = GridUtil::GetCenterY(exitCell.second);
			return true;
		}

		// 블록되지 않은 셀에 위치 (정상 상태)
		return false;
	}

	inline bool IsPositionBlocked(
		const MultiGrid& grid,
		Scalar x,
		Scalar y,
		Scalar radius,
		GridMoverType moverType = GridMoverType::WALKER,
		Scalar dirX = 0,
		Scalar dirY = 0)
	{
		// radius를 고려하여 엔티티가 차지하는 영역의 충돌 체크
		if (radius <= 0)
		{
			const ColRowPair cell = grid.PickCell(x, y);
			return grid.IsBlocked(cell.first, cell.second, moverType);
		}

		const bool directional = (dirX != 0 || dirY != 0);

		// 중심점 체크
		const ColRowPair centerCell = grid.PickCell(x, y);
		if (grid.IsBlocked(centerCell.first, centerCell.second, moverType))
			return true;

		// radius가 작으면 중심점만 체크
		if (radius < (CELL_SIZE >> 2))
			return false;

		// radius를 고려한 방향성 경계 체크 (상하좌우)
		if (!directional || dirX > 0)
		{
			const ColRowPair rightCell = grid.PickCell(x + radius, y);
			if (grid.IsBlocked(rightCell.first, rightCell.second, moverType))
				return true;
		}

		if (!directional || dirX < 0)
		{
			const ColRowPair leftCell = grid.PickCell(x - radius, y);
			if (grid.IsBlocked(leftCell.first, leftCell.second, moverType))
				return true;
		}

		if (!directional || dirY > 0)
		{
			const ColRowPair downCell = grid.PickCell(x, y + radius);
			if (grid.IsBlocked(downCell.first, downCell.second, moverType))
				return true;
		}

		if (!directional || dirY < 0)
		{
			const ColRowPair upCell = grid.PickCell(x, y - radius);
			if (grid.IsBlocked(upCell.first, upCell.second, moverType))
				return true;
		}

		return false;
	}

	inline void HandleBlocked(MultiGrid& grid, PositionComponent& position, PhysicsComponent& physics, Scalar dx, Scalar dy, GridCellDir firstDir, GridMoverType moverType = GridMoverType::WALKER)
	{
		const Scalar checkRadius = (physics.radius << 1);

		if (!IsPositionBlocked(grid, position.x, position.y, checkRadius, moverType, dx, dy))
			return;

		const Scalar prevX = position.x - dx;
		const Scalar prevY = position.y - dy;

		position.x = prevX;
		position.y = prevY;

		bool blockedX = false;
		bool blockedY = false;

		if (dx != 0)
		{
			const Scalar candidateX = prevX + dx;
			if (IsPositionBlocked(grid, candidateX, prevY, checkRadius, moverType, dx, 0))
			{
				blockedX = true;
				physics.vx = -physics.vx - (dx >> 4);
			}
			else
			{
				position.x = candidateX;
			}
		}

		if (dy != 0)
		{
			const Scalar candidateY = prevY + dy;
			if (IsPositionBlocked(grid, position.x, candidateY, checkRadius, moverType, 0, dy))
			{
				blockedY = true;
				physics.vy = -physics.vy - (dy >> 4);
			}
			else
			{
				position.y = candidateY;
			}
		}

		if (blockedX && blockedY)
		{
			_DEBUG_RED;
			position.x = prevX;
			position.y = prevY;
			PhysicsUtil::ProcessExitCells(grid, physics, IN OUT position, firstDir, moverType);
		}
	}

	inline void HandleMovementGround(OUT PhysicsComponent& physics, OUT PositionComponent& position, OUT Scalar& dx, OUT Scalar& dy)
	{
		if (physics.vx == 0 &&
			physics.vy == 0 &&
			physics.fx == 0 &&
			physics.fy == 0 &&
			physics.ex == 0 &&
			physics.ey == 0)
		{
			return;
		}

		dx = physics.vx + physics.ex;
		dy = physics.vy + physics.ey;

		{
			Scalar fx = physics.fx;
			Scalar fy = physics.fy;

			physics.vx += fx;
			physics.vy += fy;

			physics.fx = 0;
			physics.fy = 0;

			dx += (fx >> 1);
			dy += (fy >> 1);
		}

		//const int64_t PHYSICS_GROUND_STATIC_FRICTION = gDebugInfo.Get<int64_t>(L"PHYSICS_GROUND_STATIC_FRICTION");
		//const int64_t PHYSICS_GROUND_STOP_VELOCITY = gDebugInfo.Get<int64_t>(L"PHYSICS_GROUND_STOP_VELOCITY");

		// 정지 마찰력
		//if (physics.vx == 0 &&
		//	physics.vy == 0 &&
		//	MathUtil::DistanceSq(dx, dy) < PHYSICS_GROUND_STATIC_FRICTION)
		//{
		//	return;
		//}

		position.x += dx;
		position.y += dy;

		// 정지
		//if (MathUtil::DistanceSq(physics.vx, physics.vy) < PHYSICS_GROUND_STOP_VELOCITY)
		//{
		//	physics.vx = 0;
		//	physics.vy = 0;
		//}
	}

	inline void HandleMovementAir(OUT PhysicsComponent& physics, OUT PositionComponent& position, OUT Scalar& dx, OUT Scalar& dy)
	{
		if (physics.vx == 0 &&
			physics.vy == 0 &&
			physics.fx == 0 &&
			physics.fy == 0 &&
			physics.ex == 0 &&
			physics.ey == 0)
		{
			return;
		}

		dx = physics.vx + physics.ex;
		dy = physics.vy + physics.ey;

		{
			Scalar fx = physics.fx;
			Scalar fy = physics.fy;

			physics.vx += fx;
			physics.vy += fy;

			physics.fx = 0;
			physics.fy = 0;

			dx += (fx >> 1);
			dy += (fy >> 1);
		}

		position.x += dx;
		position.y += dy;		
	}

	inline void HandleFlow(
		MultiGrid& grid,
		PhysicsComponent& physics,
		PositionComponent& position,
		GridMoverType moverType = GridMoverType::WALKER
	)
	{
		// 활성 슬롯의 타겟 좌표 가져오기
		Scalar targetX = 0, targetY = 0;
		if (physics.activeLayerIndex >= 0)
		{
			const MoveCommand& activeCmd = physics.commandSlots[physics.activeLayerIndex];
			targetX = activeCmd.targetX;
			targetY = activeCmd.targetY;
		}

		const auto [flowMode, flowDir] = grid.CalcFlowDirByPos(position.x, position.y, targetX, targetY, moverType);

		if (flowMode == GRID_FLOW_MODE_USE_FLOW_DIR)
		{
			auto [fx, fy] = GridUtil::GetVec1024FromFlowDir(flowDir);

			ApplyForce(physics, position, position.x + fx, position.y + fy);
		}
		else
		{
			ApplyForce(physics, position, targetX, targetY);
		}
	}

	inline int UpdateActiveLayer(PhysicsComponent& physics, MAYBE_UNUSED int64_t arrivalRangeSq = 0)
	{
		int activeIndex = -1;
		int highestArrived = -1;

		for (int i = static_cast<int>(MoveCommandLayer::MAX) - 1; i >= 0; --i)
		{
			MoveCommand& cmd = physics.commandSlots[i];

			// 1. maxLifetime 감소 및 만료 체크
			if (cmd.state != MoveCommandState::NONE && cmd.maxLifetime > 0)
			{
				cmd.maxLifetime--;
				if (cmd.maxLifetime == 0)
				{
					cmd.state = MoveCommandState::COMPLETED;
				}
			}

			// 2. ARRIVED 상태의 minHold 감소 및 전환
			if (cmd.state == MoveCommandState::ARRIVED)
			{
				if (cmd.minHold > 0)
				{
					cmd.minHold--;
					if (cmd.minHold == 0)
					{
						// minHold 종료: 하위 레이어 전환을 위해 COMPLETED로 설정
						cmd.state = MoveCommandState::COMPLETED;
					}
				}
				else
				{
					// 처음부터 minHold==0인 ARRIVED: 즉시 COMPLETED로 전환하여 우선순위 양보
					cmd.state = MoveCommandState::COMPLETED;
				}
			}

			// 3. COMPLETED 상태는 자동 정리
			if (cmd.state == MoveCommandState::COMPLETED)
			{
				cmd.Clear();
				continue;
			}

			// 4. 우선순위 결정 (상위 레이어부터 확인)
			if (cmd.state == MoveCommandState::RUNNING)
			{
				// RUNNING 최우선: 즉시 선택하고 하위 레이어 무시
				activeIndex = i;
				break;
			}
			else if (cmd.state == MoveCommandState::ARRIVED)
			{
				// minHold가 남아있는 ARRIVED는 "블로킹" 상태로 간주
				// (예: 넉백 경직, 스킬 후딜레이 등은 하위 레이어 입력을 막아야 함)
				if (cmd.minHold > 0)
				{
					activeIndex = i;
					break;
				}

				// minHold가 없는 ARRIVED는 하위 레이어 실행을 허용
				// (실행할 하위 레이어가 없을 경우를 대비해 후보로 등록)
				if (highestArrived == -1)
					highestArrived = i;
			}
		}

		// RUNNING이나 블로킹(Hold) 중인 레이어가 없으면, 후보(단순 Arrived) 선택
		if (activeIndex == -1)
		{
			if (highestArrived != -1)
				activeIndex = highestArrived;
		}

		physics.activeLayerIndex = (int8_t)activeIndex;
		return activeIndex;
	}
}

