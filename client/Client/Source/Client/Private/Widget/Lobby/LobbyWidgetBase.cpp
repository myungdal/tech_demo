// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "LobbyWidgetBase.h"

#include "Components/WidgetSwitcher.h"

#include "Widget/Inventory/InventoryWidgetBase.h"
#include "Widget/Item/ItemDetailWidgetBase.h"
#include "Widget/GameListPopup/GameListWidgetBase.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "SubSystem/NetworkManager/NetworkManager.h"
#include "SubSystem/NetworkManager/PacketPtr/PacketPtr.h"
#include "Util/PacketUtil/PacketUtil.h"


void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	mInventory->SetVisibility(ESlateVisibility::Hidden);
	mLobbyWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void ULobbyWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void ULobbyWidgetBase::OnUiEvent(UUiEvent_MENU_INVENTORY_CLICKED& uiEvent)
{
	if (ESlateVisibility::Hidden == mInventory->GetVisibility())
	{
		UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_INVENTORY_OPEN>();
	}
	else
	{
		UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_INVENTORY_CLOSE_CLICKED>();
	}
}

void ULobbyWidgetBase::OnUiEvent(UUiEvent_INVENTORY_OPEN& uiEvent)
{
	mInventory->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyWidgetBase::OnUiEvent(UUiEvent_INVENTORY_CLOSE_CLICKED& uiEvent)
{
	mInventory->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyWidgetBase::OnUiEvent(UUiEvent_ITEM_DETAIL_LAUNCH_GAME_OPEN& uiEvent)
{
	mLobbyWidgetSwitcher->SetActiveWidget(mItemDetail);
	mLobbyWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyWidgetBase::OnUiEvent(UUiEvent_ITEM_DETAIL_REWARD_OPEN& uiEvent)
{
	mLobbyWidgetSwitcher->SetActiveWidget(mItemDetail);
	mLobbyWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyWidgetBase::OnUiEvent(UUiEvent_ITEM_DETAIL_STONE_OPEN& uiEvent)
{
	mLobbyWidgetSwitcher->SetActiveWidget(mItemDetail);
	mLobbyWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyWidgetBase::OnUiEvent(UUiEvent_ITEM_DETAIL_CLOSE_CLICKED& uiEvent)
{
	mLobbyWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyWidgetBase::OnUiEvent(UUiEvent_POPUP_GAME_LIST_OPEN& uiEvent)
{
	mLobbyWidgetSwitcher->SetActiveWidget(mGameList);
	mLobbyWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyWidgetBase::OnUiEvent(UUiEvent_POPUP_GAME_LIST_CLOSE_CLICKED& uiEvent)
{
	mLobbyWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyWidgetBase::OnUiEvent(UUiEvent_GAME_ENTER& uiEvent)
{
	mLobbyWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyWidgetBase::OnDispatchPacket(UPacketPtr* packetPtr)
{
	MAYBE_UNUSED const HandleResult handleResult = PacketUtil::DispatchPacket(this, *packetPtr->rp);
}
//
//HandleResult ULobbyWidgetBase::OnPacket(FC_ACK_GAME_LAUNCH& rp)
//{
//	const Result result = rp.GetHeader().GetPacketResult();
//	switch (result)
//	{
//		mLobbyWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
//	}
//
//	return HandleResult::OK;
//}
