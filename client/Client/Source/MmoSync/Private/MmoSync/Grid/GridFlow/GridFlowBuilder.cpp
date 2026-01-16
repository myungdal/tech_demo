// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Grid/GridFlow/GridFlowBuilder.h"


namespace
{
	inline GridFlowDir CalculateDirection(int64_t du, int64_t dv) noexcept
	{
		static constexpr GridFlowDir directionMap[3][3] = {
			{ GRID_FLOW_DIR_7, GRID_FLOW_DIR_8, GRID_FLOW_DIR_9 },
			{ GRID_FLOW_DIR_4, GRID_FLOW_NONE, GRID_FLOW_DIR_6 },
			{ GRID_FLOW_DIR_1, GRID_FLOW_DIR_2, GRID_FLOW_DIR_3 }
		};
		return directionMap[dv + 1][du + 1];
	}
}

GridFlowBuilder::GridFlowBuilder(GridFlowChunk& gridFlowChunk)
	:
	mGridFlowChunk(gridFlowChunk)
{
}

void GridFlowBuilder::BuildCostMap(IN OUT GridFlowTaskMap& flowTaskMap, int64_t targetU, int64_t targetV)
{
	targetU -= mGridFlowChunk.GetOffsetU();
	targetV -= mGridFlowChunk.GetOffsetV();

	const size_t targetIndex = GetIndex(targetU, targetV);
	GridFlowTask& target = flowTaskMap.at(targetIndex);
	if (target.cost == GRID_FLOW_COST_MAX)
		return;

	target.visited = true;

	// 8방향 탐색 방향 미리 정의
	static constexpr int64_t offsets[8][2] = {
		{-1, -1}, {0, -1}, {1, -1},
		{-1,  0},          {1,  0},
		{-1,  1}, {0,  1}, {1,  1}
	};

	std::queue<std::pair<int64_t, int64_t>> openList;
	openList.emplace(targetU, targetV);

	do
	{
		auto [u, v] = openList.front();
		openList.pop();
		const size_t index = GetIndex(u, v);
		const GridFlowTask& task = flowTaskMap.at(index);
		const GridFlowCost cost = task.cost;
		if (task.cost == GRID_FLOW_COST_MAX)
			continue;

		// 8방향 탐색
		for (int64_t i = 0; i < 8; ++i)
		{
			const int64_t du = offsets[i][0];
			const int64_t nu = u + du;
			if (nu < 0 || nu >= mGridFlowChunk.GetWidth())
				continue;

			const int64_t dv = offsets[i][1];
			const int64_t nv = v + dv;
			if (nv < 0 || nv >= mGridFlowChunk.GetHeight())
				continue;

			const size_t ni = GetIndex(nu, nv);
			GridFlowTask& neighbor = flowTaskMap.at(ni);
			GridFlowCost& neighborCost = neighbor.cost;
			if (neighborCost == GRID_FLOW_COST_MAX)
				continue;

			// 가로와 세로 모두 막힌 경우 대각선 이동 불가
			//if (du != 0 && dv != 0 &&
			//	flowTaskMap[GetIndex(nu, v)].cost == GRID_FLOW_COST_MAX &&
			//	flowTaskMap[GetIndex(u, nv)].cost == GRID_FLOW_COST_MAX)
			//{
			//	continue;
			//}

			const GridFlowCost moveCost = (du != 0 && dv != 0) ? GRID_FLOW_COST_DIAGONAL : GRID_FLOW_COST_STRAIT;
			const GridFlowCost newCost = (cost + moveCost);
			if (!neighbor.visited)
			{
				neighbor.visited = true;
				neighborCost = newCost;

				openList.emplace(nu, nv);
			}
			else if (neighborCost > newCost)
			{
				neighborCost = newCost;

				openList.emplace(nu, nv);
			}
		}
	} while (!openList.empty());
}

void GridFlowBuilder::BuildDirMap(IN OUT GridFlowTaskMap& flowTaskMap, int64_t targetU, int64_t targetV)
{
	targetU -= mGridFlowChunk.GetOffsetU();
	targetV -= mGridFlowChunk.GetOffsetV();

	const size_t targetIndex = GetIndex(targetU, targetV);
	GridFlowTask& target = flowTaskMap.at(targetIndex);
	if (target.cost == GRID_FLOW_COST_MAX)
		return;

	target.cost = 1;

	// 8방향 탐색 방향 미리 정의
	static constexpr int64_t offsets[8][2] = {
		{-1, -1}, {0, -1}, {1, -1},
		{-1,  0},          {1,  0},
		{-1,  1}, {0,  1}, {1,  1}
	};

	for (int64_t v = 0; v < mGridFlowChunk.GetHeight(); ++v)
	{
		for (int64_t u = 0; u < mGridFlowChunk.GetWidth(); ++u)
		{
			if (targetU == u && targetV == v)
				continue;

			const size_t currentIdx = GetIndex(u, v);
			const GridFlowTask& currentTask = flowTaskMap.at(currentIdx);
			if (currentTask.cost == GRID_FLOW_COST_MAX)
				continue;

			GridFlowCost minCost = GRID_FLOW_COST_MAX;
			GridFlowDir bestDir = GRID_FLOW_DIR_8;

			// 8방향 탐색
			for (int64_t i = 0; i < 8; ++i)
			{
				const int64_t du = offsets[i][0];
				const int64_t nu = u + du;
				if (nu < 0 || nu >= mGridFlowChunk.GetWidth())
					continue;

				const int64_t dv = offsets[i][1];
				const int64_t nv = v + dv;
				if (nv < 0 || nv >= mGridFlowChunk.GetHeight())
					continue;

				// 가로와 세로 모두 막힌 경우 대각선 이동 불가
				//if (du != 0 && dv != 0 &&
				//	flowTaskMap[GetIndex(nu, v)].cost == GRID_FLOW_COST_MAX &&
				//	flowTaskMap[GetIndex(u, nv)].cost == GRID_FLOW_COST_MAX)
				//{
				//	continue; 
				//}

				const size_t neighborIdx = GetIndex(nu, nv);
				const GridFlowTask& neighborTask = flowTaskMap.at(neighborIdx);
				//const GridFlowCost neighborCost = (neighborTask.cost == GRID_FLOW_COST_MAX) ? GRID_FLOW_COST_MAX : (neighborTask.cost + neighborTask.avoidanceCost);
				const GridFlowCost neighborCost = (neighborTask.cost == GRID_FLOW_COST_MAX) ? GRID_FLOW_COST_MAX : neighborTask.cost;

				if (minCost > neighborCost)
				{
					minCost = neighborCost;
					bestDir = CalculateDirection(du, dv);
				}
			}

			// 최적 방향 설정
			if (minCost != 0 && minCost != GRID_FLOW_COST_MAX)
			{
				mGridFlowChunk.SetFlowDir(u, v, targetU, targetV, bestDir);
			}
		}	
	}
}

