// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// pch_module.h - 모듈 import용 PCH
// ServerEngine 우산 모듈은 사용 불가 - import Shell; 로 대체

#pragma once

#pragma warning(disable: 5028)

// ServerEngine 기본 PCH
#include "ServerEngine/PCH/pch_serverEngine.h"

// 매크로 (모듈 export 불가 - 헤더 필수)
#include "ServerEngine/Log/LogInternal/LogQueue.h"
#include "ServerEngine/Core/Macros.h"

// 템플릿 함수 (PacketUtil/DbUtil은 모듈화 전환 - .cpp에서 import 권장)
#include "ServerEngine/Worker/WorkerManager.h"

// PacketUtil 템플릿 함수 (ForwardBypassPacketReq, DispatchPacketOnAnyThread 등)
#include "ServerEngine/Util/PacketUtil/PacketUtil.h"

