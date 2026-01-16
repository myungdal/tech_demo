// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// TestWorker.ixx - Test Worker Module Interface
module;

#pragma warning(disable: 5028)

#include "Test/PCH/pch.h"

#include "Test/TestCoWorker/TestCoWorker.h"

export module TestWorker;

export import ServerEngineCore;
export import ServerEngineWorker;
export
{
	using ::TestCoWorker;
}
