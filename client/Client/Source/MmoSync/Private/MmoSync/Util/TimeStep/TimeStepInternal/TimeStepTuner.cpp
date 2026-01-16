// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Util/TimeStep/TimeStepInternal/TimeStepTuner.h"


TimeStepTuner::TimeStepTuner(bool catchUpMode)
	:
	mCatchUpMode(catchUpMode)
{
}
void TimeStepTuner::Reset()
{
	mSpeed = 1.0;
}

void TimeStepTuner::TuneSpeed(double jitterTime, double queuedTime)
{
	// jitterTime 와 queuedTime 가 같으면 0
	const double t =
		(queuedTime - jitterTime * JITTER_COEFFICIENT) * TIME_COEFFICIENT;

	double sigmoid = DoubleSigmoid(t);

	if (mCatchUpMode)
	{
		const double speed = 1.0 + sigmoid;
		mSpeed = mSpeed * 0.5 + speed * 0.5;
	}
	else
	{
		// jitterTime 와 queuedTime 가 같으면 1, 크면 더 빠른 속력, 느리면 더 느린 속력.
		const double speed = 1.0 + sigmoid * SPEED_COEFFICIENT;

		// 평탄화.
		mSpeed = mSpeed * 0.9 + speed * 0.1;
	}
}

double TimeStepTuner::DoubleSigmoid(double t) const
{
	t *= 5.0;
	const double sigmoid = 1.0 / (1.0 + exp(-t));

	if (0.0 > t)
		return ((sigmoid * (1.0 - sigmoid) - 0.25) * 4.0);
	else
		return (-(sigmoid * (1.0 - sigmoid) - 0.25) * 4.0);
}
