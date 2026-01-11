// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Grid/AStarSearch/Internal/AStarState.h"

#include "MmoSync/Grid/AStarSearch/AStarSearch.h"


bool AStarState::IsSameState(AStarState& other) const 
{
	return (x == other.x) && (y == other.y);
}

int AStarState::GoalDistanceEstimate(AStarState& goal) const
{
	return abs(x - goal.x) + abs(y - goal.y);
}

bool AStarState::IsGoal(AStarState& goal) const
{
	return (x == goal.x) && (y == goal.y);
}
