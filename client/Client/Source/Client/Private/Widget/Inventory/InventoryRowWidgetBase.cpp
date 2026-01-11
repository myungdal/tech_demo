// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "InventoryRowWidgetBase.h"

#include "Widget/Item/ItemWidgetBase.h"
#include "UiObject/RowData/UiRowDataItem.h"


void UInventoryRowWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UUiRowDataItem* rowData = Cast<UUiRowDataItem>(ListItemObject);
	
	SetItemIconImg(mItem0, rowData->mData[0], rowData->mRedDot[0]);
	SetItemIconImg(mItem1, rowData->mData[1], rowData->mRedDot[1]);
	SetItemIconImg(mItem2, rowData->mData[2], rowData->mRedDot[2]);
	SetItemIconImg(mItem3, rowData->mData[3], rowData->mRedDot[3]);

	mItem0->SetIsEnabled(!rowData->mDisabled[0]);
	mItem1->SetIsEnabled(!rowData->mDisabled[1]);
	mItem2->SetIsEnabled(!rowData->mDisabled[2]);
	mItem3->SetIsEnabled(!rowData->mDisabled[3]);

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UInventoryRowWidgetBase::SetItemIconImg(UItemWidgetBase* target, const UserItemRow* userItemRow, bool redDot)
{
	target->SetupItem(userItemRow, ItemWidgetContainerType::INVENTORY, redDot);
} 
