// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "ItemDetailStoneWidgetBase.h"

#include "Packet/UserCache/UserItem/UserItemRow.h"
#include "Packet/StaticData/Item/ItemDoc.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "UiService/Inventory/UiServiceInventory.h"
#include "Widget/Item/ItemWidgetBase.h"


void UItemDetailStoneWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemDetailStoneWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UItemDetailStoneWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UItemDetailStoneWidgetBase::OnUiEvent(UUiEvent_ITEM_DETAIL_RELOAD& uiEvent)
{
	mItem0->SetupItem(nullptr, ItemWidgetContainerType::ITEM_DETAIL_STONE, false);
	mItem1->SetupItem(nullptr, ItemWidgetContainerType::ITEM_DETAIL_STONE, false);

	if (const UserItemRow* userItemRow = UUiServiceManager::Get(this)->GetService<FUiServiceInventory>()->GetSelectedItem0())
	{
		if (ItemType::STONE == userItemRow->GetDoc()->mStaticItem->Get_c_item_type())
			mItem0->SetupItem(userItemRow, ItemWidgetContainerType::ITEM_DETAIL_STONE, false);
	}

	if (const UserItemRow* userItemRow = UUiServiceManager::Get(this)->GetService<FUiServiceInventory>()->GetSelectedItem1())
	{
		if (ItemType::STONE == userItemRow->GetDoc()->mStaticItem->Get_c_item_type())
			mItem1->SetupItem(userItemRow, ItemWidgetContainerType::ITEM_DETAIL_STONE, false);
	}
} 
