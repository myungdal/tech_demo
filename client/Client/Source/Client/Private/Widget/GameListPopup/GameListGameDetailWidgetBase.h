// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "GameListGameDetailWidgetBase.generated.h"


/**
 * 게임 상세 정보 UI 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UGameListGameDetailWidgetBase : public UWidgetBase
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
};
