// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Tactics/TacticsSystemIdle.h"


TacticsSystemIdle::TacticsSystemIdle(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	TacticsSystemBase<TacticsSystemIdle>(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<CharacterComponent>();
	IncludeComponent<PositionComponent>();
	IncludeComponent<PhysicsComponent>();
	IncludeComponent<DetectionComponentEnemy>();
	IncludeComponent<TacticsComponentIdle>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
	// PC Leader도 TacticsSystemIdle에서 처리 (적 탐지)
}

void TacticsSystemIdle::Update()
{
	if (Data().empty())
		return;

	for (EntityIdp entityIdp : Data())
	{
		PositionComponent& position0 = GetComponent<PositionComponent>(entityIdp.first);
		CharacterComponent& character0 = GetComponent<CharacterComponent>(entityIdp.first);
		PhysicsComponent& physics0 = GetComponent<PhysicsComponent>(entityIdp.first);
		DetectionComponentEnemy& t0 = GetComponent<DetectionComponentEnemy>(entityIdp.first);
		TacticsComponentIdle& state0 = GetComponent<TacticsComponentIdle>(entityIdp.first);
		const TacticsType tacticsType = character0.tacticsType;
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

		// PC Leader는 자동 탐색 안 함 (명시적 입력 기반 처리)
		const bool isPcLeader = HasComponent<PcComponent>(entityIdp) && HasComponent<LeaderComponent>(entityIdp);

		// Offensive 전용 로직: WorldUser 타겟 공유
		if (tacticsType == TacticsType::OFFENSIVE && !isPcLeader)
		{
			WorldUser* worldUser = nullptr;
			if (PcComponent* pc = TryGetComponent<PcComponent>(*entity))
			{
				worldUser = FindWorldUser(pc->worldUserId);
				// WorldUser에 캐싱된 leaderEntityIdp 사용 (최적화)
				if (character0.skillTarget == INVALID_ENTITY_IDP && worldUser &&
					worldUser->leaderEntityIdp != INVALID_ENTITY_IDP && worldUser->leaderEntityIdp != entityIdp)
				{
					if (Entity* leaderEntity = GetEntity(worldUser->leaderEntityIdp))
					{
						if (LeaderComponent* leaderComp = TryGetComponent<LeaderComponent>(*leaderEntity))
							character0.skillTarget = leaderComp->leaderEntityTargetIdp;
					}
				}
			}

			if ((t0.e != INVALID_ENTITY_IDP) &&
				(t0.d < MathUtil::Pow(DebugInfo::TACTICS_COMBAT_RANGE)))
			{
				ChangeTacticsToCombat(*entity);
				continue;
			}
		}

		if (character0.skillTarget != INVALID_ENTITY_IDP)
		{
			ChangeTacticsToCombat(*entity);
			continue;
		}

		// PC Leader는 Roaming 안 함 (Idle 유지)
		if (!isPcLeader && state0.remainedDuration == 0)
		{
			ChangeTacticsToRoaming(*entity, position0, physics0);
			continue;
		}
	}
}
