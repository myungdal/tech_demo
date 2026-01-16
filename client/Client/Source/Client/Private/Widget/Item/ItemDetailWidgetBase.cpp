// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "ItemDetailWidgetBase.h"

#include "Components/WidgetSwitcher.h"

#include "Widget/Item/ItemDetailLaunchGameWidgetBase.h"
#include "Widget/Item/ItemDetailRewardWidgetBase.h"
#include "Widget/Item/ItemDetailStoneWidgetBase.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"


void UItemDetailWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	mItemDetailWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void UItemDetailWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UItemDetailWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UItemDetailWidgetBase::OnUiEvent(UUiEvent_ITEM_DETAIL_LAUNCH_GAME_OPEN& uiEvent)
{
	mItemDetailWidgetSwitcher->SetActiveWidget(mItemDetailLaunchGame);
	mItemDetailWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
}

void UItemDetailWidgetBase::OnUiEvent(UUiEvent_ITEM_DETAIL_REWARD_OPEN& uiEvent)
{
	mItemDetailWidgetSwitcher->SetActiveWidget(mItemDetailReward);
	mItemDetailWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
}

void UItemDetailWidgetBase::OnUiEvent(UUiEvent_ITEM_DETAIL_STONE_OPEN& uiEvent)
{
	mItemDetailWidgetSwitcher->SetActiveWidget(mItemDetailStone);
	mItemDetailWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
}
