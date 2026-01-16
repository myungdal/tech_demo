// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "GameListWidgetBase.h"

#include "Components/ListView.h"

#include "SubSystem/NetworkManager/NetworkManager.h"
#include "SubSystem/NetworkContext/NetworkContext.h"
#include "SubSystem/NetworkManager/PacketPtr/PacketPtr.h"
#include "UiObject/RowData/UiRowDataGame.h"
#include "Util/PacketUtil/PacketUtil.h"
#include "Widget/Button/ButtonWidgetBase.h"
#include "Widget/GameListPopup/GameListGameDetailWidgetBase.h"


void UGameListWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	mCloseBtn->SetupButton(UUiEvent_POPUP_GAME_LIST_CLOSE_CLICKED::TYPE, TEXT("X"));
	mOkBtn->SetupButton(UUiEvent_POPUP_GAME_LIST_OK_CLICKED::TYPE, TEXT("Enter"));

	mGameDetail->SetVisibility(ESlateVisibility::Hidden);
}

void UGameListWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGameListWidgetBase::OnDispatchPacket(UPacketPtr* packetPtr)
{
	MAYBE_UNUSED const HandleResult handleResult = PacketUtil::DispatchPacket(this, *packetPtr->rp);
}

HandleResult UGameListWidgetBase::OnPacket(MC_ACK_GAME_LIST_OVER& rp)
{
	mGameListView->ClearListItems();
	
	for (auto& [gameId, game] : UNetworkContext::Get(this)->GetGameList())
	{
		UUiRowDataGame* elem = NewObject<UUiRowDataGame>();
		elem->mData = *game;
		mGameListView->AddItem(elem);
	}	

	if (mGameListView->GetNumItems() > 0)
	{
		const TArray<UObject*>& listItems = mGameListView->GetListItems();
		UObject* firstItem = listItems[0];
		mGameListView->SetSelectedItem(firstItem);
	}

	return HandleResult::OK;
}
