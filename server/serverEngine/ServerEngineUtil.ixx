// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ServerEngineUtil.ixx - Util Module (경량)
//
// 경량 Util 모듈 구성:
//   StringUtil, TimeUtil, UuidUtil, TlsCommonBuffer, CommandDispatcher,
//   PerformanceRecorder, NetworkUtil, SocketUtil, MwsrQueue, SafeMap
//
// 별도 export:
//   - PacketUtil: ServerEnginePacketUtil 모듈로 전환 (권장: import ServerEngineUtil;)
//
// 분리된 모듈:
//   - ServerEngineDev: DevPacketConverter 제공

module;

// GMF - 경량 Util
#include "ServerEngine/Util/StringUtil/StringUtil.h"
#include "ServerEngine/Util/TimeUtil/TimeUtil.h"
#include "ServerEngine/Util/UuidUtil/UuidUtil.h"
#include "ServerEngine/Util/TlsCommonBuffer/TlsCommonBuffer.h"
#include "ServerEngine/Util/CommandDispatcher/CommandDispatcherWithResult.h"
#include "ServerEngine/Util/CommandDispatcher/CommandDispatcherWithPromise.h"
#include "ServerEngine/Util/PerformanceRecorder/PerformanceRecorder.h"
#include "ServerEngine/Util/PerformanceRecorder/PerformanceRecorderContainer.h"
#include "ServerEngine/Util/PerformanceRecorder/PerformanceTracer.h"
#include "ServerEngine/Util/NetworkUtil/NetworkUtil.h"
#include "ServerEngine/Util/SocketUtil/SocketUtil.h"
#include "ServerEngine/Util/MwsrQueue/MwsrQueue.h"
#include "ServerEngine/Util/SafeMap/SafeMap.h"

export module ServerEngineUtil;

// ============= PacketUtil (Template) =============
export import ServerEnginePacketUtil;

// ============= 경량 Util =============
export using ::PerformanceRecorder;
export using ::PerformanceRecorderContainer;
export using ::PerformanceTracer;
export using ::SocketUtil;

export namespace NetworkUtil {
	using ::NetworkUtil::IsMyIp;
	using ::NetworkUtil::GetMyIpList;
	using ::NetworkUtil::GetAddr;
	using ::NetworkUtil::GetPort;
	using ::NetworkUtil::CreateSocket;
	using ::NetworkUtil::SetSendBufSize;
	using ::NetworkUtil::SetRecvBufSize;
	using ::NetworkUtil::SetNoDelay;
	using ::NetworkUtil::SetKeepAlive;
	using ::NetworkUtil::SetReuseAddr;
	using ::NetworkUtil::SetLinger;
	using ::NetworkUtil::BindAnyIpAddress;
	using ::NetworkUtil::UpdateAcceptContext;
	using ::NetworkUtil::Encrypt;
}

export namespace TimeUtil {
	using ::TimeUtil::ValidateStrTime;
	using ::TimeUtil::TT_FROM_STR;
	using ::TimeUtil::TT_FROM_NUM;
	using ::TimeUtil::TT_FROM_TS;
	using ::TimeUtil::TS_FROM_TT;
}

