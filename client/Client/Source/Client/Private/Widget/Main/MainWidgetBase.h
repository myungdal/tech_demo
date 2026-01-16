// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "UiService/Game/UiServiceGame.h"
#include "MainWidgetBase.generated.h"


class UButton;
class UWidgetSwitcher;

class UMultiPlayViewportWidgetBase;
class USinglePlayViewportWidgetBase;
class UTestPlayViewportWidgetBase;
class UDebugWidgetBase;
class UCheatWidgetBase;
class ULobbyWidgetBase;
class ULoginPopupWidgetBase;
class UServerListWidgetBase;
class UUserNamePopupWidgetBase;

/**
 * 메인 UI 루트 위젯.
 * - 뷰포트(플레이 모드) 전환과 메인 팝업(로그인/서버목록 등)을 관리합니다.
 */
UCLASS(Abstract, Blueprintable)
class UMainWidgetBase : public UWidgetBase
{
	GENERATED_BODY()

public:
	// ========== Viewport Switcher ==========
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* mViewportWidgetSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	UMultiPlayViewportWidgetBase* mMultiPlayViewport = nullptr;

	UPROPERTY(meta = (BindWidget))
	USinglePlayViewportWidgetBase* mSinglePlayViewport = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTestPlayViewportWidgetBase* mTestPlayViewport = nullptr;

	UPROPERTY(meta = (BindWidget))
	UDebugWidgetBase* mDebug = nullptr;

	UPROPERTY(meta = (BindWidget))
	UCheatWidgetBase* mCheat = nullptr;

	UPROPERTY(meta = (BindWidget))
	ULobbyWidgetBase* mLobby = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* mLoading = nullptr; // 로딩 중 입력을 차단하기 위한 오버레이(클릭 흡수).

	// Main Popup
public:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* mMainWidgetSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	ULoginPopupWidgetBase* mLoginPopup = nullptr;

	UPROPERTY(meta = (BindWidget))
	UServerListWidgetBase* mServerList = nullptr;

	UPROPERTY(meta = (BindWidget))
	UUserNamePopupWidgetBase* mUserNamePopup = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_LOADING_START& uiEvent);
	void OnUiEvent(UUiEvent_LOADING_END& uiEvent);
	void OnUiEvent(UUiEvent_TEST_GAME_ENTER_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_TEST_GAME_LEAVE_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_GAME_ENTER& uiEvent);
	void OnUiEvent(UUiEvent_GAME_LEAVE& uiEvent);
	void OnUiEvent(UUiEvent_DEBUG_TOGGLE_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_CHEAT_TOGGLE_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_CONNECTED& uiEvent);
	void OnUiEvent(UUiEvent_DISCONNECTED& uiEvent);
	void OnUiEvent(UUiEvent_FATAL_ERROR& uiEvent);
	void OnUiEvent(UUiEvent_MENU_EXIT_CLICKED& uiEvent);

	void OnUiEvent(UUiEvent_POPUP_SERVER_LIST_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_POPUP_SERVER_LIST_CLOSE_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_POPUP_SERVER_LIST_OK_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_POPUP_USER_NAME_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_POPUP_USER_NAME_OK_CLICKED_FINAL& uiEvent);

public:
	// ========== Viewport 관리 ==========
	void SetActiveViewport(EGamePlayMode mode);
	UViewportWidgetBase* GetActiveViewport() const;
	EGamePlayMode GetActivePlayMode() const;

public:
	void OnDispatchPacket(UPacketPtr* packetPtr) override;
	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
	HandleResult OnPacket(class DC_ACK_USER_DATA& rp);
};

