// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "SinglePlayViewportWidgetBase.h"


void USinglePlayViewportWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void USinglePlayViewportWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void USinglePlayViewportWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}

int32 USinglePlayViewportWidgetBase::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return LayerId;
}

void USinglePlayViewportWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}
