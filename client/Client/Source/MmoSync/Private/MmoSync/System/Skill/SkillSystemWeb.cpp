// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemWeb.h"

#include "Common/Stat/statContainer/StatContainer.h"

#include "MmoSync/Util/WorldIffChecker/WorldIffChecker.h"


SkillSystemWeb::SkillSystemWeb(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<SkillComponentWeb>();
	IncludeComponent<TacticsComponentCombat>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
}
void SkillSystemWeb::Update()
{
	UpdateSkillRandomTarget<SkillComponentWeb, MotionComponentSkillB>();
}

bool SkillSystemWeb::OnCheckSkillTargetValid(MAYBE_UNUSED Entity& e)
{
	// 거미줄로 당겨야하는데, PhysicsComponent가 없으면 이동 불가여서 걸러야됨
	if (!e.HasComponent(PhysicsComponent::GetComponentType()))
		return false;

	return true;
}

void SkillSystemWeb::OnStartSkillMotion(
	MAYBE_UNUSED Entity& e0,
	MAYBE_UNUSED PositionComponent& p0,
	MAYBE_UNUSED Entity& e1,
	MAYBE_UNUSED PositionComponent& p1
)
{
	if (SkillComponentWeb* webComp = TryGetComponent<SkillComponentWeb>(e0.GetEntityIdp()))
	{
		webComp->renderSkillX = p1.x;
		webComp->renderSkillY = p1.y;
		webComp->renderSkillR = CELL_SIZE;
	}
}

void SkillSystemWeb::OnGiveDamage(
	MAYBE_UNUSED Entity& e0,
	MAYBE_UNUSED PositionComponent& p0,
	MAYBE_UNUSED Entity& e1,
	MAYBE_UNUSED PositionComponent& p1,
	MAYBE_UNUSED DamageInfo& damageInfo
)
{
	// 거미줄로 당겨야하는데, PhysicsComponent가 없으면 이동 불가처리하고 있음 (SkillSystemWeb::OnCheckSkillTargetValid 참고)
	_ASSERT_CRASH(e1.HasComponent(PhysicsComponent::GetComponentType()));

	{
		const Scalar dx = (p1.x - p0.x);
		const Scalar dy = (p1.y - p0.y);
		const auto [nx, ny] = MathUtil::GetVec1024(dx, dy);

		const Scalar x = p0.x + (((nx << CELL_SIZE_FACTOR)) >> 10);
		const Scalar y = p0.y + (((ny << CELL_SIZE_FACTOR)) >> 10);

		if (PositionComponent* pos1 = TryGetComponent<PositionComponent>(e1.GetEntityIdp()))
		{
			pos1->x = x;
			pos1->y = y;
		}
	}
	{
		MotionComponentWeb& c = AttachComponent<MotionComponentWeb>(e1);
		c.now = 0;
		c.duration = DebugInfo::MOTION_DURATION_WEB;
	}
}
