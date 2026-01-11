// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "ItemDetailLaunchGameWidgetBase.h"

#include "Packet/UserCache/UserItem/UserItemRow.h"
#include "Packet/StaticData/Item/ItemDoc.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "UiService/Inventory/UiServiceInventory.h"
#include "Widget/Item/ItemWidgetBase.h"


void UItemDetailLaunchGameWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemDetailLaunchGameWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UItemDetailLaunchGameWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UItemDetailLaunchGameWidgetBase::OnUiEvent(UUiEvent_ITEM_DETAIL_RELOAD& uiEvent)
{
	if (const UserItemRow* userItemRow = UUiServiceManager::Get(this)->GetService<FUiServiceInventory>()->GetSelectedItem0())
	{
		ItemType itemType = userItemRow->GetDoc()->mStaticItem->Get_c_item_type();

		if (ItemType::LAUNCH_GAME != itemType)
			return;

		mItem->SetupItem(userItemRow, ItemWidgetContainerType::ITEM_DETAIL_LAUNCH_GAME, false);
	}
} 
