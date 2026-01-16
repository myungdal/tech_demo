// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbServerUtil.ixx - DbServer Util Module Interface
module;

// C5028 경고 비활성화: 모듈과 헤더 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "DbServer/PCH/pch.h"

// Util 헤더들
#include "DbServer/Util/DbSocketUtil/DbSocketUtil.h"
#include "DbServer/Util/DbUserUtil/DbUserDataMaker.h"
#include "DbServer/Util/RewardUtil/RewardUtil.h"

// DbUserUtil.h는 DbUtil 템플릿 사용으로 #include 유지 필요

export module DbServerUtil;

// 필요한 모듈 import
export import ServerEngineCore;
export import ServerEngineUtil;
export import DbServerItemUtil;

export
{
	using ::DbSocketUtil;
	using ::DbUserDataMaker;
	
	// namespace는 자동 export됨
	namespace RewardUtil {}
}

