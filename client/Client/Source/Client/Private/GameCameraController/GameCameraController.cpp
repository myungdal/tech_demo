// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "GameCameraController.h"

#include "GameCameraController/Internal/GameCameraZoom.h"
#include "GameCameraController/Internal/GameCameraVelocity.h"
#include "GameCameraController/GameCameraViewport.h"


FGameCameraController::FGameCameraController(
	TSharedPtr<FGameCameraViewport> viewportForTile,
	TSharedPtr<FGameCameraZoom> cameraZoom,
	TSharedPtr<FGameCameraVelocity> cameraVelocity
)
	:
	mViewportForTile(viewportForTile),
	mCameraZoom(cameraZoom),
	mCameraVelocity(cameraVelocity)
{
}

void FGameCameraController::StartDragging(const FSlateRect& boundingRect, const FVector2D& cursorPos, double currentTime)
{
	if (true == mIsMoving)
		return;

	mIsMoving = true;
	mCursorPosSaved = cursorPos;
	mCursorPosCurrent = cursorPos;
	mTimeCurrent = currentTime;
	mCamTargetSaved = mCamTarget;
	mCamTargetDest = mCamTarget;

	mCameraVelocity->OnStartMove();

	_DEBUG_LOG(WHITE, L"{}, {}", mCursorPosSaved.X, mCursorPosSaved.Y);
}

void FGameCameraController::EndDragging(double currentTime)
{
	mIsMoving = false;

	mCameraVelocity->OnEndMove(currentTime);

	mCamTargetDest = mCamTarget;

	_DEBUG_LOG(WHITE, L"{}, {}", mCursorPosSaved.X, mCursorPosSaved.Y);
}

void FGameCameraController::UpdateDragging(const FSlateRect& boundingRect, const FVector2D& cursorPos, double currentTime)
{
	if (false == mIsMoving)
		return;

	{
		const FVector2D cursorPosDelta = cursorPos - mCursorPosSaved;
		mCamTarget = mCamTargetSaved - cursorPosDelta * mCameraZoom->GetZoomInversed();

		ClampCamTarget(mCamTarget, boundingRect);
		mCamTargetDest = mCamTarget;
	}

	{
		const FVector2D cursorPosDelta = cursorPos - mCursorPosCurrent;
		mCursorPosCurrent = cursorPos;

		double timeDelta = currentTime - mTimeCurrent;
		mTimeCurrent = currentTime;

		mCameraVelocity->OnUpdateMove(timeDelta, currentTime, cursorPosDelta);
	}
}

void FGameCameraController::ClampCamTarget(const FVector2D& camTarget, const FSlateRect& boundingRect)
{
	mCamTarget = camTarget;

	const FVector2D halfViewportSize = 0.5f * mViewportForTile->GetViewportSize() * mCameraZoom->GetZoomInversed();
	const FVector2D mapRenderSize = mViewportForTile->GetMapRenderSize() * mCameraZoom->GetZoomInversed();

	//const FVector2D min = halfViewportSize;
	//const FVector2D max = mapRenderSize - halfViewportSize;

	const FVector2D min = halfViewportSize * 0.3f;
	const FVector2D max = mapRenderSize - halfViewportSize * 0.3f;

	if (min.X < max.X)
	{
		if ((min.X > mCamTarget.X) && (max.X < mCamTarget.X)) { mCamTarget.X = (min.X + max.X) * 0.5f; }
		else if (min.X > mCamTarget.X) { mCamTarget.X = min.X; }
		else if (max.X < mCamTarget.X) { mCamTarget.X = max.X; }
	}
	else
	{
		if (min.X < mCamTarget.X) { mCamTarget.X = min.X; }
		else if (max.X > mCamTarget.X) { mCamTarget.X = max.X; }
	}

	if (min.Y < max.Y)
	{
		if ((min.Y > mCamTarget.Y) && (max.Y < mCamTarget.Y)) { mCamTarget.Y = (min.Y + max.Y) * 0.5f; }
		else if (min.Y > mCamTarget.Y) { mCamTarget.Y = min.Y; }
		else if (max.Y < mCamTarget.Y) { mCamTarget.Y = max.Y; }
	}
	else
	{
		if (min.Y < mCamTarget.Y) { mCamTarget.Y = min.Y; }
		else if (max.Y > mCamTarget.Y) { mCamTarget.Y = max.Y; }
	}
}

void FGameCameraController::UpdateDamping(const FSlateRect& boundingRect, double currentTime)
{
	if (true == mCameraVelocity->IsDamping())
	{
		const double timeDelta = currentTime - mTimeCurrent;
		mTimeCurrent = currentTime;

		mCamTarget += mCameraVelocity->OnUpdateDamping(timeDelta);
		ClampCamTarget(mCamTarget, boundingRect);
		mCamTargetDest = mCamTarget;
	}

	mCamTarget = mCamTarget * 0.9f + mCamTargetDest * 0.1f;
	ClampCamTarget(mCamTarget, boundingRect);

	mCameraZoom->OnUpdateZoom();
}

void FGameCameraController::ZoomOut(const FSlateRect& boundingRect)
{
	mCameraZoom->ZoomOut();
	ClampCamTarget(mCamTarget, boundingRect);
	mCamTargetDest = mCamTarget;
}

void FGameCameraController::ZoomIn(const FSlateRect& boundingRect)
{
	mCameraZoom->ZoomIn();
	ClampCamTarget(mCamTarget, boundingRect);
	mCamTargetDest = mCamTarget;
}

void FGameCameraController::Move(const FVector2D& delta, const FSlateRect& boundingRect)
{
	mCamTarget += delta;
	ClampCamTarget(mCamTarget, boundingRect);
	mCamTargetDest = mCamTarget;
}

void FGameCameraController::SetCamTargetSmooth(const FVector2D& camTarget)
{ 
	mCamTargetDest = camTarget; 
	mCursorPosSaved = mCursorPosCurrent;
	mCameraVelocity->Velocity() = FVector2D::Zero();
}
