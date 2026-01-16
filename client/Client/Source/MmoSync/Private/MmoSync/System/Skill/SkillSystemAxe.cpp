// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemAxe.h"

#include "Common/Stat/statContainer/StatContainer.h"

#include "MmoSync/Util/WorldIffChecker/WorldIffChecker.h"


SkillSystemAxe::SkillSystemAxe(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<PositionComponent>();
	IncludeComponent<CharacterComponent>();
	IncludeComponent<SkillComponentAxe>();
	IncludeComponent<TacticsComponentCombat>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();	
}
void SkillSystemAxe::Update()
{
	UpdateNormalSkill<SkillComponentAxe, MotionComponentSkillB>();
}

void SkillSystemAxe::OnStartSkillMotion(
	MAYBE_UNUSED Entity& e0,
	MAYBE_UNUSED PositionComponent& p0,
	MAYBE_UNUSED Entity& e1,
	MAYBE_UNUSED PositionComponent& p1
)
{
	const Scalar dx = (p1.x - p0.x);
	const Scalar dy = (p1.y - p0.y);
	const Angle rotation = MathUtil::ArcTan(dx, dy);
	const Angle halfAngle = ANGLE_60;
	//const Scalar sqRange = MathUtil::Pow(CELL_SIZE);

	if (SkillComponentAxe* axeComp = TryGetComponent<SkillComponentAxe>(e0.GetEntityIdp()))
	{
		axeComp->renderSkillX = p0.x;
		axeComp->renderSkillY = p0.y;
		axeComp->renderSkillR = CELL_SIZE;
		axeComp->renderSkillRotation = rotation;
		axeComp->renderSkillHalfAngle = halfAngle;
	}
}

void SkillSystemAxe::OnGiveDamage(
	MAYBE_UNUSED Entity& e0,
	MAYBE_UNUSED PositionComponent& p0,
	MAYBE_UNUSED Entity& e1,
	MAYBE_UNUSED PositionComponent& p1,
	MAYBE_UNUSED DamageInfo& damageInfo
)
{
	const Scalar dx = (p1.x - p0.x);
	const Scalar dy = (p1.y - p0.y);
	const Angle rotation = MathUtil::ArcTan(dx, dy);
	//const Angle halfAngle = ANGLE_60;
	const Scalar sqRange = MathUtil::Pow(CELL_SIZE);

	std::deque<EntityIdp> result;

	WorldManager& worldManager = GetWorldManager();
	worldManager.PickEntityInSector(   
		p0.x, p0.y,
		sqRange,
		rotation,
		ANGLE_60,
		0,
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
			info.pushBack = true;
			info.damage = halfDamage;
			info.lastAttacker = e0.GetEntityIdp();
			AddDamageInfo(c, info);
		}
	}
}
