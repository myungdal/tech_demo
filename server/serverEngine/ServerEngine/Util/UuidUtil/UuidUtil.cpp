// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "UuidUtil.h"


namespace UuidUtil
{
	Uuid GenerateUuid()
	{
		_ASSERT_CRASH(tThreadId != INVALID_THREAD_ID);

		// 시간 조작의 영향을 받지 않도록, Global 이 아닌 Local 시간을 사용한다.
		Uuid uuid(tClock.GetLocalNowMs(), static_cast<uint8_t>(gMyAppId), static_cast<uint8_t>(tThreadId), ++tUuidSeq);
		return uuid;
	}

	LogId GenerateLogId()
	{
		return GenerateUuid();
	}
}
