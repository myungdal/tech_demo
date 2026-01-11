// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

class FGameCameraVelocity
{
private:
	static constexpr double MAX_TIME_DELTA_VELOCITY_SETTING = 0.2;

	bool mHasVelocity = false;
	bool mIsDamping = false;
	double mVelocitySettingTime = 0.0;
	FVector2D mVelocity{ 0.f, 0.f };

public:
	void OnStartMove();
	void OnEndMove(double timeTick);
	void OnUpdateMove(double timeDelta, double timeTick, const FVector2D& cursorPosDelta);
	FVector2D OnUpdateDamping(double timeDelta);

public:
	bool IsDamping() const { return mIsDamping; }
	FVector2D& Velocity() { return mVelocity; }
};
