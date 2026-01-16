// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// pch_module.h - 모듈 import용 PCH
// ServerEngine 우산 모듈은 사용 불가 - import Bot; 로 대체

#pragma once

#pragma warning(disable: 5028)

// Bot 기본 PCH (pch_serverEngine.h + pch_mmoSync.h + Bot 전용 타입)
#include "Bot/PCH/pch.h"

// 매크로 (모듈 export 불가 - 헤더 필수)
#include "ServerEngine/Log/LogInternal/LogQueue.h"
#include "ServerEngine/Core/Macros.h"

// 템플릿 함수 (PacketUtil/DbUtil은 모듈화 전환 - .cpp에서 import 권장)
#include "ServerEngine/Worker/WorkerManager.h"

// PacketUtil 템플릿 함수 (ForwardBypassPacketReq, DispatchPacketOnAnyThread 등)
#include "ServerEngine/Util/PacketUtil/PacketUtil.h"

// DevPacketConverter (Bot에서 사용)
#include "ServerEngine/Util/DevPacketConverter/DevPacketConverter.h"

// UserCache 클래스 (BotSceneGame 등에서 사용)
#include "Packet/UserCache/UserItem/UserItemRow.h"
#include "Packet/UserCache/UserItem/UserItemTable.h"
#include "Packet/UserCacheAccessor/UserCacheAccessor.h"

