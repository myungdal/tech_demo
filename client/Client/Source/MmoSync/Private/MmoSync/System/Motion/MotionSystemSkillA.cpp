// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Motion/MotionSystemSkillA.h"

#include "MmoSync/Component/SkillComponent.h"


MotionSystemSkillA::MotionSystemSkillA(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	DurationSystemBase<MotionSystemSkillA, MotionComponentSkillA>(engine, timeStepUpdateCycle, game)
{
}
bool MotionSystemSkillA::OnMotionCompleted(MAYBE_UNUSED Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	return true;
}
