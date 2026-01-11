// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

class FGameCameraZoom
{
protected:
	int mZoomIdx = 11;
	float mCameraZoomDest = 1.f;
	float mCameraZoom = 1.f;
	float mFollowerZoomBonus = 0.f; // 팔로워 수에 따른 추가 줌 보너스

public:
	DISABLE_COPY(FGameCameraZoom);
	explicit FGameCameraZoom();

public:
	void ZoomReset();
	void ZoomOut();
	void ZoomIn();

	void OnUpdateZoom();
	void SetFollowerZoomBonus(int followerCount);

public:
	inline float GetZoom() const { return mCameraZoom; }
	inline float GetZoomInversed() const { return MathUtil::IsZero(mCameraZoom) ? 0.f : 1.f / mCameraZoom; }
	inline float& RefZoom() { return mCameraZoom; }
};
