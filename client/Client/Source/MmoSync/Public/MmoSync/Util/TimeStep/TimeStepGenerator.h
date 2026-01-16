// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeStep
// Host 가 사용한다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MMOSYNC_API TimeStepGenerator final
{
private:
	const int64_t mInterval = 0;
	ClockPoint mStartTime;

	TimeStep mGeneratedStepCount = 0;

public:
	DISABLE_COPY(TimeStepGenerator);
	explicit TimeStepGenerator(int64_t interval, ClockPoint now);

public:
	void Reset(ClockPoint now);
	TimeStep Generate(ClockPoint now); 
};
