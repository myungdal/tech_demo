// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "CheatListRowWidgetBase.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"

#include "ClientUtil/ClientStringUtil.h"
#include "UiObject/RowData/UiRowDataString.h"


void UCheatListRowWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == mBorder->OnMouseButtonUpEvent.IsBoundToObject(this))
	{
		mBorder->OnMouseButtonUpEvent.BindUFunction(this, FName(TEXT("OnClicked")));
	}
}

void UCheatListRowWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCheatListRowWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UUiRowDataString* rowData = Cast<UUiRowDataString>(ListItemObject);
	//mEntityIdp = rowData->

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UCheatListRowWidgetBase::OnClicked()
{
	//UUiServiceManager::Get(this)->GetService<FUiServiceDebug>()->SetSelectedEntityIdp(mEntityIdp);
	//UUiServiceManager::Get(this)->GetService<FUiServiceDebug>()->SetSelectedComponentType(mComponentType);

	//UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_DEBUG_COMPONENT_SELECTED>();
} 
