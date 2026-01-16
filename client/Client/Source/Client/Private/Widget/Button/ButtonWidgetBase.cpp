// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "Widget/Button/ButtonWidgetBase.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "UiObject/Button/UiButton.h"


void UButtonWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UButtonWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UButtonWidgetBase::SetupButton(UiEventType uiEventTypeClicked, const TCHAR* name)
{
	mButton->SetupButton(uiEventTypeClicked);
	mName->SetText(FText::FromString(FString(name)));
	SetNameVisible(true);
	SetRedDotVisible(false);
}

void UButtonWidgetBase::SetNameVisible(bool visible)
{
	mName->SetVisibility(visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UButtonWidgetBase::SetRedDotVisible(bool visible)
{
	mRedDot->SetVisibility(visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
