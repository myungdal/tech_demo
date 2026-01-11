// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "ItemDetailRewardWidgetBase.h"

#include "Packet/UserCache/UserItem/UserItemRow.h"
#include "Packet/StaticData/Item/ItemDoc.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "UiService/Inventory/UiServiceInventory.h"
#include "Widget/Item/ItemWidgetBase.h"


void UItemDetailRewardWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemDetailRewardWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UItemDetailRewardWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UItemDetailRewardWidgetBase::OnUiEvent(UUiEvent_ITEM_DETAIL_RELOAD& uiEvent)
{
	if (const UserItemRow* userItemRow = UUiServiceManager::Get(this)->GetService<FUiServiceInventory>()->GetSelectedItem0())
	{
		ItemType itemType = userItemRow->GetDoc()->mStaticItem->Get_c_item_type();

		if (ItemType::REWARD_BOX != itemType)
			return;

		mItem->SetupItem(userItemRow, ItemWidgetContainerType::ITEM_DETAIL_REWARD, false);
	}
} 
