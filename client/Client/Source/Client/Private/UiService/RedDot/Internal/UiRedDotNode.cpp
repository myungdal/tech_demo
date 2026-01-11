// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "UiRedDotNode.h"

void FUiRedDotNode::AddRedDot()
{
	if (true == mRedDot)
		return;

	mRedDot = true;

	if (nullptr != mParent)
		mParent->AddRedDot();
}

void FUiRedDotNode::TryRemoveRedDot()
{
	if (false == mRedDot)
		return;

	mRedDot = false;
	if (true == HasRedDot())
		mRedDot = true;

	if (nullptr != mParent)
		mParent->TryRemoveRedDot();
}

bool FUiRedDotNode::HasRedDot()
{
	if (true == mRedDot)
		return true;

	for (FUiRedDotNode* child : mChildList)
	{
		if (true == child->mRedDot)
			return true;
	}

	return false;
}
