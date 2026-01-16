// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "InventoryWidgetBase.h"

#include "Components/ListView.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "UiService/Inventory/UiServiceInventory.h"
#include "UiObject/RowData/UiRowDataItem.h"
#include "Widget/Button/ButtonWidgetBase.h"


void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	mAllBtn->SetupButton(UUiEvent_INVENTORY_TAB_ALL_CLICKED::TYPE, TEXT("All"));
	mStoneBtn->SetupButton(UUiEvent_INVENTORY_TAB_STONE_CLICKED::TYPE, TEXT("Stone"));
	mEtcBtn->SetupButton(UUiEvent_INVENTORY_TAB_ETC_CLICKED::TYPE, TEXT("Etc"));
	mCloseBtn->SetupButton(UUiEvent_INVENTORY_CLOSE_CLICKED::TYPE, TEXT("Close"));
}

void UInventoryWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UInventoryWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UInventoryWidgetBase::OnUiEvent(UUiEvent_INVENTORY_ITEM_LIST_RELOAD& uiEvent)
{
	mInventoryView->ClearListItems();

	const TArray<FUiServiceInventory::InventoryItem>& itemInfoList = UUiServiceManager::Get(this)->GetService<FUiServiceInventory>()->GetInventoryItemList();

	for (int32 i = 0; i < itemInfoList.Num(); i += 4)
	{
		UUiRowDataItem* inventoryItem = NewObject<UUiRowDataItem>();

		for (int32 j = 0; (j < 4) && ((i + j) < itemInfoList.Num()); ++j)
		{
			//inventoryItem->mData[j] = itemInfoList[i + j].first;
			inventoryItem->mRedDot[j] = itemInfoList[i + j].second.mRedDot;
			inventoryItem->mDisabled[j] = itemInfoList[i + j].second.mDisabled;
		}

		mInventoryView->AddItem(inventoryItem);
	}
}

void UInventoryWidgetBase::OnUiEvent(UUiEvent_INVENTORY_TAB_STATE_UPDATE& uiEvent)
{
	mAllBtn->SetIsEnabled(true);
	mStoneBtn->SetIsEnabled(true);
	mEtcBtn->SetIsEnabled(true);

	switch (UUiServiceManager::Get(this)->GetService<FUiServiceInventory>()->GetCurrentTabBtnEventType())
	{
	case UUiEvent_INVENTORY_TAB_ALL_CLICKED::TYPE:
	{
		mAllBtn->SetIsEnabled(false);
		break;
	}
	case UUiEvent_INVENTORY_TAB_STONE_CLICKED::TYPE:
	{
		mStoneBtn->SetIsEnabled(false);
		break;
	}
	case UUiEvent_INVENTORY_TAB_ETC_CLICKED::TYPE:
	{
		mEtcBtn->SetIsEnabled(false);
		break;
	}
	default:
	{
		break;
	}
	}
	
}
