// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "GameCameraViewport.h"

#include "Layout/SlateRect.h"

#include "GameRenderer/Tile/TileDataType.h"


void FGameCameraViewport::Init(const size_t* mapWidth, const size_t* mapHeight, const FVector2D* target, const float* zoom) noexcept
{
	mMapWidth = mapWidth;
	mMapHeight = mapHeight;
	mCamTarget = target;
	mCamZoom = zoom;
	mMapRenderSize = FVector2D(GetMapWidth() * mTileRenderSize, GetMapHeight() * mTileRenderSize);
}

void FGameCameraViewport::UpdateForTile(const FVector2D& viewportRect) noexcept
{
	if (0 == GetMapWidth())
		return;

	PreUpdate(viewportRect);

	const int64 tileX0 = static_cast<int64>(mViewportMin.X / mTileRenderSize);
	const int64 tileX1 = static_cast<int64>(mViewportMax.X / mTileRenderSize);
	const int64 tileY0 = static_cast<int64>(mViewportMin.Y / mTileRenderSize);
	const int64 tileY1 = static_cast<int64>(mViewportMax.Y / mTileRenderSize);
	SetTileRegion(tileX0, tileX1, tileY0, tileY1);
}

void FGameCameraViewport::UpdateForGrid(const FVector2D& viewportRect) noexcept
{
	if (0 == GetMapWidth())
		return;

	PreUpdate(viewportRect);

	const int64 tileX0 = static_cast<int64>((mViewportMin.X - mTileRenderSizeHalf) / mTileRenderSize);
	const int64 tileX1 = static_cast<int64>((mViewportMax.X - mTileRenderSizeHalf) / mTileRenderSize) + 1;
	const int64 tileY0 = static_cast<int64>((mViewportMin.Y - mTileRenderSizeHalf) / mTileRenderSize);
	const int64 tileY1 = static_cast<int64>((mViewportMax.Y - mTileRenderSizeHalf) / mTileRenderSize) + 1;
	SetTileRegion(tileX0, tileX1, tileY0, tileY1);
}

void FGameCameraViewport::PreUpdate(const FVector2D& viewportRect) noexcept
{
	mTileRenderSize = TILE_RENDER_SIZE * (*mCamZoom);
	mTileRenderSizeHalf = (mTileRenderSize * 0.5f);
	mMapRenderSize = FVector2D(GetMapWidth() * mTileRenderSize, GetMapHeight() * mTileRenderSize);

	mViewportSize = viewportRect * (1.f - (2.f * MARGIN));
	mViewportMin = (*mCamTarget) * (*mCamZoom) - mViewportSize * 0.5f;
	mViewportMax = (*mCamTarget) * (*mCamZoom) + mViewportSize * 0.5f;

	mRenderOffset = -mViewportMin + viewportRect * MARGIN;
}

void FGameCameraViewport::SetTileRegion(int64_t tileX0, int64_t tileX1, int64_t tileY0, int64_t tileY1) noexcept
{
	if (0 > tileX0) { mTileX0 = 0; }
	else if (static_cast<int64>(GetMapWidth() - 1) < tileX0) { mTileX0 = (GetMapWidth() - 1); }
	else { mTileX0 = static_cast<size_t>(tileX0); }

	if (0 > tileX1) { mTileX1 = 0; }
	else if (static_cast<int64>(GetMapWidth() - 1) < tileX1) { mTileX1 = (GetMapWidth() - 1); }
	else { mTileX1 = static_cast<size_t>(tileX1); }

	if (0 > tileY0) { mTileY0 = 0; }
	else if (static_cast<int64>(GetMapHeight() - 1) < tileY0) { mTileY0 = (GetMapHeight() - 1); }
	else { mTileY0 = static_cast<size_t>(tileY0); }

	if (0 > tileY1) { mTileY1 = 0; }
	else if (static_cast<int64>(GetMapHeight() - 1) < tileY1) { mTileY1 = (GetMapHeight() - 1); }
	else { mTileY1 = static_cast<size_t>(tileY1); }
}
