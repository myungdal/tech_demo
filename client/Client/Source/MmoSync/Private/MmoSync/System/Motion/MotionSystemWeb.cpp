// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Motion/MotionSystemWeb.h"

#include "MmoSync/DebugControlTool/DebugInfo.h"


MotionSystemWeb::MotionSystemWeb(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	DurationSystemBase<MotionSystemWeb, MotionComponentWeb>(engine, timeStepUpdateCycle, game)
{
}
void MotionSystemWeb::OnMotionStarted(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	SetSpeed(entity, -1);
}

bool MotionSystemWeb::OnMotionCompleted(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{	
	SetSpeed(entity, 0);
	return true;
}
