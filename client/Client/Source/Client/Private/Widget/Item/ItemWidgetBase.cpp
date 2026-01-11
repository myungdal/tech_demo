// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "ItemWidgetBase.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "Paper2D/Classes/PaperSprite.h"

#include "Packet/StaticData/Item/ItemDoc.h"
#include "Packet/StaticData/Item/ItemDocLookup.h"
#include "Packet/UserCache/UserItem/UserItemRow.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "UiService/Inventory/UiServiceInventory.h"


void UItemWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == mBorder->OnMouseButtonUpEvent.IsBoundToObject(this))
	{
		mBorder->OnMouseButtonUpEvent.BindUFunction(this, FName(TEXT("OnItemClicked")));
	}
}

void UItemWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UItemWidgetBase::SetupItem(const UserItemRow* userItemRow, ItemWidgetContainerType containerType, bool redDot)
{
	mItemIcon->SetBrushFromAtlasInterface(nullptr);
	mItemQuantity->SetVisibility(ESlateVisibility::Hidden);
	mRedDot->SetVisibility(ESlateVisibility::Hidden);
	mStonePattern0->SetVisibility(ESlateVisibility::Hidden);
	mStonePattern1->SetVisibility(ESlateVisibility::Hidden);
	mStonePattern2->SetVisibility(ESlateVisibility::Hidden);
	mStonePattern3->SetVisibility(ESlateVisibility::Hidden);
	mStonePattern4->SetVisibility(ESlateVisibility::Hidden);
	mStonePattern5->SetVisibility(ESlateVisibility::Hidden);
	mStonePattern6->SetVisibility(ESlateVisibility::Hidden);
	mStonePattern7->SetVisibility(ESlateVisibility::Hidden);

	mUserItemRow = userItemRow;
	mItemWidgetContainerType = containerType;

	SetupItemIcon();
	SetupItemQuantity();
	SetupItemRedDot(redDot);

	ItemType itemType = mUserItemRow->GetDoc()->mStaticItem->Get_c_item_type();

	switch (itemType)
	{
	case ItemType::STONE:
	{
		SetupItemStonePattern();
		break;
	}
	default:
	{
		break;
	}
	}
}

void UItemWidgetBase::SetupItemIcon()
{
	uint16_t itemIconImg = mUserItemRow->GetDoc()->mStaticItem->Get_c_uasset_icon_img();

	const FString& key = 
		FString::Printf(
			TEXT("/Game/Sprite/Ui/ItemIcon/ItemIcon_%d.ItemIcon_%d"),
			itemIconImg, itemIconImg
		);
	FSoftObjectPath assetRef(key);
	FStreamableManager& streamableManager = UAssetManager::GetStreamableManager();
	UPaperSprite* paperSprite = Cast<UPaperSprite>(streamableManager.LoadSynchronous(assetRef));
	mItemIcon->SetBrushFromAtlasInterface(paperSprite);
}

void UItemWidgetBase::SetupItemQuantity()
{
	if (1 < mUserItemRow->GetDoc()->mStaticItem->Get_c_max_quantity())
	{
		mItemQuantity->SetText(
			FText::FromString(FString::FromInt(mUserItemRow->Data().Get_c_item_quantity()))
		);
	}
	else
	{
		mItemQuantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UItemWidgetBase::SetupItemRedDot(bool redDot)
{
	mRedDot->SetVisibility((true == redDot) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UItemWidgetBase::SetupItemStonePattern()
{
	const ItemColorUnion& itemColorUnion = mUserItemRow->Data().Get_c_item_color_union();

	UImage* stonePattern[8] = {
		mStonePattern0,
		mStonePattern1,
		mStonePattern2,
		mStonePattern3,
		mStonePattern4,
		mStonePattern5,
		mStonePattern6,
		mStonePattern7
	};

	for (int i = 0; i < 8; ++i)
	{
		int x = std::abs(((i + 5) % 8) - 3) - 2;
		int y = std::abs(((i + 3) % 8) - 3) - 2;
		stonePattern[i]->SetRenderTranslation(FVector2D(x * 25, y * 25));

		const FString& key =
			FString::Printf(
				TEXT("/Game/Sprite/Ui/ItemStonePattern/ItemStonePattern_%d.ItemStonePattern_%d"),
				itemColorUnion.itemColor[i], itemColorUnion.itemColor[i]
			);
		FSoftObjectPath assetRef(key);
		FStreamableManager& streamableManager = UAssetManager::GetStreamableManager();
		UPaperSprite* paperSprite = Cast<UPaperSprite>(streamableManager.LoadSynchronous(assetRef));
		stonePattern[i]->SetBrushFromAtlasInterface(paperSprite);
		stonePattern[i]->SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemWidgetBase::OnItemClicked()
{
	if (ItemWidgetContainerType::INVENTORY == mItemWidgetContainerType)
	{
		UUiServiceManager::Get(this)->GetService<FUiServiceInventory>()->SelectItem(mUserItemRow);
	}
	else if (ItemWidgetContainerType::ITEM_DETAIL_STONE == mItemWidgetContainerType)
	{
		UUiServiceManager::Get(this)->GetService<FUiServiceInventory>()->DeselectItem(mUserItemRow);
	}
}
