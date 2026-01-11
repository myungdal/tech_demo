// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// MainServerUtil.ixx - MainServer Util Module Interface
module;

// C5028 경고 비활성화: 모듈과 헤더 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "MainServer/PCH/pch.h"

// Util 헤더들
#include "MainServer/Util/MainSocketUtil/MainSocketUtil.h"

export module MainServerUtil;

// 필요한 모듈 import
export import ServerEngineCore;
export import ServerEngineUtil;

export
{
	using ::MainSocketUtil;
}

