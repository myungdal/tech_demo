// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


thread_local inline uint16_t tUuidSeq = 0;

namespace UuidUtil
{
	Uuid GenerateUuid();
	LogId GenerateLogId();
}

