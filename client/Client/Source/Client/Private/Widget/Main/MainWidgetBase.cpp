// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "MainWidgetBase.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

#include "MmoSync/DebugControlTool/DebugInfo.h"

#include "Widget/LoginPopup/LoginPopupWidgetBase.h"
#include "Widget/Debug/DebugWidgetBase.h"
#include "Widget/Cheat/CheatWidgetBase.h"
#include "Widget/Lobby/LobbyWidgetBase.h"
#include "Widget/ServerListPopup/ServerListWidgetBase.h"
#include "Widget/UserNamePopup/UserNamePopupWidgetBase.h"
#include "Widget/Viewport/TestPlayViewportWidgetBase.h"
#include "Widget/Viewport/SinglePlayViewportWidgetBase.h"
#include "Widget/Viewport/MultiPlayViewportWidgetBase.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "SubSystem/NetworkManager/NetworkManager.h"
#include "SubSystem/NetworkManager/PacketPtr/PacketPtr.h"
#include "UiService/Game/UiServiceGame.h"
#include "Util/PacketUtil/PacketUtil.h"


void UMainWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	mMainWidgetSwitcher->SetActiveWidget(mLoginPopup);

	// Viewport는 Switcher로 관리, 초기에는 숨김
	mViewportWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
	mDebug->SetVisibility(ESlateVisibility::Hidden);
	mLobby->SetVisibility(ESlateVisibility::Hidden);

	if (DebugInfo::DEBUG_GAME_DIRECTLY)
	{
		UUiServiceManager::Get(this)->SetClientMode(ClientMode::TEST);
		UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_TEST_GAME_ENTER_CLICKED>();
	}
}

void UMainWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMainWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_LOADING_START& uiEvent)
{
	mLoading->SetVisibility(ESlateVisibility::Visible);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_LOADING_END& uiEvent)
{
	mLoading->SetVisibility(ESlateVisibility::Hidden);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_TEST_GAME_ENTER_CLICKED& uiEvent)
{
	mLoading->SetVisibility(ESlateVisibility::Hidden);

	mLobby->SetVisibility(ESlateVisibility::Hidden);

	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);

	SetActiveViewport(EGamePlayMode::TestPlay);

	UUiServiceManager::Get(this)->SetClientMode(ClientMode::TEST);

	// ULoginWidgetBase 의 mTestGameFastCheck 체크에 따라, 아래 두 개중에 분기
	if (true == mLoginPopup->IsTestGameFastChecked())
	{
		UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_TEST_GAME_START_FAST>();
	}
	else
	{
		UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_TEST_GAME_START>();
	}
}

void UMainWidgetBase::OnUiEvent(UUiEvent_TEST_GAME_LEAVE_CLICKED& uiEvent)
{
	mLoading->SetVisibility(ESlateVisibility::Hidden);

	mLobby->SetVisibility(ESlateVisibility::Visible);

	mMainWidgetSwitcher->SetActiveWidget(mLoginPopup);
	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);

	SetActiveViewport(EGamePlayMode::None);

	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_TEST_GAME_FINISH>();
}

void UMainWidgetBase::OnUiEvent(UUiEvent_GAME_ENTER& uiEvent)
{
	mLoading->SetVisibility(ESlateVisibility::Hidden);

	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);

	SetActiveViewport(EGamePlayMode::MultiPlay);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_GAME_LEAVE& uiEvent)
{
	SetActiveViewport(EGamePlayMode::None);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_DEBUG_TOGGLE_CLICKED& uiEvent)
{
	ESlateVisibility visibility = (ESlateVisibility::Hidden ==
		mDebug->GetVisibility()) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	mDebug->SetVisibility(visibility);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_CHEAT_TOGGLE_CLICKED& uiEvent)
{
	ESlateVisibility visibility =
		(ESlateVisibility::Hidden == mCheat->GetVisibility()) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	mCheat->SetVisibility(visibility);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_CONNECTED& uiEvent)
{
	if (UUiServiceManager::Get(this)->GetClientMode() != ClientMode::GAME)
		return;

	mLobby->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_DISCONNECTED& uiEvent)
{
	if (UUiServiceManager::Get(this)->GetClientMode() != ClientMode::GAME)
		return;

	mMainWidgetSwitcher->SetActiveWidget(mLoginPopup);
	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);

	SetActiveViewport(EGamePlayMode::None);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_FATAL_ERROR& uiEvent)
{
	UNetworkManager::Get(this)->Disconnect();

	mMainWidgetSwitcher->SetActiveWidget(mLoginPopup);
	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);

	SetActiveViewport(EGamePlayMode::None);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_MENU_EXIT_CLICKED& uiEvent)
{
	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_FATAL_ERROR>();
}

void UMainWidgetBase::OnUiEvent(UUiEvent_POPUP_SERVER_LIST_OPEN& uiEvent)
{
	mMainWidgetSwitcher->SetActiveWidget(mServerList);
	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_POPUP_SERVER_LIST_CLOSE_CLICKED& uiEvent)
{
	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_POPUP_SERVER_LIST_OK_CLICKED& uiEvent)
{
	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_POPUP_USER_NAME_OPEN& uiEvent)
{
	mMainWidgetSwitcher->SetActiveWidget(mUserNamePopup);
	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
}

void UMainWidgetBase::OnUiEvent(UUiEvent_POPUP_USER_NAME_OK_CLICKED_FINAL& uiEvent)
{
	mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void UMainWidgetBase::OnDispatchPacket(UPacketPtr* packetPtr)
{
	MAYBE_UNUSED const HandleResult handleResult = PacketUtil::DispatchPacket(this, *packetPtr->rp);
}

HandleResult UMainWidgetBase::OnPacket(DC_ACK_USER_DATA& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();
	if (Result::SUCCEEDED == result)
	{
		if ((ESlateVisibility::Hidden != mMainWidgetSwitcher->GetVisibility()) &&
			(mMainWidgetSwitcher->GetActiveWidget() == mLoginPopup))
		{
			mMainWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden); 
		}
	}

	return HandleResult::OK;
}

// ========================================================================
// Viewport 관리
// ========================================================================
void UMainWidgetBase::SetActiveViewport(EGamePlayMode mode)
{
	switch (mode)
	{
	case EGamePlayMode::TestPlay:
		mViewportWidgetSwitcher->SetActiveWidget(mTestPlayViewport);
		mViewportWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		break;

	case EGamePlayMode::SinglePlay:
		mViewportWidgetSwitcher->SetActiveWidget(mSinglePlayViewport);
		mViewportWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		break;

	case EGamePlayMode::MultiPlay:
		mViewportWidgetSwitcher->SetActiveWidget(mMultiPlayViewport);
		mViewportWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		break;

	case EGamePlayMode::None:
	default:
		mViewportWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
		break;
	}

	// FUiServiceGame에도 알림
	if (TSharedPtr<FUiServiceGame> gameService = UUiServiceManager::Get(this)->GetService<FUiServiceGame>())
	{
		gameService->SetPlayMode(mode);
	}
}

UViewportWidgetBase* UMainWidgetBase::GetActiveViewport() const
{
	if (mViewportWidgetSwitcher->GetVisibility() == ESlateVisibility::Hidden)
		return nullptr;

	return Cast<UViewportWidgetBase>(mViewportWidgetSwitcher->GetActiveWidget());
}

EGamePlayMode UMainWidgetBase::GetActivePlayMode() const
{
	if (mViewportWidgetSwitcher->GetVisibility() == ESlateVisibility::Hidden)
		return EGamePlayMode::None;

	UWidget* activeWidget = mViewportWidgetSwitcher->GetActiveWidget();
	
	if (activeWidget == mTestPlayViewport)
		return EGamePlayMode::TestPlay;
	if (activeWidget == mSinglePlayViewport)
		return EGamePlayMode::SinglePlay;
	if (activeWidget == mMultiPlayViewport)
		return EGamePlayMode::MultiPlay;

	return EGamePlayMode::None;
}
