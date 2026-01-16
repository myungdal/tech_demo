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
#include "ItemWidgetBase.generated.h"


class UBorder;
class UImage;
class UTextBlock;
class UserItemRow;

UENUM()
enum class ItemWidgetContainerType : uint8
{
	NONE = 0,
	INVENTORY,
	ITEM_DETAIL_LAUNCH_GAME,
	ITEM_DETAIL_REWARD,
	ITEM_DETAIL_STONE,
};

/**
 * 아이템 슬롯 UI 위젯.
 */
UCLASS(Abstract, Blueprintable)
class UItemWidgetBase : public UUserWidget
{
	GENERATED_BODY()

private:
	const UserItemRow* mUserItemRow = nullptr;
	ItemWidgetContainerType mItemWidgetContainerType = ItemWidgetContainerType::NONE;

protected:
	UPROPERTY(meta = (BindWidget))
	UBorder* mBorder = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* mItemIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mItemQuantity = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* mRedDot = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* mStonePattern0 = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UImage* mStonePattern1 = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UImage* mStonePattern2 = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UImage* mStonePattern3 = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UImage* mStonePattern4 = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UImage* mStonePattern5 = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UImage* mStonePattern6 = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UImage* mStonePattern7 = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void SetupItem(const UserItemRow* userItemRow, ItemWidgetContainerType containerType, bool redDot);

private:
	void SetupItemIcon();
	void SetupItemQuantity();
	void SetupItemRedDot(bool redDot);
	void SetupItemStonePattern();

protected:
	UFUNCTION()
	void OnItemClicked();
};
