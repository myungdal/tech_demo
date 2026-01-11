// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "GameCameraZoom.h"


namespace
{
	constexpr float gZoomScaleList[] = {
		0.70f, 0.90f, 1.10f, 1.30f, 1.50f, 1.70f,
		1.90f, 2.10f, 2.30f, 2.50f, 2.60f, 2.70f, 2.80f, 2.90f
	};
}

FGameCameraZoom::FGameCameraZoom()
{
	ZoomReset();
}

void FGameCameraZoom::ZoomReset()
{
	size_t camZoomCnt = sizeof(gZoomScaleList) / sizeof(gZoomScaleList[0]);
	mZoomIdx = (camZoomCnt >> 1);
	mCameraZoomDest = gZoomScaleList[mZoomIdx];	
}

void FGameCameraZoom::ZoomOut()
{
	++mZoomIdx;
	size_t camZoomCnt = sizeof(gZoomScaleList) / sizeof(gZoomScaleList[0]);
	if ((camZoomCnt - 1) < mZoomIdx)
	{
		mZoomIdx = (camZoomCnt - 1);
	}

	mCameraZoomDest = gZoomScaleList[mZoomIdx];
}

void FGameCameraZoom::ZoomIn()
{
	--mZoomIdx;
	if (0 > mZoomIdx) mZoomIdx = 0;
	mCameraZoomDest = gZoomScaleList[mZoomIdx];
}

void FGameCameraZoom::OnUpdateZoom()
{
	// 기본 줌 + 팔로워 보너스 줌 적용
	const float targetZoom = mCameraZoomDest + mFollowerZoomBonus;
	mCameraZoom = mCameraZoom * 0.8f + targetZoom * 0.2f;
}

void FGameCameraZoom::SetFollowerZoomBonus(int followerCount)
{
	// 팔로워 1마리당 -0.1씩 줌 아웃 (더 넓게 보임)
	// 줌 값이 작을수록 줌 아웃 (더 넓은 시야)
	// 예: 팔로워 5마리 = -0.5 줌 아웃
	mFollowerZoomBonus = followerCount * -0.001f;
}
	
