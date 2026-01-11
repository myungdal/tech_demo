// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Util/Smooth/SmoothScalar.h"


SmoothScalar::SmoothScalar()
{
}
float SmoothScalar::Update(float from, float to, float deltaTime)
{
	// 감쇠 계수 계산: e^(-x) ≈ 1/(1 + x + 0.48x² + 0.235x³) (테일러 급수 근사)
	const float x = INVERSE_SMOOTH_TIME_2 * deltaTime;
	const float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
	
	// 현재 위치에서 목표까지의 변화량 계산 및 클램핑 (급격한 변화 방지)
	const float change = from - to;
	const float maxChange = MAX_CHANGE * SMOOTH_TIME;
	const float clampedChange = MathUtil::Clamp(change, -maxChange, maxChange);
	
	// 클램핑된 변화량을 적용한 조정된 목표값
	const float clampedTo = from - clampedChange;
	
	// 속도 기반 변화량 계산 (현재 속도 + 목표 방향 가속도)
	const float temp = (mVelocity + clampedChange * INVERSE_SMOOTH_TIME_2) * deltaTime;
	
	// 속도 업데이트: 감쇠 적용하여 점진적으로 0에 수렴
	mVelocity = (mVelocity - temp * INVERSE_SMOOTH_TIME_2) * exp;
	
	// 최종 보간 값 반환: 조정된 목표 + 감쇠된 변화량
	return clampedTo + (clampedChange + temp) * exp;
}
