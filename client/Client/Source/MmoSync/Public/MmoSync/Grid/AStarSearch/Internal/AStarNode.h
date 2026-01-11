// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/AStarSearch/Internal/AStarState.h"


struct AStarNode
{
	AStarNode* parent = nullptr;
	AStarNode* child = nullptr;

	int g = 0;
	int h = 0;
	int f = 0;

	AStarState state{};
};


struct AStarNodeHeapCompare
{
	bool operator() (const AStarNode* x, const AStarNode* y) const
	{
		return x->f > y->f;
	}
};
