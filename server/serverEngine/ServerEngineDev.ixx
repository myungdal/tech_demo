// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ServerEngineDev.ixx - DevPacketConverter 모듈
//
// DevPacketConverter: 개발/디버그 전용
// 주의: ServerEngine(우산 모듈)로 재-export하지 않는다.

module;

// DevPacketConverter 구현
#include "ServerEngine/Lock/Lock.h"
#include "ServerEngine/Util/DevPacketConverter/DevPacketConverter.h"

export module ServerEngineDev;

export using ::DevPacketConverter;
#ifdef _DEBUG
export using ::gDevPacketConverter;
#endif
