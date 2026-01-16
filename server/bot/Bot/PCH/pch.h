// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PCH/pch_serverEngine.h"

#include "MmoSync/PCH/pch_mmoSync.h"

// ValuePerTime - BotScene에서 사용 (모듈 기본 인수 재정의 방지)
#include "Common/Time/ValuePerTime.h"


#define BOT_DATA_PATH "../../data/botStatistics/"
#define MAKE_BOT_DATA_FILE_PATH(filename) MARGE_FILE_PATH(BOT_DATA_PATH, filename)
#define _DEBUG_BOT_WHITE PrintOutputLog(WHITE, __FUNCTIONW__, __LINE__, L"[{}]", GetBotId())
using BotId = int64_t;
constexpr BotId INVALID_BOT_ID = -1;
constexpr size_t LIMIT_BOT_COUNT = 10000;

using BotSceneSeq = uint16_t;
constexpr BotSceneSeq INVALID_BOT_SCENE_SEQ = UINT16_MAX;

class Bot;
using BotPtr = std::shared_ptr<Bot>;
using BotPtrList = std::list<BotPtr>;

