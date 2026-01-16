// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// TestNetwork.ixx - Test Network Module Interface
module;

#pragma warning(disable: 5028)

#include "Test/PCH/pch.h"

#include "Test/TestSocketStateMachine/TestSocketStateMachine.h"

export module TestNetwork;

export import ServerEngineCore;
export import ServerEngineNetwork;
export
{
	using ::TestSocketStateMachine;
}
