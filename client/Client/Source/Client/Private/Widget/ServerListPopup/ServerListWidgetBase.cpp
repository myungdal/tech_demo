// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "ServerListWidgetBase.h"

#include "Components/ListView.h"

#include "SubSystem/NetworkManager/NetworkManager.h"
#include "SubSystem/NetworkContext/NetworkContext.h"
#include "UiObject/RowData/UiRowDataServer.h"
#include "Widget/Button/ButtonWidgetBase.h"


void UServerListWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	mCloseBtn->SetupButton(UUiEvent_POPUP_SERVER_LIST_CLOSE_CLICKED::TYPE, TEXT("Close"));
	mOkBtn->SetupButton(UUiEvent_POPUP_SERVER_LIST_OK_CLICKED::TYPE, TEXT("OK"));
}

void UServerListWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UServerListWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UServerListWidgetBase::OnUiEvent(UUiEvent_POPUP_SERVER_LIST_OPEN& uiEvent)
{
	mUserCreateMode = uiEvent.GetMsg()->mUserCreateMode;

	mCloseBtn->SetIsEnabled(mUserCreateMode ? false : true);
	mOkBtn->SetIsEnabled(true);

	mServerListView->ClearListItems();

	for (const PacketKeep<SERVER>& server : UNetworkContext::Get(this)->ServerList())
	{
		UUiRowDataServer* elem = NewObject<UUiRowDataServer>();
		elem->mData = *server;
		mServerListView->AddItem(elem);
	}

	// 첫 번째 서버 자동 선택
	if (mServerListView->GetNumItems() > 0)
	{
		const TArray<UObject*>& listItems = mServerListView->GetListItems();
		UObject* firstItem = listItems[0];
		mServerListView->SetSelectedItem(firstItem);
	}
}
