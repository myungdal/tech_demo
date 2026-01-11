// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemSword.h"

#include "Common/Stat/statContainer/StatContainer.h"


SkillSystemSword::SkillSystemSword(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<PositionComponent>();
	IncludeComponent<CharacterComponent>();
	IncludeComponent<SkillComponentSword>();
	IncludeComponent<TacticsComponentCombat>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
}
void SkillSystemSword::Update()
{
	UpdateNormalSkill<SkillComponentSword, MotionComponentSkillB>();
}
