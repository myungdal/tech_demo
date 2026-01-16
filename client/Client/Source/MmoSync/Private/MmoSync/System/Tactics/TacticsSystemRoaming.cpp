// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Tactics/TacticsSystemRoaming.h"


TacticsSystemRoaming::TacticsSystemRoaming(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	TacticsSystemBase<TacticsSystemRoaming>(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<CharacterComponent>();
	IncludeComponent<PositionComponent>();
	IncludeComponent<PhysicsComponent>();
	IncludeComponent<DetectionComponentEnemy>();
	IncludeComponent<TacticsComponentRoaming>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
	ExcludeAllComponents<LeaderComponent, PcComponent>();
}
// 탐색 상태 ? 거점 주변을 탐색. 도착 시 IDLE 상태로, 적을 만나면 전투 상태로 전환.
void TacticsSystemRoaming::Update()
{
	if (Data().empty())
		return;

	for (EntityIdp entityIdp : Data())
	{
		CharacterComponent& c0 = GetComponent<CharacterComponent>(entityIdp.first);
		PhysicsComponent& physics0 = GetComponent<PhysicsComponent>(entityIdp.first);
		DetectionComponentEnemy& t0 = GetComponent<DetectionComponentEnemy>(entityIdp.first);
		TacticsComponentRoaming& state0 = GetComponent<TacticsComponentRoaming>(entityIdp.first);
		const TacticsType tacticsType = c0.tacticsType;
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

		// Offensive 전용 로직: WorldUser 타겟 공유
		if (tacticsType == TacticsType::OFFENSIVE)
		{
			WorldUser* worldUser = nullptr;
			if (PcComponent* pc = TryGetComponent<PcComponent>(*entity))
			{
				worldUser = FindWorldUser(pc->worldUserId);
				// WorldUser에 캐싱된 leaderEntityIdp 사용 (최적화)
				if (c0.skillTarget == INVALID_ENTITY_IDP && worldUser &&
					worldUser->leaderEntityIdp != INVALID_ENTITY_IDP && worldUser->leaderEntityIdp != entityIdp)
				{
					if (Entity* leaderEntity = GetEntity(worldUser->leaderEntityIdp))
					{
						if (LeaderComponent* leaderComp = TryGetComponent<LeaderComponent>(*leaderEntity))
							c0.skillTarget = leaderComp->leaderEntityTargetIdp;
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

		if (c0.skillTarget != INVALID_ENTITY_IDP)
		{
			ChangeTacticsToCombat(*entity);
			continue;
		}

		if ((state0.remainedDuration == 0) || physics0.IsIdle())
		{
			ChangeTacticsToIdle(*entity);
			continue;
		}
	}
}
