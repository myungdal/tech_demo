// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "ItemDetailWidgetBase.generated.h"


class UWidgetSwitcher;
class UItemDetailLaunchGameWidgetBase;
class UItemDetailRewardWidgetBase;
class UItemDetailStoneWidgetBase;

/**
 * 아이템 상세 패널 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UItemDetailWidgetBase : public UWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* mItemDetailWidgetSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	UItemDetailLaunchGameWidgetBase* mItemDetailLaunchGame = nullptr;

	UPROPERTY(meta = (BindWidget))
	UItemDetailRewardWidgetBase* mItemDetailReward = nullptr;

	UPROPERTY(meta = (BindWidget))
	UItemDetailStoneWidgetBase* mItemDetailStone = nullptr;

private:
	FDelegateHandle mOnDispatchUiEventHandle;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_ITEM_DETAIL_LAUNCH_GAME_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_ITEM_DETAIL_REWARD_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_ITEM_DETAIL_STONE_OPEN& uiEvent);
};
