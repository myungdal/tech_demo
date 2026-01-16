// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "PerformanceRecorder.h"


void PerformanceRecorder::Push(const wchar_t* functionName, double duration)
{
	Record& record = mRecordMap[functionName];

	++record.mCallCount;
	record.mTotalDuration += duration;

	if (record.mMaxDuration < duration)
		record.mMaxDuration = duration;
}

void PerformanceRecorder::Dump(MAYBE_UNUSED OUT std::wostream& out) const
{
	for (const auto& [functionName, record] : mRecordMap)
	{
		out <<
			functionName << L"\t" <<
			record.mCallCount << L"\t" <<
			record.mTotalDuration << L"\t" <<
			record.mTotalDuration / record.mCallCount << L"\t" <<
			record.mMaxDuration << std::endl;
	}
}
