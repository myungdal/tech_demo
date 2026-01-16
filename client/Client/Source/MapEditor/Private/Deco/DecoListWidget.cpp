// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DecoListWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SScrollBox.h"


#define LOCTEXT_NAMESPACE "MapEditor"

SDecoListWidget::~SDecoListWidget()
{
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SDecoListWidget::Construct(const FArguments& InArgs)
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
				.OnGenerateRow(this, &SDecoListWidget::OnGenerateRow)
				.OnMouseButtonClick(InArgs._OnMouseButtonClick)
				.OnMouseButtonDoubleClick(InArgs._OnMouseButtonDoubleClick)
			]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDecoListWidget::Load(SMapListWidget::ItemPtr item)
{
	mFilename = TEXT("mapEditorDecoList_");
	mFilename += item->mData->mName;
	mFilename += TEXT(".json");
	Parent::Load();
}


TSharedRef<ITableRow> SDecoListWidget::OnGenerateRow(SDecoListWidget::ItemPtr item, const TSharedRef<STableViewBase>& ownerTable)
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
					.OnClicked(this, &SDecoListWidget::DeleteItem, item)
				]
			]
		];
}

#undef LOCTEXT_NAMESPACE
