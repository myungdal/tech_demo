// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "EntityListWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SScrollBox.h"


#define LOCTEXT_NAMESPACE "MapEditor"

SEntityListWidget::~SEntityListWidget()
{
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEntityListWidget::Construct(const FArguments& InArgs)
{
	mDataList = InArgs._dataList;
	mFilename = TEXT("");

	ChildSlot
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SAssignNew(mListView, ListView)
				//.ItemHeight(24)
				.ListItemsSource(&mItemList)
				.OnGenerateRow(this, &SEntityListWidget::OnGenerateRow)
				.OnMouseButtonClick(InArgs._OnMouseButtonClick)
				.OnMouseButtonDoubleClick(InArgs._OnMouseButtonDoubleClick)
			]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SEntityListWidget::Load(SMapListWidget::ItemPtr item)
{
	mFilename = TEXT("mapEditorEntityList_");
	mFilename += item->mData->mName;
	mFilename += TEXT(".json");
	Parent::Load();
}


TSharedRef<ITableRow> SEntityListWidget::OnGenerateRow(SEntityListWidget::ItemPtr item, const TSharedRef<STableViewBase>& ownerTable)
{
	return
		SNew(STableRow<ItemPtr>, ownerTable)
		.Padding(6.0f)
		.Content()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()			
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					SNew(STextBlock)
					.Text(FText::FromString(item->mData->mName))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(EHorizontalAlignment::HAlign_Right)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Delete")))
					.OnClicked(this, &SEntityListWidget::DeleteItem, item)
				]
			]
		];
}

#undef LOCTEXT_NAMESPACE
