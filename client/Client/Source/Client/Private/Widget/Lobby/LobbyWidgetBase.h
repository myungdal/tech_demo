// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "LobbyWidgetBase.generated.h"


class UWidgetSwitcher;
class UDebugControllerWidgetBase;
class UWalletWidgetBase;
class UGameListWidgetBase;
class UInventoryWidgetBase;
class UMenuWidgetBase;
class UItemDetailWidgetBase;

/**
 * 로비 UI 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class ULobbyWidgetBase : public UWidgetBase
{
	GENERATED_BODY()

	// LeftTop
public:

	// Top
public:
	UPROPERTY(meta = (BindWidget))
	UWalletWidgetBase* mWallet = nullptr;

	// RightTop
public:
	UPROPERTY(meta = (BindWidget))
	UMenuWidgetBase* mMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UInventoryWidgetBase* mInventory = nullptr;

	// Window
public:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* mLobbyWidgetSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	UItemDetailWidgetBase* mItemDetail = nullptr;

	UPROPERTY(meta = (BindWidget))
	UGameListWidgetBase* mGameList = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}

	void OnUiEvent(UUiEvent_MENU_INVENTORY_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_INVENTORY_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_INVENTORY_CLOSE_CLICKED& uiEvent);

	void OnUiEvent(UUiEvent_ITEM_DETAIL_LAUNCH_GAME_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_ITEM_DETAIL_REWARD_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_ITEM_DETAIL_STONE_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_ITEM_DETAIL_CLOSE_CLICKED& uiEvent);

	void OnUiEvent(UUiEvent_POPUP_GAME_LIST_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_POPUP_GAME_LIST_CLOSE_CLICKED& uiEvent);

	void OnUiEvent(UUiEvent_GAME_ENTER& uiEvent);

public:
	void OnDispatchPacket(UPacketPtr* packetPtr) override;
	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
	//HandleResult OnPacket(class FC_ACK_GAME_LAUNCH& rp);
};

