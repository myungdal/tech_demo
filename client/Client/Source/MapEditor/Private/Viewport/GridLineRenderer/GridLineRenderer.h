// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class FPaintArgs;
struct FGeometry;
class FSlateRect;
class FSlateWindowElementList;
class FWidgetStyle;
class FGameCameraViewport;

class FGridLineRenderer
{
private:
	GridCol mCol = 0;
	GridRow mRow = 0;

public:
	DISABLE_COPY(FGridLineRenderer);
	explicit FGridLineRenderer(size_t width, size_t height);

public:
	int32 OnPaint(
		const FGameCameraViewport& tileViewport,
		const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
	);

public:
	void UpdateCursorPos(GridCol c, GridRow r);
};
