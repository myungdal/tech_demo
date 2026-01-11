// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Grid/AStarSearch/AStarSearch.h"

#include "MmoSync/DebugControlTool/DebugInfo.h"


AStarSearch::AStarSearch()
{
}
AStarSearch::~AStarSearch()
{
	FreeAllNodes();
}
void AStarSearch::SetStartAndGoalStates(AStarState&& startState, AStarState&& goalState)
{
	FreeAllNodes();

	mStart = AllocateNode();
	mGoal = AllocateNode();

	mStart->state = startState;
	mGoal->state = goalState;

	mResult = AStarResult::SEARCHING;

	mStart->g = 0;
	mStart->h = mStart->state.GoalDistanceEstimate(mGoal->state);
	mStart->f = (mStart->g + mStart->h);
	mStart->parent = nullptr;

	mOpenList.push_back(mStart);

	push_heap(mOpenList.begin(), mOpenList.end(), AStarNodeHeapCompare());

	mSteps = 0;

	mMaxSteps = (mStart->h * DebugInfo::MOVE_TO_ASTAR_STEP);

	if (mStart->state.IsSameState(mGoal->state))
	{
		mStart->child = mGoal;
		mGoal->parent = mStart;
		mGoal->g = 0;
		mOpenList.clear();
		mResult = AStarResult::SUCCEEDED;
	}
}

AStarState* AStarSearch::GetSolutionStart()
{
	mCurrentSolutionNode = mStart;
	return (mStart ? &mStart->state : nullptr);
}

AStarState* AStarSearch::GetSolutionNext()
{
	if (!mCurrentSolutionNode)
		return nullptr;

	if (!mCurrentSolutionNode->child)
		return nullptr;

	AStarNode* child = mCurrentSolutionNode->child;
	mCurrentSolutionNode = mCurrentSolutionNode->child;
	return &child->state;
}

AStarState* AStarSearch::GetSolutionEnd()
{
	mCurrentSolutionNode = mGoal;
	return (mGoal ? &mGoal->state : nullptr);
}

AStarState* AStarSearch::GetSolutionPrev()
{
	if (!mCurrentSolutionNode)
		return nullptr;

	if (!mCurrentSolutionNode->parent)
		return nullptr;

	AStarNode* parent = mCurrentSolutionNode->parent;
	mCurrentSolutionNode = mCurrentSolutionNode->parent;
	return &parent->state;
}

int AStarSearch::GetSolutionCost()
{
	if (!mGoal || mResult != AStarResult::SUCCEEDED)
		return -1;

	return mGoal->g;
}

void AStarSearch::FreeAllNodes()
{
	if (!mOpenList.empty() || !mClosedList.empty())
	{
		for (AStarNode* n : mOpenList) FreeNode(n);
		for (AStarNode* n : mClosedList) FreeNode(n);

		mOpenList.clear();
		mClosedList.clear();

		if (mGoal)
			FreeNode(mGoal);
	}
	else
	{
		AStarNode* n = mStart;
		while (n)
		{
			AStarNode* next = n->child;
			FreeNode(n);
			if (n == mGoal) break;
			n = next;
		}
	}

#if UE_EDITOR || defined(_DEBUG)
	_ASSERT_CRASH(mActiveNodeCount.size() == 0);
#endif

	mStart = mGoal = mCurrentSolutionNode = nullptr;
	mSteps = mMaxSteps = 0;
}

void AStarSearch::FreeUnusedNodes()
{
	for (AStarNode* node : mOpenList)
	{
		if (node->child)
			continue;

		FreeNode(node);
	}

	mOpenList.clear();

	for (AStarNode* node : mClosedList)
	{
		if (node->child)
			continue;

		FreeNode(node);
	}

	mClosedList.clear();
}

AStarNode* AStarSearch::AllocateNode()
{
	AStarNode* node = mNodePool.Pop();

#if UE_EDITOR || defined(_DEBUG)
	mActiveNodeCount.emplace(node);

	if (node->parent != nullptr) _ASSERT_CRASH(false);
	if (node->child != nullptr) _ASSERT_CRASH(false);

	if (node->g != 0) _ASSERT_CRASH(false);
	if (node->h != 0) _ASSERT_CRASH(false);
	if (node->f != 0) _ASSERT_CRASH(false);
#endif

	return node;
}

void AStarSearch::FreeNode(AStarNode* node)
{
	if (node)
	{
#if UE_EDITOR || defined(_DEBUG)
		if (0 == mActiveNodeCount.erase(node))
			_ASSERT_CRASH(false);

		//node->parent = nullptr;
		//node->child = nullptr;

		//node->g = 0;
		//node->h = 0;
		//node->f = 0;
#endif

		mNodePool.Push(node);
	}
}
