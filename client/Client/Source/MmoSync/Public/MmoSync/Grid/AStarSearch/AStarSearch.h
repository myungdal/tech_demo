// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/AStarSearch/Internal/AStarDef.h"
#include "MmoSync/Grid/AStarSearch/Internal/AStarNode.h"
#include "Common/MemPool/DynamicPool.h"


class AStarSearch
{
private:
	AStarNode* mCurrentSolutionNode = nullptr;
	DynamicPool<AStarNode> mNodePool;

private:
	AStarResult mResult = AStarResult::NOT_INITIALISED;
	int mSteps = 0;
	int mMaxSteps = 0;

	AStarNode* mStart = nullptr;
	AStarNode* mGoal = nullptr;

#if UE_EDITOR || defined(_DEBUG)
	std::set<AStarNode*> mActiveNodeCount;
#endif

private:
	std::deque<AStarNode*> mOpenList;
	std::deque<AStarNode*> mClosedList;
	std::deque<AStarNode*> mSuccessors;

public:
	explicit AStarSearch();
	virtual ~AStarSearch();

public:
	void SetStartAndGoalStates(AStarState&& startState, AStarState&& goalState);

	AStarState* GetSolutionStart();
	AStarState* GetSolutionNext();
	AStarState* GetSolutionEnd();
	AStarState* GetSolutionPrev();

	int GetSolutionCost();

public:
	void FreeAllNodes();
	void FreeUnusedNodes();
	AStarNode* AllocateNode();
	void FreeNode(AStarNode* node);

public:
	template<typename _IsBlocked>
	AStarResult SearchStep(_IsBlocked IsBlocked)
	{
		if (mResult == AStarResult::SUCCEEDED)
			return mResult;

		if (mResult == AStarResult::FAILED)
			return mResult;

		if (mOpenList.empty())
		{
			mResult = AStarResult::FAILED;
			return mResult;
		}

		++mSteps;
		if (mSteps > mMaxSteps)
		{
			_DEBUG_LOG(RED, L"mSteps: {}, mMaxSteps: {}", mSteps, mMaxSteps);

			mResult = AStarResult::FAILED;
			return mResult;
		}

		AStarNode* n = mOpenList.front();
		pop_heap(mOpenList.begin(), mOpenList.end(), AStarNodeHeapCompare());
		mOpenList.pop_back();

		_ASSERT_CRASH(n == mStart || n->parent != nullptr);

		if (n->state.IsGoal(mGoal->state))
		{
			mGoal->parent = n->parent;
			mGoal->g = n->g;

			if (false == n->state.IsSameState(mStart->state))
			{
				FreeNode(n);
				AStarNode* nodeChild = mGoal;
				AStarNode* nodeParent = mGoal->parent;

				do
				{
					nodeParent->child = nodeChild;
					nodeChild = nodeParent;
					nodeParent = nodeParent->parent;

				} while (nodeChild != mStart);
			}

			FreeUnusedNodes();

			mResult = AStarResult::SUCCEEDED;
			return mResult;
		}
		else
		{
			mSuccessors.clear();

			AStarState* parentNode = n->parent ? &n->parent->state : nullptr;
			const bool ret = n->state.GetSuccessors<_IsBlocked>(parentNode, IsBlocked,
				[this](AStarState&& successor)
				{
					if (AStarNode* node = AllocateNode())
					{
						node->state = successor;
						mSuccessors.push_back(node);
					}
				}
			);

			if (!ret)
			{
				typename std::deque<AStarNode*>::iterator successor;
				for (successor = mSuccessors.begin(); successor != mSuccessors.end(); successor++)
				{
					AStarNode* successorNode = *successor;
					FreeNode(successorNode);
				}

				mSuccessors.clear();
				FreeNode(n);
				FreeAllNodes();
				mResult = AStarResult::OUT_OF_MEMORY;
				return mResult;
			}

			for (typename std::deque<AStarNode*>::iterator successor = mSuccessors.begin(); successor != mSuccessors.end(); ++successor)
			{
				AStarNode* successorNode = (*successor);

				int g = (n->g + 1);

				typename std::deque<AStarNode*>::iterator opened = mOpenList.begin();
				for (; opened != mOpenList.end(); ++opened)
				{
					if ((*opened)->state.IsSameState(successorNode->state))
						break;
				}

				if (opened != mOpenList.end())
				{
					if ((*opened)->g <= g)
					{
						FreeNode(successorNode);
						continue;
					}
				}

				typename std::deque<AStarNode*>::iterator closed = mClosedList.begin();
				for (; closed != mClosedList.end(); ++closed)
				{
					if ((*closed)->state.IsSameState(successorNode->state))
						break;
				}

				if (closed != mClosedList.end())
				{
					if ((*closed)->g <= g)
					{
						FreeNode(successorNode);
						continue;
					}
				}

				successorNode->parent = n;
				successorNode->g = g;
				successorNode->h = successorNode->state.GoalDistanceEstimate(mGoal->state);
				successorNode->f = successorNode->g + successorNode->h;

				if (closed != mClosedList.end())
				{
					(*closed)->parent = successorNode->parent;
					(*closed)->g = successorNode->g;
					(*closed)->h = successorNode->h;
					(*closed)->f = successorNode->f;

					_ASSERT_CRASH((*closed) == mStart || (*closed)->parent != nullptr);

					FreeNode(successorNode);
					mOpenList.push_back((*closed));
					mClosedList.erase(closed);
					push_heap(mOpenList.begin(), mOpenList.end(), AStarNodeHeapCompare());

				}
				else if (opened != mOpenList.end())
				{
					(*opened)->parent = successorNode->parent;
					(*opened)->g = successorNode->g;
					(*opened)->h = successorNode->h;
					(*opened)->f = successorNode->f;

					FreeNode(successorNode);
					make_heap(mOpenList.begin(), mOpenList.end(), AStarNodeHeapCompare());
				}
				else
				{
					_ASSERT_CRASH(successorNode == mStart || successorNode->parent != nullptr);

					mOpenList.push_back(successorNode);
					push_heap(mOpenList.begin(), mOpenList.end(), AStarNodeHeapCompare());
				}

			}

			mClosedList.push_back(n);
		}

		return mResult;
	}
};
