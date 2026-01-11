// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "ItemDetailStoneWidgetBase.generated.h"


class UItemWidgetBase;
class UserItemRow;

/**
 * 아이템 상세(스톤) 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UItemDetailStoneWidgetBase : public UWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UItemWidgetBase* mItem0 = nullptr;

	UPROPERTY(meta = (BindWidget))
	UItemWidgetBase* mItem1 = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_ITEM_DETAIL_RELOAD& uiEvent);
}; 
