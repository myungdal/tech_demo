// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemLaser.h"

#include "Common/Stat/statContainer/StatContainer.h"

#include "MmoSync/Util/WorldIffChecker/WorldIffChecker.h"


SkillSystemLaser::SkillSystemLaser(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<SkillComponentLaser>();
	IncludeComponent<TacticsComponentCombat>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
}
void SkillSystemLaser::Update()
{
	UpdateNormalSkill<SkillComponentLaser, MotionComponentSkillA>();
}

void SkillSystemLaser::OnStartSkillMotion(
	MAYBE_UNUSED Entity& e0,
	MAYBE_UNUSED PositionComponent& p0,
	MAYBE_UNUSED Entity& e1,
	MAYBE_UNUSED PositionComponent& p1
)
{
	// 레이저 크기: 길이 8칸(halfWidth=4칸), 두께 1칸(halfHeight=0.5칸)
	const Scalar halfWidth = static_cast<Scalar>(CELL_SIZE << 2);
	const Scalar halfHeight = static_cast<Scalar>(CELL_SIZE >> 1);
	
	const Scalar dx = (p1.x - p0.x);
	const Scalar dy = (p1.y - p0.y);
	const Angle rotation = MathUtil::ArcTan(dx, dy);
	
	// 중심점 이동: 시전자 위치에서 전방으로 halfWidth(4칸) 만큼 이동
	const Scalar distanceFromCenter = halfWidth;

	const Scalar offsetX = (MathUtil::Cos(rotation) * distanceFromCenter) >> 10;
	const Scalar offsetY = (MathUtil::Sin(rotation) * distanceFromCenter) >> 10;
	const Scalar cx = p0.x + offsetX;
	const Scalar cy = p0.y + offsetY;

	if (SkillComponentLaser* laserComp = TryGetComponent<SkillComponentLaser>(e0.GetEntityIdp()))
	{
		laserComp->renderSkillX = cx;
		laserComp->renderSkillY = cy;
		laserComp->renderSkillW = halfWidth;
		laserComp->renderSkillH = halfHeight;
		laserComp->renderSkillRotation = rotation;
	}
}

void SkillSystemLaser::OnGiveDamage(
	MAYBE_UNUSED Entity& e0,
	MAYBE_UNUSED PositionComponent& p0,
	MAYBE_UNUSED Entity& e1,
	MAYBE_UNUSED PositionComponent& p1,
	MAYBE_UNUSED DamageInfo& damageInfo
)
{
	// 레이저 크기: 길이 8칸(halfWidth=4칸), 두께 1칸(halfHeight=0.5칸)
	const Scalar halfWidth = static_cast<Scalar>(CELL_SIZE << 2);
	const Scalar halfHeight = static_cast<Scalar>(CELL_SIZE >> 1);
	
	const Scalar dx = (p1.x - p0.x);
	const Scalar dy = (p1.y - p0.y);
	const Angle rotation = MathUtil::ArcTan(dx, dy);
	
	// 중심점 이동: 시전자 위치에서 전방으로 halfWidth(4칸) 만큼 이동
	const Scalar distanceFromCenter = halfWidth;

	std::deque<EntityIdp> result;

	WorldManager& worldManager = GetWorldManager();
	worldManager.PickEntityInObb(
		p0.x, p0.y,
		halfWidth,
		halfHeight,
		rotation,
		distanceFromCenter,
		result
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
