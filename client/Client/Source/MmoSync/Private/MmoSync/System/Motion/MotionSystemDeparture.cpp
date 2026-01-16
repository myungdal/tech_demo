// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Motion/MotionSystemDeparture.h"

#include "MmoSync/DebugControlTool/DebugInfo.h"


MotionSystemDeparture::MotionSystemDeparture(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	DurationSystemBase<MotionSystemDeparture, MotionComponentDeparture>(engine, timeStepUpdateCycle, game)
{
}
bool MotionSystemDeparture::OnMotionCompleted(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{	
	entity;
	// 내부 텔레포트라면 이동
	return true;
}
