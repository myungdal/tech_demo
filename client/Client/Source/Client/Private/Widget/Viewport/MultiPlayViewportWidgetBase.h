// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Viewport/ViewportWidgetBase.h"
#include "MultiPlayViewportWidgetBase.generated.h"


/**
 * 멀티플레이 뷰포트 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UMultiPlayViewportWidgetBase : public UViewportWidgetBase
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	WorldUserId GetMyWorldUserId() const override;

	// 마우스 클릭 시 서버로 커맨드 전송
	void OnMouseButtonDownExtra(TSharedPtr<FUiServiceGame> gameService, const Entity* selectedEntity, const Scalar x, const Scalar y) override;

	// 키보드 입력 처리
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;

private:
	void SendCommand(const PacketTemp& packet);
	void SendKeyStateSync();
	FReply NativeOnKeyDownWithShift(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}

public:
	void OnDispatchPacket(UPacketPtr* packetPtr) override;
	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
};
