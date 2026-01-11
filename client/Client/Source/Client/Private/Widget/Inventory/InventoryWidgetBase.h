// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "InventoryWidgetBase.generated.h"


class UButtonWidgetBase;
class UListView;

/**
 * 인벤토리 UI 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UInventoryWidgetBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mAllBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mStoneBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mEtcBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mCloseBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UListView* mInventoryView = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
protected:
	void Reload();	
	void ApplyUserAssetDelta(const class USER_DATA& rp);

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_INVENTORY_ITEM_LIST_RELOAD& uiEvent);
	void OnUiEvent(UUiEvent_INVENTORY_TAB_STATE_UPDATE& uiEvent);
};
