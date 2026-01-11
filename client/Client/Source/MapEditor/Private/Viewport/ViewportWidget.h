// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "MapEditor/Private/Map/MapListWidget.h"


class FMapRenderer;
class FGridLineRenderer;

class SViewportWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SViewportWidget)
	{}
	SLATE_END_ARGS()

private:
	std::shared_ptr<FMapRenderer> mMapRenderer = nullptr;
	std::shared_ptr<FGridLineRenderer> mGridLineRenderer = nullptr;

public:
	void Construct(const FArguments& InArgs);

public:
	void Load(SMapListWidget::ItemPtr item);

protected:
	//void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	bool SupportsKeyboardFocus() const override;
	FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	FVector2D GetCursorPos(const FPointerEvent& pointerEvent);
	void UpdateGridCursorPos(const FVector2D& cursorPos);
};
