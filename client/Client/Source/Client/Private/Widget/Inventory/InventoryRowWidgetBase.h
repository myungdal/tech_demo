// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InventoryRowWidgetBase.generated.h"


class UItemWidgetBase;
class UserItemRow;

/**
 * 인벤토리 목록 행 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UInventoryRowWidgetBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UItemWidgetBase* mItem0 = nullptr;

	UPROPERTY(meta = (BindWidget))
	UItemWidgetBase* mItem1 = nullptr;

	UPROPERTY(meta = (BindWidget))
	UItemWidgetBase* mItem2 = nullptr;

	UPROPERTY(meta = (BindWidget))
	UItemWidgetBase* mItem3 = nullptr;

protected:
	void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	void SetItemIconImg(UItemWidgetBase* target, const UserItemRow* userItemRow, bool redDot);
}; 
