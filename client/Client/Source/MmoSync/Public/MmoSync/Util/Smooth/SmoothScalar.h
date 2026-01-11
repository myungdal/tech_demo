// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class MMOSYNC_API SmoothScalar
{
protected:
	static constexpr float SMOOTH_TIME = 0.1f;
	static constexpr float INVERSE_SMOOTH_TIME_2 = (2.f / SMOOTH_TIME);
	static constexpr float MAX_CHANGE = 300.f;

protected:
	float mVelocity = 0.f;

public:
	SmoothScalar();

public:
	float Update(float from, float to, float deltaTime);
};
