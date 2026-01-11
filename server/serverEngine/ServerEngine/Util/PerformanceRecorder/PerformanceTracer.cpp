// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "PerformanceTracer.h"


PerformanceTracer::PerformanceTracer(const wchar_t* functionName)
	:
	mFunctionName(functionName),
	mStartTime(std::chrono::high_resolution_clock::now())
{
}
PerformanceTracer::~PerformanceTracer()
{
	if (nullptr == tPerformanceRecorder)
		return;
	const std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	const std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(now - mStartTime);

	if (now > mStartTime) 
	{
		tPerformanceRecorder->Push(mFunctionName, duration.count() / 1000000.0); // micro -> second
	}
	else 
	{
		tPerformanceRecorder->Push(mFunctionName, 0);
	}
}
