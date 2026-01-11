// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeStepTuner
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MMOSYNC_API TimeStepTuner final
{
private:
	static constexpr double JITTER_COEFFICIENT = 3.0;
	static constexpr double TIME_COEFFICIENT = (1 / 300.0);
	static constexpr double SPEED_COEFFICIENT = 0.2;

	double mSpeed = 1.0;
	bool mCatchUpMode = false;

public:
	DISABLE_COPY(TimeStepTuner);
	explicit TimeStepTuner(bool catchUpMode);

public:
	void Reset();

	double GetSpeed() const { return mSpeed; }

	// 평균 지터와 큐의 균형을 이루기 위한 speed 를 조율한다
	void TuneSpeed(double jitterTime, double queuedTime);

private:
	// t = -1 ~ +1 일 때, 반환 값은 -1 ~ +1
	double DoubleSigmoid(double t) const;
};
