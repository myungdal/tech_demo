// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "Widget/Base/WidgetBase.h"
#include "SkillControllerWidgetBase.generated.h"


class UButtonWidgetBase;

/**
 * 스킬/액션 단축키 컨트롤 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class USkillControllerWidgetBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mRollBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mChargeBtn = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_BTN_ROLL_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_BTN_CHARGE_CLICKED& uiEvent);

public:
	void OnDispatchPacket(UPacketPtr* packetPtr) override;
	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
};
