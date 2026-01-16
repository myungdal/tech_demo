// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "CheatWidgetBase.h"

#include "Components/ListView.h"

#include "Widget/Button/ButtonWidgetBase.h"


void UCheatWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	mCloseBtn->SetupButton(UUiEvent_CHEAT_TOGGLE_CLICKED::TYPE, TEXT("X"));
}

void UCheatWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

TSharedRef<SWidget> UCheatWidgetBase::RebuildWidget()
{
	TSharedRef<SWidget> ret = Super::RebuildWidget();

	//mCheatListView->ClearListItems();

	//UCheatItem* item = NewObject<UCheatItem>();
	//item->mCheat = TEXT("createItem 1 1");
	//mCheatListView->AddItem(item);

	return ret;
}
