// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemBazooka.h"

#include "Common/Stat/statContainer/StatContainer.h"
#include "MmoSync/Util/EntityUtil/EntityUtil.h"
#include "MmoSync/Grid/MultiGrid.h"
#include "MmoSync/Component/PhysicsComponent.h"
#include "MmoSync/Component/SkillComponent.h"
#include "MmoSync/Component/PcComponent.h"


SkillSystemBazooka::SkillSystemBazooka(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<SkillComponentBazooka>();
	IncludeComponent<TacticsComponentCombat>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
}
void SkillSystemBazooka::Update()
{
	UpdateNormalSkill<SkillComponentBazooka, MotionComponentSkillB>();
}

void SkillSystemBazooka::OnStartSkillMotion(
	MAYBE_UNUSED Entity& e0,
	MAYBE_UNUSED PositionComponent& p0,
	MAYBE_UNUSED Entity& e1,
	MAYBE_UNUSED PositionComponent& p1
)
{
	// 현재 Bazooka는 발사체를 생성하며, SkillComponentBazooka를 통한 라인 렌더링 코드는 없습니다.
	// [TODO] 라인 이펙트가 필요하면 여기서 SkillComponentBazooka(또는 렌더 시스템)에 필요한 파라미터를 설정한다.
}

void SkillSystemBazooka::OnGiveDamage(
	MAYBE_UNUSED Entity& e0,
	MAYBE_UNUSED PositionComponent& p0,
	MAYBE_UNUSED Entity& e1,
	MAYBE_UNUSED PositionComponent& p1,
	MAYBE_UNUSED DamageInfo& damageInfo
)
{
	damageInfo.stun = true;

	MultiGrid& grid = RefMultiGrid();
	ColRowPair cell0 = grid.PickCell(p0.x, p0.y);
	ColRowPair cell1 = grid.PickCell(p1.x, p1.y);

	Scalar x1 = p1.x;
	Scalar y1 = p1.y;
	if (cell0 == cell1)
	{
		const Scalar dx = (x1 - p0.x);
		const Scalar dy = (y1 - p0.y);
		auto [nx, ny] = MathUtil::GetVec1024(dx, dy);
		x1 = x1 + ((nx * CELL_SIZE) >> 10);
		y1 = y1 + ((ny * CELL_SIZE) >> 10);
	}

	// 새로 생성하고 나면 원래 포인터들이 무효하기 때문에 필요한 변수 미리 구함
	const Scalar x0 = p0.x;
	const Scalar y0 = p0.y;
	WorldUserId worldUserId = INVALID_WORLD_USER_ID;
	if (PcComponent* pc0 = TryGetComponent<PcComponent>(e0))
		worldUserId = pc0->worldUserId;

	EntityDescription entityDescription = EntityUtil::DescriptEntityPcNpc(
		EntityDescriptorArg{
			.x = x0,
			.y = y0,
			.characterSid = 2,
			.characterType = CharacterType::PROJECTILE,
			.worldUserId = worldUserId,
			.spawnerIdp = INVALID_ENTITY_IDP,
			.commandX = 0,
			.commandY = 0
		});
	Entity* projectile = CreateEntityFromDescription(entityDescription);
	if (projectile)
	{
		if (PhysicsComponent* physics = TryGetComponent<PhysicsComponent>(*projectile))
		{
			// 발사체는 REACTIVE 레이어 사용
			physics->SetCommand(false, x0, y0, MoveCommandLayer::REACTIVE, x1, y1, DebugInfo::MOVE_LAYER_REACTIVE_MAX_LIFETIME, DebugInfo::MOVE_LAYER_REACTIVE_MIN_HOLD);
			ChangeTacticsToCombat(*projectile);
		}

		if (SkillComponentProjectile* proj = TryGetComponent<SkillComponentProjectile>(*projectile))
		{
			proj->targetX = x1;
			proj->targetY = y1;
			proj->bombInterval = 60; // arbitrary interval
			proj->elapsedBombTime = 0;
		}
	}
}
