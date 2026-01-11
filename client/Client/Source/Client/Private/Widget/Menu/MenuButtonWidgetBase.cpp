// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "MenuButtonWidgetBase.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "UiObject/Button/UiButton.h"


void UMenuButtonWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMenuButtonWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMenuButtonWidgetBase::SetupMenuButton(UiEventType uiEventTypeClicked, const TCHAR* name)
{
	mButton->SetupButton(uiEventTypeClicked);
	mName->SetText(FText::FromString(FString(name)));
	SetNameVisible(true);
	SetRedDotVisible(false);
}

void UMenuButtonWidgetBase::SetNameVisible(bool visible)
{
	mName->SetVisibility(visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMenuButtonWidgetBase::SetRedDotVisible(bool visible)
{
	mRedDot->SetVisibility(visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
} 
