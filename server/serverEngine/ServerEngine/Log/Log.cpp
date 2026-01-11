// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Log.h"

#include "Common/MemPool/FixedPool.h"


namespace
{
	using LogPool = FixedPool<Log, 14>;
	inline LogPool& GetLogPool()
	{
		static LogPool pool{};
		return pool;
	}
}

Log* Log::Acquire()
{
	return GetLogPool().Pop();
}

void Log::Dispose()
{
	GetLogPool().Push(this);
}

