// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/GridFlow/Internal/GridFlowChunk.h"


class GridFlowBuilder
{
private:
	GridFlowChunk& mGridFlowChunk;

public:
	explicit GridFlowBuilder(GridFlowChunk& gridFlowChunk);

private:
	void BuildCostMap(IN OUT GridFlowTaskMap& flowTaskMap, int64_t targetU, int64_t targetV);
	void BuildDirMap(IN OUT GridFlowTaskMap& flowTaskMap, int64_t targetU, int64_t targetV);

private:
	inline size_t GetIndex(int64_t u, int64_t v) const noexcept { return (v * mGridFlowChunk.GetWidth() + u); }

public:
	template<typename _IsBlocked>
	void InitializeTaskMap(_IsBlocked IsBlocked)
	{
		GridFlowTaskMap& draft = mGridFlowChunk.RefTaskMapDraft();
		std::fill(draft.begin(), draft.end(), GridFlowTask{});

		for (int64_t v = 0; v < mGridFlowChunk.GetHeight(); ++v)
		{
			for (int64_t u = 0; u < mGridFlowChunk.GetWidth(); ++u)
			{
				if (IsBlocked(u + mGridFlowChunk.GetOffsetU(), v + mGridFlowChunk.GetOffsetV()))
				{
					draft.at(GetIndex(u, v)).cost = GRID_FLOW_COST_MAX;
				}
			}
		}

		static constexpr int64_t offsets[8][2] = {
			{-1, -1}, {0, -1}, {1, -1},
			{-1,  0},          {1,  0},
			{-1,  1}, {0,  1}, {1,  1}
		};

		for (int64_t v = 0; v < mGridFlowChunk.GetHeight(); ++v)
		{
			for (int64_t u = 0; u < mGridFlowChunk.GetWidth(); ++u)
			{
				GridFlowTask& task = draft.at(GetIndex(u, v));
				if (task.cost != 0)
					continue;

				for (int64_t i = 0; i < 8; ++i)
				{
					const int64_t du = offsets[i][0];
					const int64_t dv = offsets[i][1];
					if (du == 0 && dv == 0)
						continue;

					int64_t nu = (u + du);
					int64_t nv = (v + dv);
					if (nu < 0 || nu >= mGridFlowChunk.GetWidth() ||
						nv < 0 || nv >= mGridFlowChunk.GetHeight())
					{
						continue;
					}

					const GridFlowTask& neighborTask = draft.at(GetIndex(nu, nv));
					if (neighborTask.cost == 0)
						continue;

					//task.avoidanceCost = GRID_FLOW_COST_AVOIDANCE;
				}
			}
		}
	}

	inline void UpdateFlowField(int64_t u, int64_t v)
	{
		GridFlowTaskMap& draft = mGridFlowChunk.RefTaskMapDraft();
		GridFlowTaskMap& flowTaskMap = mGridFlowChunk.RefTaskMap();

		// draft 내용을 flowTaskMap에 복사
		std::copy(draft.begin(), draft.end(), flowTaskMap.begin());
		mGridFlowChunk.ClearFlowDirs(u, v);

		// 필요한 작업 수행
		BuildCostMap(IN OUT flowTaskMap, u, v);
		BuildDirMap(IN OUT flowTaskMap, u, v);
	}
};
