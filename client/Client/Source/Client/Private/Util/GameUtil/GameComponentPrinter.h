// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


struct ComponentBase;
class WorldManager;

// formatter에서 사용하는 thread_local 변수들
extern thread_local std::wstring tTabStr;
extern thread_local WorldManager* tWorldManager;

namespace ComponentsPrinter
{
	FString Print(const ComponentBase& base, ComponentType componentType);
}
