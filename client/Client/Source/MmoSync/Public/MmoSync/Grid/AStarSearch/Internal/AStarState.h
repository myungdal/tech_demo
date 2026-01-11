// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


struct AStarState
{
	int x = 0;
	int y = 0;

	bool IsSameState(AStarState& other) const;
	int GoalDistanceEstimate(AStarState& goal) const;
	bool IsGoal(AStarState& goal) const;

	template<typename _IsBlocked, typename _AddSuccessor>
	bool GetSuccessors(AStarState* parent, _IsBlocked IsBlocked, _AddSuccessor AddSuccessor)
	{
		int parentX = -1;
		int parentY = -1;

		if (parent)
		{
			parentX = parent->x;
			parentY = parent->y;
		}

		if (!IsBlocked(x - 1, y) && (parentX != (x - 1) || parentY != y))
			AddSuccessor(AStarState{ x - 1, y });

		if (!IsBlocked(x, y - 1) && (parentX != x || parentY != (y - 1)))
			AddSuccessor(AStarState{ x, y - 1 });

		if (!IsBlocked(x + 1, y) && (parentX != (x + 1) || parentY != y))
			AddSuccessor(AStarState{ x + 1, y });

		if (!IsBlocked(x, y + 1) && (parentX != x || parentY != (y + 1)))
			AddSuccessor(AStarState{ x, y + 1 });

		return true;
	}
};
