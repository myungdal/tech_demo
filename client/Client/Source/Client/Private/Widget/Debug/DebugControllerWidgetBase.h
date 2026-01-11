// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "DebugControllerWidgetBase.generated.h"


class UButtonWidgetBase;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class UDebugControllerWidgetBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mReqGameListBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mDebugBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mCheatBtn = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void OnDispatchPacket(UPacketPtr* packetPtr) override;
	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
}; 
