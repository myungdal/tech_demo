// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemBow.h"

#include "Common/Stat/statContainer/StatContainer.h"


SkillSystemBow::SkillSystemBow(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<PositionComponent>();
	IncludeComponent<CharacterComponent>();
	IncludeComponent<SkillComponentBow>();
	IncludeComponent<TacticsComponentCombat>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
}
void SkillSystemBow::Update()
{
	UpdateNormalSkill<SkillComponentBow, MotionComponentSkillB>();
}
