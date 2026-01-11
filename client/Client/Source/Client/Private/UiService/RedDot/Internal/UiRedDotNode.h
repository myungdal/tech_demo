// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Containers/Array.h"


struct FUiRedDotNode
{
	using ChildList = TArray<FUiRedDotNode*>;
	ChildList mChildList;

	FUiRedDotNode* mParent = nullptr;

	bool mRedDot = false;

public:
	void AddRedDot();
	void TryRemoveRedDot();

private:
	bool HasRedDot();
};
