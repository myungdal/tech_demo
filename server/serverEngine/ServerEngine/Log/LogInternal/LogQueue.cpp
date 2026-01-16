// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "LogQueue.h"


void LogQueue::Push(Log* log)
{
	mTimerTaskTable.Push(log);
}

Log* LogQueue::Pop(OUT size_t& count)
{
	return mTimerTaskTable.Pop(count);
}
