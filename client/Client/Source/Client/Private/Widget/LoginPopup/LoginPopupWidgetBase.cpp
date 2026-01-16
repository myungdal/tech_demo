// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "LoginPopupWidgetBase.h"

#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"

#include "Widget/Button/ButtonWidgetBase.h"

#include "ClientUtil/ClientStringUtil.h"
#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "SubSystem/NetworkManager/NetworkManager.h"
#include "SubSystem/NetworkContext/NetworkContext.h"
#include "SubSystem/NetworkManager/PacketPtr/PacketPtr.h"
#include "SubSystem/NetworkManager/SendBuffer/TempBuffer.h"
#include "Util/PacketUtil/PacketUtil.h"
#include "Util/SocketUtil/SocketUtil.h"


void ULoginPopupWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();	

	mLoginBtn->SetupButton(UUiEvent_POPUP_LOGIN_OK_CLICKED::TYPE, TEXT("Login"));
	mTestGameBtn->SetupButton(UUiEvent_TEST_GAME_ENTER_CLICKED::TYPE, TEXT("Test"));

	mLoginBtn->SetIsEnabled(true);
	mTestGameBtn->SetIsEnabled(true);
 
	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_LOAD_ACCOUNT>();
}

void ULoginPopupWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void ULoginPopupWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void ULoginPopupWidgetBase::OnUiEvent(UUiEvent_LOAD_ACCOUNT& uiEvent)
{
	if (false == UNetworkContext::Get(this)->LoadAccountJsonFile())
		return;

	const PacketKeep<ACCOUNT>& account = UNetworkContext::Get(this)->Account();
	mAccountId->SetText(FText::FromString(FString::Printf(TEXT("%llu"), account->Get_c_account_id().GetData()))); // accountId 로 이름 변경 필요
	mDeviceToken->SetText(ClientStringUtil::w_to_t(account->Get_c_device_token()));
	mGoogleToken->SetText(ClientStringUtil::w_to_t(account->Get_c_google_token()));
	mAppleToken->SetText(ClientStringUtil::w_to_t(account->Get_c_apple_token()));
}

void ULoginPopupWidgetBase::OnUiEvent(UUiEvent_POPUP_LOGIN_OK_CLICKED& uiEvent)
{
	mLoginBtn->SetIsEnabled(false);

	AccountId accountId = FCString::Atoi64(*mAccountId->GetText().ToString()); // accountId 로 이름 변경하기
	const FString& deviceToken = mDeviceToken->GetText().ToString();
	const FString& googleToken = mGoogleToken->GetText().ToString();
	const FString& appleToken = mAppleToken->GetText().ToString();

	FTempBuffer buffer;
	ACCOUNT::Writer wp(PARAM, *buffer);
	wp.SetValues(
		accountId,
		*ClientStringUtil::s_to_w(deviceToken), 
		*ClientStringUtil::s_to_w(googleToken), 
		*ClientStringUtil::s_to_w(appleToken),
		INVALID_DB_SHARD_IDX,
		AccountState::NONE,
		time_t{ 0 },
		time_t{ 0 },
		time_t{ 0 }
	);

	UNetworkContext::Get(this)->Account() = wp.GetPacketBufPtr();
	UNetworkContext::Get(this)->SaveAccount();
	UNetworkManager::Get(this)->Connect();
}

void ULoginPopupWidgetBase::OnUiEvent(UUiEvent_DISCONNECTED& uiEvent)
{
	mLoginBtn->SetIsEnabled(true);
}

void ULoginPopupWidgetBase::OnDispatchPacket(UPacketPtr* packetPtr)
{
	MAYBE_UNUSED const HandleResult handleResult = PacketUtil::DispatchPacket(this, *packetPtr->rp);
}

bool ULoginPopupWidgetBase::IsTestGameFastChecked() const
{
	if (nullptr == mTestGameFastCheck)
		return false;

	return mTestGameFastCheck->IsChecked();
}

