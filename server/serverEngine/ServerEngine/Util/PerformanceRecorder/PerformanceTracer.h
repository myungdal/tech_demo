// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PerformanceTracer
{
private:
	const wchar_t* mFunctionName;
	const std::chrono::high_resolution_clock::time_point mStartTime;
	
public:
	explicit PerformanceTracer(const wchar_t* functionName);
	~PerformanceTracer();
};

