// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbServerStaticDb.ixx - DbServer StaticDb Module Interface
module;

// C5028 경고 비활성화: 모듈과 헤더 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "DbServer/PCH/pch.h"

// StaticDb 헤더
#include "DbServer/StaticDb/StaticDbLoader.h"

export module DbServerStaticDb;

// 필요한 모듈 import
export import ServerEngineCore;
export import ServerEngineDb;

export
{
	using ::StaticDbLoader;
}

