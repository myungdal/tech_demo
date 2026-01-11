// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ToolbarWidget.h"

#include "Serialization/JsonWriter.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"

#include "SlateOptMacros.h"

#include "Styling/SlateTypes.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"

#include "ClientUtil/ClientFileUtil.h"


#define LOCTEXT_NAMESPACE "MapEditor"

SToolbarWidget::~SToolbarWidget()
{
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SToolbarWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SCheckBox)
			//.IsChecked(this, &SToolbarWidget::IsChecked)
			//.OnCheckStateChanged(this, &SToolbarWidget::OnCheckStateChanged)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("aaa")))
			]
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SCheckBox)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("aaa")))
			]
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			// "ReloadDebugInfoFile" 버튼 추가
			SNew(SButton)
			.Text(FText::FromString(TEXT("ReloadDebugInfoFile")))
			.OnClicked(this, &SToolbarWidget::OnReloadDebugInfoFile)
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SToolbarWidget::OnReloadDebugInfoFile()
{
	FClientFileUtil::ReloadDebugInfoFile();

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
