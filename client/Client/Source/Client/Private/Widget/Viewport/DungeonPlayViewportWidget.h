// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Viewport/ViewportWidgetBase.h"
#include "DungeonPlayViewportWidget.generated.h"


class FUiServiceGame;
class FGameCore;
class GameInstance;
class Engine;
class Entity;
class PacketTemp;

/**
 * 던전 플레이용 뷰포트 위젯 (키보드 조작 기능 포함)
 * SinglePlayViewportWidgetBase, TestPlayViewportWidgetBase의 공통 부모
 * - 하단에 소유 NPC Follower 목록 표시
 */
UCLASS(Abstract, Blueprintable)
class UDungeonPlayViewportWidget : public UViewportWidgetBase
{
	GENERATED_BODY()

protected:
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnKeyDownWithAlt(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;

	// 키 상태 동기화 패킷 전송
	void SendKeyStateSync();

	void OnMouseButtonDownExtra(TSharedPtr<FUiServiceGame> gameService, const Entity* selectedEntity, const Scalar x, const Scalar y) override;

	// 로컬/멀티 플레이에 따라 커맨드를 처리
	void SendCommand(const PacketTemp& packet);

	// 하단 NPC Follower UI 렌더링
	int32 RenderFollowerUI(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled,
		Engine& engine,
		WorldUserId worldUserId
	) const;

	// PC의 NPC Follower 수 계산
	int32 CountNpcFollowers(Engine& engine, WorldUserId worldUserId) const;
};
