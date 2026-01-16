// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemBlocking.h"

#include "Common/Stat/statContainer/StatContainer.h"
#include "MmoSync/Util/EntityUtil/EntityUtil.h"
#include "MmoSync/Grid/MultiGrid.h"


SkillSystemBlocking::SkillSystemBlocking(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<SkillComponentBlocking>();
	IncludeComponent<TacticsComponentCombat>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
}
void SkillSystemBlocking::Update()
{
	UpdateNormalSkill<SkillComponentBlocking, MotionComponentSkillB>();
}

void SkillSystemBlocking::OnGiveDamage(
	MAYBE_UNUSED Entity& e0,
	MAYBE_UNUSED PositionComponent& p0,
	MAYBE_UNUSED Entity& e1,
	MAYBE_UNUSED PositionComponent& p1,
	MAYBE_UNUSED DamageInfo& damageInfo
)
{
	const Scalar x = p0.x;
	const Scalar y = p0.y;

	std::deque<EntityIdp> result;

	WorldManager& worldManager = GetWorldManager();
	worldManager.PickEntityInCircle(
		x, y,
		MathUtil::Pow(CELL_SIZE),
		OUT result
	);

	const StatValue halfDamage = (damageInfo.damage >> 1);
	const PcComponent* pc0 = TryGetComponent<PcComponent>(e0);
	const NpcComponent* npc0 = TryGetComponent<NpcComponent>(e0);
	const EntityIdp& entityIdp0 = e0.GetEntityIdp();

	for (EntityIdp entityIdp1 : result)
	{
		Entity* entity1 = GetEntity(entityIdp1);
		if (!entity1)
			continue;

		if (entity1->HasComponent(AbilityComponentUntargetrable::GetComponentType()))
			continue;

		const PcComponent* pc1 = TryGetComponent<PcComponent>(*entity1);
		const NpcComponent* npc1 = TryGetComponent<NpcComponent>(*entity1);
		if (false == WorldIffChecker::IsFriend(*this, entityIdp0, entityIdp1, pc0, npc0, pc1, npc1))
		{
			DamageComponent& c = AttachComponent<DamageComponent>(*entity1);
			DamageInfo info;
			info.stun = false;
			info.pushBack = true;
			info.damage = halfDamage;
			info.lastAttacker = e0.GetEntityIdp();
			AddDamageInfo(c, info);
		}
	}
}
