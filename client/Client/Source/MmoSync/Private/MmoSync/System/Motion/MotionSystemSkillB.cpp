// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Motion/MotionSystemSkillB.h"

#include "MmoSync/Component/SkillComponent.h"


MotionSystemSkillB::MotionSystemSkillB(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	DurationSystemBase<MotionSystemSkillB, MotionComponentSkillB>(engine, timeStepUpdateCycle, game)
{
}
bool MotionSystemSkillB::OnMotionCompleted(MAYBE_UNUSED Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	return true;
}
