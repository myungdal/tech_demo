// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Tactics/TacticsSystemFlee.h"

#include "Common/Stat/statContainer/StatContainer.h"

#include "Packet/StaticData/Character/CharacterDoc.h"


TacticsSystemFlee::TacticsSystemFlee(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	TacticsSystemBase<TacticsSystemFlee>(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<CharacterComponent>();
	IncludeComponent<PositionComponent>();
	IncludeComponent<PhysicsComponent>();
	IncludeComponent<DetectionComponentEnemy>();
	IncludeComponent<TacticsComponentFlee>();
	//ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
	ExcludeAllComponents<LeaderComponent, PcComponent>();
}

// 도망 상태 ? 적 반대 방향으로 이동하며 회복. 체력이 임계값 이상이면 거점으로 회귀 상태로, 임계값 이상이면 전투 상태로.
void TacticsSystemFlee::Update()
{
	if (Data().empty())
		return;

	for (EntityIdp entityIdp : Data())
	{
		CharacterComponent& character0 = GetComponent<CharacterComponent>(entityIdp.first);
		PositionComponent& position0 = GetComponent<PositionComponent>(entityIdp.first);
		PhysicsComponent& physics0 = GetComponent<PhysicsComponent>(entityIdp.first);
		TacticsComponentFlee& state0 = GetComponent<TacticsComponentFlee>(entityIdp.first);
		Entity* entity = GetEntity(entityIdp);
		if (!entity)
			continue;

#if defined(UE_EDITOR) || defined(_DEBUG)
		if (entityIdp.first == DebugInfo::DEBUG_ENTITY_ID)
			__nop();
#endif

		// PC 팔로워가 리더와 너무 멀면 전술 스킵 (귀환 우선)
		//if (ShouldSkipTacticsDueToDistance(entityIdp))
		//	continue;

		if (state0.remainedDuration > 0)
			--state0.remainedDuration;

		const Entity* skillTarget = GetEntity(character0.skillTarget);

		if (nullptr == skillTarget)
		{
			character0.skillTarget = INVALID_ENTITY_IDP;
			ChangeTacticsToReturn(*entity, position0, physics0);
			continue;
		}

		const PositionComponent* position1 = TryGetComponent<PositionComponent>(*skillTarget);
		if (!position1)
		{
			character0.skillTarget = INVALID_ENTITY_IDP;
			ChangeTacticsToReturn(*entity, position0, physics0);
			continue;
		}

		const Scalar targetDistance = position0.CalcSquaredLength(*position1);
		if (targetDistance > MathUtil::Pow(DebugInfo::TACTICS_COMBAT_RANGE))
		{
			character0.skillTarget = INVALID_ENTITY_IDP;
			ChangeTacticsToReturn(*entity, position0, physics0);
			continue;
		}

		// HP 체크 및 상태 전환
		if (const STATIC_CHARACTER* staticCharacter = character0.GetCharacterDoc().mStaticCharacter)
		{
			if (StatContainer* statContainer0 = character0.GetStatContainer())
			{
				const StatValue currHp = statContainer0->GetStatValue(StatType::HP);
				const StatValue maxHp = statContainer0->GetStatValue(StatType::HP_MAX);

				if (int64_t(maxHp) * int64_t(staticCharacter->ConstRef_c_hp_threshold_last_stand()) > int64_t(currHp) * int64_t(100))
				{
					ChangeTacticsToCombat(*entity);
					continue;
				}
			}
		}

		if (state0.remainedDuration == 0)
		{
			ChangeTacticsToCombat(*entity);
			continue;
		}

		// 적 반대 방향으로 이동
		const Scalar dx = (position0.x - position1->x);
		const Scalar dy = (position0.y - position1->y);

		auto [sx, sy] = MathUtil::GetVec1024(dx, dy);

		sx = (position0.x + ((sx * DebugInfo::MOVE_TO_DISTANCE_BY_KEY) >> 10));
		sy = (position0.y + ((sy * DebugInfo::MOVE_TO_DISTANCE_BY_KEY) >> 10));

	const bool isPcLeader = HasComponent<PcComponent>(entityIdp) && HasComponent<LeaderComponent>(entityIdp);
	physics0.SetCommand(isPcLeader, position0.x, position0.y, MoveCommandLayer::TACTICS, sx, sy, DebugInfo::MOVE_LAYER_TACTICS_MAX_LIFETIME, DebugInfo::MOVE_LAYER_TACTICS_MIN_HOLD);
	ChangeTacticsToCombat(*entity);
	}
}
