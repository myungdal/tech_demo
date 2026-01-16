// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class FGameCameraViewport
{
public:
	static constexpr float MARGIN = 0.0f;

private:
	const size_t* mMapWidth = nullptr;
	const size_t* mMapHeight = nullptr;
	const FVector2D* mCamTarget = nullptr;
	const float* mCamZoom = nullptr;
	float mTileRenderSize = 64.f;
	float mTileRenderSizeHalf = 32.f;
	FVector2D mMapRenderSize{};

	FVector2D mRenderOffset{};
	FVector2D mViewportSize{};
	FVector2D mViewportMin{};
	FVector2D mViewportMax{};
	size_t mTileX0 = 0;
	size_t mTileX1 = 0;
	size_t mTileY0 = 0;
	size_t mTileY1 = 0;	

public:
	void Init(const size_t* mapWidth, const size_t* mapHeight, const FVector2D* target, const float* zoom) noexcept;
	void UpdateForTile(const FVector2D& viewportRectSize) noexcept;
	void UpdateForGrid(const FVector2D& viewportRectSize) noexcept;

private:
	void PreUpdate(const FVector2D& viewportRectSize) noexcept;
	void SetTileRegion(int64_t tileX0, int64_t tileX1, int64_t tileY0, int64_t tileY1) noexcept;

public:
	inline const size_t& GetMapWidth() const noexcept { return *mMapWidth; }
	inline const size_t& GetMapHeight() const noexcept { return *mMapHeight; }
	inline const FVector2D& GetMapRenderSize() const noexcept { return mMapRenderSize;  }
	inline const FVector2D& GetViewportSize() const noexcept { return mViewportSize; }
	inline const FVector2D& GetViewportMin() const noexcept { return mViewportMin; }
	inline const FVector2D& GetViewportMax() const noexcept { return mViewportMax; }
	inline const FVector2D& GetRenderOffset() const noexcept { return mRenderOffset; }
	inline float GetTileRenderSize() const noexcept { return mTileRenderSize; }
	inline float GetTileRenderSizeHalf() const noexcept { return mTileRenderSizeHalf; }
	inline size_t GetTileX0() const noexcept { return mTileX0; }
	inline size_t GetTileX1() const noexcept { return mTileX1; }
	inline size_t GetTileY0() const noexcept { return mTileY0; }
	inline size_t GetTileY1() const noexcept { return mTileY1; }
	inline float GetCamZoom() const noexcept { return *mCamZoom; }
};
