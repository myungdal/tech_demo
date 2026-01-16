// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Viewport/DungeonPlayViewportWidget.h"
#include "TestPlayViewportWidgetBase.generated.h"


class TimeStepGenerator;

/**
 * 테스트 플레이용 뷰포트 위젯
 */
UCLASS(Abstract, Blueprintable)
class UTestPlayViewportWidgetBase : public UDungeonPlayViewportWidget
{
	GENERATED_BODY()

private:
	TSharedPtr<TimeStepGenerator> mTimeStepGenerator = nullptr;
	TimeStep mTimeStepNow = 0;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	WorldUserId GetMyWorldUserId() const override;

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_TEST_GAME_START& uiEvent);
	void OnUiEvent(UUiEvent_TEST_GAME_START_FAST& uiEvent);
	void OnUiEvent(UUiEvent_TEST_GAME_STATIC_DATA_COMPLETED& uiEvent);

//public:
//	void OnDispatchPacket(UPacketPtr* packetPtr) override;
//	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
};
