// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "GridLineRenderer.h"

#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"

#include "GameCameraController/GameCameraViewport.h"


FGridLineRenderer::FGridLineRenderer(size_t width, size_t height)
{	
}

int32 FGridLineRenderer::OnPaint(
	const FGameCameraViewport& viewport,
	const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled
)
{
	if (0 == viewport.GetMapWidth())
		return LayerId;

	TArray<FVector2D> pointList;
	pointList.SetNum(2);

	const float& ts = viewport.GetTileRenderSize();
	const float& th = viewport.GetTileRenderSizeHalf();
	const size_t& tx0 = viewport.GetTileX0();
	const size_t& tx1 = viewport.GetTileX1();
	const size_t& ty0 = viewport.GetTileY0();
	const size_t& ty1 = viewport.GetTileY1();
	const float& ox = viewport.GetRenderOffset().X + th;
	const float& oy = viewport.GetRenderOffset().Y + th;

	// 세로 라인.
	{
		const float py0 = oy;
		const float py1 = oy + (viewport.GetMapHeight() - 1) * ts;
		for (size_t x = tx0; x <= tx1; ++x)
		{
			const float px = ox + x * ts;
			pointList[0] = { px, py0 };
			pointList[1] = { px, py1 };

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				pointList,
				ESlateDrawEffect::None,
				FLinearColor(1.000000, 1.000000, 1.000000, 1.000000),
				false,
				1.f
			);
		}
	}

	// 가로 라인.
	{
		const float px0 = ox;
		const float px1 = ox + (viewport.GetMapWidth() - 1) * ts;
		for (size_t y = ty0; y <= ty1; ++y)
		{
			const float py = oy + y * ts;
			pointList[0] = { px0, py };
			pointList[1] = { px1, py };

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				pointList,
				ESlateDrawEffect::None,
				FLinearColor(1.000000, 1.000000, 1.000000, 1.000000),
				false,
				1.f
			);
		}
	}

	// 커서.
	{
		const FSlateBrush* brush = FCoreStyle::Get().GetBrush(TEXT("GenericWhiteBox"));

		const FVector2D boxPos = FVector2D(ox + mCol * ts, oy + mRow * ts);
		const FVector2D boxSize = FVector2D(ts, ts);

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(FVector2D(boxSize), FSlateLayoutTransform(boxPos)),
			brush,
			ESlateDrawEffect::None,
			FLinearColor::Red
		);
	}

	++LayerId;

	return LayerId;
}

void FGridLineRenderer::UpdateCursorPos(GridCol c, GridRow r)
{
	mCol = c;
	mRow = r;
}
