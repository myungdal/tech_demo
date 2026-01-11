// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "ServerListWidgetBase.generated.h"


class UButtonWidgetBase;
class UListView;

/**
 * 서버 목록 팝업 위젯
 * 
 * 사용자가 게임 서버를 선택할 수 있는 팝업입니다.
 */
UCLASS(Abstract, Blueprintable)
class UServerListWidgetBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UListView* mServerListView = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mCloseBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mOkBtn = nullptr;

private:
	bool mUserCreateMode = true;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_POPUP_SERVER_LIST_OPEN& uiEvent);
};
