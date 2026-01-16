// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Util/Smooth/SmoothRadian.h"


SmoothRadian::SmoothRadian()
{
}
float SmoothRadian::Update(float from, float to, float deltaTime)
{
	while ((to - from) > FLOAT_PI)
	{ 
		from += (FLOAT_PI * 2.0f);
	}
	while ((to - from) < -FLOAT_PI)
	{ 
		from -= (FLOAT_PI * 2.0f);
	}
	const float x = INVERSE_SMOOTH_TIME_2 * deltaTime;
	// 지수적 감쇠(exp) 계산을 위한 변수입니다.
	const float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
	// 변화량을 계산하고 최대 변경 가능한 값을 고려하여 변화량을 클램핑합니다.
	const float change = from - to;
	const float maxChange = MAX_CHANGE * SMOOTH_TIME;
	const float clampedChange = MathUtil::Clamp(change, -maxChange, maxChange);
	// 'to' 위치를 조정하여 최대 변화량을 초과하지 않도록 합니다.
	const float clampedTo = from - clampedChange;
	// 중간(temporary) 속도 항을 계산합니다. (SmoothDamp 계열 공식의 temp 변수)
	const float temp = (mVelocity + clampedChange * INVERSE_SMOOTH_TIME_2) * deltaTime;
	// 속도(velocity)를 업데이트합니다.
	mVelocity = (mVelocity - temp * INVERSE_SMOOTH_TIME_2) * exp;
	// 최종 값을 계산하여 반환합니다.
	return clampedTo + (clampedChange + temp) * exp;
}
