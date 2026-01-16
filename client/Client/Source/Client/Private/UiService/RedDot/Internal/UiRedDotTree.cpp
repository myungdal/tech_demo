// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "UiRedDotTree.h"

#include "UiService/RedDot/Internal/UiRedDotNode.h"


FUiRedDotTree::FUiRedDotTree()
{
	for (FUiRedDotNode*& node : mRedDotNodeArray)
	{
		node = new FUiRedDotNode();
	}

	AttachChildToParent(RedDotType::MAIL, RedDotType::MENU);
	AttachChildToParent(RedDotType::MISSION, RedDotType::MENU);
	AttachChildToParent(RedDotType::ACHIEVEMENT, RedDotType::MENU);
	AttachChildToParent(RedDotType::RANKING, RedDotType::MENU);
}

FUiRedDotTree::~FUiRedDotTree()
{
	for (FUiRedDotNode* node : mRedDotNodeArray)
	{
		delete node;
	}
}

void FUiRedDotTree::AttachChildToParent(RedDotType child, RedDotType parent)
{
	FUiRedDotNode* parentNode = mRedDotNodeArray[static_cast<size_t>(parent)];
	FUiRedDotNode* childNode = mRedDotNodeArray[static_cast<size_t>(child)];

	parentNode->mChildList.Emplace(childNode);
	childNode->mParent = parentNode;
}

void FUiRedDotTree::AddRedDot(RedDotType redDotType)
{
	FUiRedDotNode* node = mRedDotNodeArray[static_cast<size_t>(redDotType)];
	node->AddRedDot();
}

void FUiRedDotTree::RemoveRedDot(RedDotType redDotType)
{
	FUiRedDotNode* node = mRedDotNodeArray[static_cast<size_t>(redDotType)];
	node->TryRemoveRedDot();
}

bool FUiRedDotTree::HasRedDot(RedDotType redDotType) const
{
	FUiRedDotNode* node = mRedDotNodeArray[static_cast<size_t>(redDotType)];
	return node->mRedDot;
}
