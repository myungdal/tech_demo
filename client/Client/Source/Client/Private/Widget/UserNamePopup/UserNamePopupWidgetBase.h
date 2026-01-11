// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "UserNamePopupWidgetBase.generated.h"


class UButtonWidgetBase;
class UEditableTextBox;

/**
 * 유저 이름 입력 팝업 위젯
 * 
 * 캐릭터 생성 시 사용자 이름을 입력받는 팝업입니다.
 */
UCLASS(Abstract, Blueprintable)
class UUserNamePopupWidgetBase : public UWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* mUserName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mOkBtn = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_POPUP_USER_NAME_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_POPUP_USER_NAME_OK_CLICKED& uiEvent);
};

