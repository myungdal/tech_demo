// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// Test.ixx - Test Module Interface
module;

#include "Test/PCH/pch.h"

// Test headers
#include "Test/App/TestAppBase.h"

export module Test;

// ServerEngine modules
export import ServerEngineCore;
export import ServerEngineApp;
export import ServerEngineLog;
export import ServerEngineWorker;
export import ServerEngineNetwork;
export import ServerEngineUtil;
export import ServerEnginePacket;

// Test split modules
export import TestWorker;
export import TestNetwork;

export
{
	using ::TeeWstreambuf;
	using ::TestAppBase;
}

