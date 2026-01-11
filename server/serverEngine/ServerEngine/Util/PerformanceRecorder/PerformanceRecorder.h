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

class PerformanceRecorder final
{
public:	
	struct Record
	{
		uint64_t mCallCount = 0;
		double mTotalDuration = 0.;
		double mMaxDuration = 0.;
	};

private:
	using RecordMap = std::unordered_map<const wchar_t*, Record>;
	RecordMap mRecordMap;

public:
	void Push(const wchar_t* functionName, double duration);

	void Dump(OUT std::wostream& out) const;
};

thread_local inline PerformanceRecorder* tPerformanceRecorder = nullptr;
