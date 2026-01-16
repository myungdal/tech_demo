// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "EntityTemplateListWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SScrollBox.h"


#define LOCTEXT_NAMESPACE "MapEditor"

SEntityTemplateListWidget::~SEntityTemplateListWidget()
{
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEntityTemplateListWidget::Construct(const FArguments& InArgs)
{
	mDataList = InArgs._dataList;
	mFilename = TEXT("mapEditorEntityTemplateList.json");

	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Add New EntityTemplate")))
			.OnClicked(this, &SEntityTemplateListWidget::OnAddNewClicked)
		]
		+ SScrollBox::Slot()
		[
			SAssignNew(mListView, ListView)
			//.ItemHeight(24)
			.ListItemsSource(&mItemList)
			.OnGenerateRow(this, &SEntityTemplateListWidget::OnGenerateRow)
			.OnMouseButtonClick(InArgs._OnMouseButtonClick)
			.OnMouseButtonDoubleClick(InArgs._OnMouseButtonDoubleClick)
		]
	];

	Load();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<ITableRow> SEntityTemplateListWidget::OnGenerateRow(SEntityTemplateListWidget::ItemPtr item, const TSharedRef<STableViewBase>& ownerTable)
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
					.OnClicked(this, &SEntityTemplateListWidget::DeleteItem, item)
				]
			]
		];
}

#undef LOCTEXT_NAMESPACE
