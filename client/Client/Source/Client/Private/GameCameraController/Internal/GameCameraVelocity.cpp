// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "GameCameraVelocity.h"

#include "Math/UnrealMathUtility.h"

void FGameCameraVelocity::OnStartMove()
{
	mHasVelocity = false;
	mVelocity = FVector2D::Zero();
}

void FGameCameraVelocity::OnEndMove(double timeTick)
{
	if(mHasVelocity)
		mIsDamping = (MAX_TIME_DELTA_VELOCITY_SETTING > (timeTick - mVelocitySettingTime)) ? true : false;
}

void FGameCameraVelocity::OnUpdateMove(double timeDelta, double timeTick, const FVector2D& cursorPosDelta)
{
	if (FMath::IsNearlyZero(timeDelta) ||
		(FMath::IsNearlyZero(cursorPosDelta.X) &&
		FMath::IsNearlyZero(cursorPosDelta.Y)))
	{
		if(mIsDamping)
			mIsDamping = (MAX_TIME_DELTA_VELOCITY_SETTING > (timeTick - mVelocitySettingTime)) ? true : false;
		
		return;
	}

	mHasVelocity = true;
	mVelocitySettingTime = timeTick;
	mVelocity = -cursorPosDelta / timeDelta;
}

FVector2D FGameCameraVelocity::OnUpdateDamping(double timeDelta)
{
	if(false == mIsDamping)
		return FVector2D{ 0.f, 0.f };

	mVelocity *= 0.95f;

	if (MathUtil::DistanceSq(mVelocity.X, mVelocity.Y) < 1.f)
	{
		mIsDamping = false;
		return FVector2D{ 0.f, 0.f };
	}

	return mVelocity * timeDelta;
}
