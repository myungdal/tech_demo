// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "LoginPopupWidgetBase.generated.h"


class UButtonWidgetBase;
class UEditableTextBox;
class UCheckBox;

/**
 * 로그인 팝업 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class ULoginPopupWidgetBase : public UWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* mAccountId = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* mDeviceToken = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* mGoogleToken = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* mAppleToken = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mLoginBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mTestGameBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* mTestGameFastCheck = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_LOAD_ACCOUNT& uiEvent);
	void OnUiEvent(UUiEvent_POPUP_LOGIN_OK_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_DISCONNECTED& uiEvent);

public:
		void OnDispatchPacket(UPacketPtr* packetPtr) override;
		HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
	bool IsTestGameFastChecked() const;
};

