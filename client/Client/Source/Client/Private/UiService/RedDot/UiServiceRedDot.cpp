// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "UiServiceRedDot.h"

#include "UiService/RedDot/Internal/UiRedDotNode.h"
#include "SubSystem/UiServiceManager/UiServiceManager.h"


FUiServiceRedDot::FUiServiceRedDot()
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

FUiServiceRedDot::~FUiServiceRedDot()
{
	for (FUiRedDotNode* node : mRedDotNodeArray)
	{
		delete node;
	}
}

void FUiServiceRedDot::AttachChildToParent(RedDotType child, RedDotType parent)
{
	FUiRedDotNode* parentNode = mRedDotNodeArray[static_cast<size_t>(parent)];
	FUiRedDotNode* childNode = mRedDotNodeArray[static_cast<size_t>(child)];

	parentNode->mChildList.Emplace(childNode);
	childNode->mParent = parentNode;
}

bool FUiServiceRedDot::HasRedDot(RedDotType redDotType) const
{
	FUiRedDotNode* node = mRedDotNodeArray[static_cast<size_t>(redDotType)];
	return node->mRedDot;
}

void FUiServiceRedDot::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void FUiServiceRedDot::OnUiEvent(UUiEvent_RED_DOT_ADD& uiEvent)
{
	FUiRedDotNode* node = mRedDotNodeArray[static_cast<size_t>(uiEvent.GetMsg()->mRedDotType)];
	node->AddRedDot();

	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_INVENTORY_TAB_STATE_UPDATE>();
}

void FUiServiceRedDot::OnUiEvent(UUiEvent_RED_DOT_REMOVE& uiEvent)
{
	FUiRedDotNode* node = mRedDotNodeArray[static_cast<size_t>(uiEvent.GetMsg()->mRedDotType)];
	node->TryRemoveRedDot();

	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_INVENTORY_TAB_STATE_UPDATE>();
}
