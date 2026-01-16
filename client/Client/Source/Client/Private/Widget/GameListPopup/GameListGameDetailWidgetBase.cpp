// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "GameListGameDetailWidgetBase.h"


void UGameListGameDetailWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameListGameDetailWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGameListGameDetailWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}
