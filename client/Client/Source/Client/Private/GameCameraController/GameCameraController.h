// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class FGameCameraViewport;
class FGameCameraZoom;
class FGameCameraVelocity;
class FSlateRect;

class FGameCameraController
{
private:
	bool mIsMoving = false;
	FVector2D mCamTarget{ 0.f, 0.f };
	FVector2D mCamTargetDest{ 0.f, 0.f };
	FVector2D mCamTargetSaved{ 0.f, 0.f };
	FVector2D mCursorPosSaved{ 0.f, 0.f };
	FVector2D mCursorPosCurrent{ 0.f, 0.f };
	double mTimeCurrent = 0.0;

	const TSharedPtr<FGameCameraViewport> mViewportForTile = nullptr;

	TSharedPtr<FGameCameraZoom> mCameraZoom = nullptr;
	TSharedPtr<FGameCameraVelocity> mCameraVelocity = nullptr;

public:
	DISABLE_COPY(FGameCameraController);
	explicit FGameCameraController(
		TSharedPtr<FGameCameraViewport> viewportForTile,
		TSharedPtr<FGameCameraZoom> cameraZoom,
		TSharedPtr<FGameCameraVelocity> cameraVelocity
	);

public:
	void StartDragging(const FSlateRect& boundingRect, const FVector2D& cursorPos, double currentTime);
	void EndDragging(double currentTime);
	void UpdateDragging(const FSlateRect& boundingRect, const FVector2D& cursorPos, double currentTime);

private:
	void ClampCamTarget(const FVector2D& camTarget, const FSlateRect& boundingRect);

public:
	void UpdateDamping(const FSlateRect& boundingRect, double currentTime);

public:
	void ZoomReset(const FSlateRect& boundingRect);
	void ZoomOut(const FSlateRect& boundingRect);
	void ZoomIn(const FSlateRect& boundingRect);

	void Move(const FVector2D& delta, const FSlateRect& boundingRect);

public:
	bool IsMoving() const { return mIsMoving; }
	void SetCamTarget(const FVector2D& camTarget) { mCamTarget = camTarget; mCamTargetDest = camTarget; }
	void SetCamTargetSmooth(const FVector2D& camTarget);
	FVector2D& RefCamTarget() { return mCamTarget; }
	const FVector2D& GetCamTarget() const { return mCamTarget; }
};
