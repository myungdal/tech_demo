// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#include "UiButton.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"


void UUiButton::SetupButton(UiEventType uiEventTypeClicked)
{
	mUiEventClicked = uiEventTypeClicked;
}

TSharedRef<SWidget> UUiButton::RebuildWidget()
{
	TSharedRef<SWidget> ret = Super::RebuildWidget();

	if (false == OnClicked.IsAlreadyBound(this, &UUiButton::OnButtonClicked))
	{
		OnClicked.AddDynamic(this, &UUiButton::OnButtonClicked);
	}

	return ret;
}

void UUiButton::OnButtonClicked()
{
	UUiServiceManager::Get(this)->BroadcastUiEvent(mUiEventClicked);
}
