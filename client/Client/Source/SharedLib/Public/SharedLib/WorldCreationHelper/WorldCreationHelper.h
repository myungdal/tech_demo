// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/PCH/pch_packet.h"

#include "MmoSync/PCH/pch_mmoSync.h"
#include "MmoSync/Map/Configuration/FieldMapConfig.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 여러 종류의 Lib를 활용하여 월드를 구성하는 함수들을 정의합니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GameInstance;
class STATIC_MAP;
class STATIC_MAP_FSM_ACTION;
class STATIC_MAP_FSM_TRANSITION;

// 제거 예정
namespace WorldCreationHelper
{
	// STATIC_MAP 데이터를 기반으로 FieldMapConfig를 생성합니다.
	SHAREDLIB_API FieldMapConfig MakeGridConfig(const STATIC_MAP* staticMap);
}
