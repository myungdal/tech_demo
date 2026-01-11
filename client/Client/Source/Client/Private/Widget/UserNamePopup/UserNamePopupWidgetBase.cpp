// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "UserNamePopupWidgetBase.h"

#include "Components/EditableTextBox.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "SubSystem/NetworkContext/NetworkContext.h"
#include "Util/SocketUtil/SocketUtil.h"
#include "ClientUtil/ClientStringUtil.h"
#include "Widget/Button/ButtonWidgetBase.h"


void UUserNamePopupWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();	

	mOkBtn->SetupButton(UUiEvent_POPUP_USER_NAME_OK_CLICKED::TYPE, TEXT("OK"));
	mOkBtn->SetIsEnabled(true);
}

void UUserNamePopupWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUserNamePopupWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UUserNamePopupWidgetBase::OnUiEvent(UUiEvent_POPUP_USER_NAME_OPEN& uiEvent)
{
	mOkBtn->SetIsEnabled(true);
}

void UUserNamePopupWidgetBase::OnUiEvent(UUiEvent_POPUP_USER_NAME_OK_CLICKED& uiEvent)
{
	mOkBtn->SetIsEnabled(false);

	// 입력된 유저 이름을 NetworkContext에 저장
	const FString& userName = mUserName->GetText().ToString();
	UNetworkContext::Get(this)->SetNewUserName(*ClientStringUtil::s_to_w(userName));

	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_POPUP_USER_NAME_OK_CLICKED_FINAL>();
}

