// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Tactics/TacticsSystemCombat.h"

#include "Common/Stat/statContainer/StatContainer.h"

#include "Packet/StaticData/Character/CharacterDoc.h"


TacticsSystemCombat::TacticsSystemCombat(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	TacticsSystemBase<TacticsSystemCombat>(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<CharacterComponent>();
	IncludeComponent<PositionComponent>();
	IncludeComponent<PhysicsComponent>();
	IncludeComponent<DetectionComponentEnemy>();
	IncludeComponent<TacticsComponentCombat>();
	//ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
	// PC Leader도 TacticsSystemCombat에서 처리 (키보드 입력 중에는 자동 이동 건너뜀)
}

// 전투(Combat) 상태 업데이트
// - 적을 추격하며 최적 스킬 사용 거리를 유지
// - 체력이 임계값 이하가 되면 도망(Flee) 상태로 전환
// - 적이 없거나 전투 범위 밖으로 나가면 회귀(Return) 상태로 전환
// - Leader는 타겟이 없으면 즉시 대기(Idle) 상태로 전환
void TacticsSystemCombat::Update()
{
	if (Data().empty())
		return;

	// 전투 범위 설정 (디버그 설정값)
	const int64_t combatRangeSq = MathUtil::Pow(DebugInfo::TACTICS_COMBAT_RANGE);
	WorldUserPool& worldUserPool = GetWorldUserPool();

	for (EntityIdp entityIdp : Data())
	{
		// 필수 컴포넌트 획득
		CharacterComponent& c0 = GetComponent<CharacterComponent>(entityIdp.first);
		PositionComponent& position = GetComponent<PositionComponent>(entityIdp.first);
		PhysicsComponent& physics = GetComponent<PhysicsComponent>(entityIdp.first);
		DetectionComponentEnemy& t0 = GetComponent<DetectionComponentEnemy>(entityIdp.first);
		TacticsComponentCombat& state0 = GetComponent<TacticsComponentCombat>(entityIdp.first);
		const TacticsType tacticsType = c0.tacticsType;
		const bool isPcLeader = HasComponent<PcComponent>(entityIdp) && HasComponent<LeaderComponent>(entityIdp);
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

		// 전투 지속 시간 감소 (도망 전환 판정용)
		if (state0.remainedDuration > 0)
			--state0.remainedDuration;

		// 캐릭터 정적 데이터 및 스탯 컨테이너 검증
		const STATIC_CHARACTER* staticCharacter = c0.GetCharacterDoc().mStaticCharacter;
		if (!staticCharacter)
			continue;

		StatContainer* statContainer0 = c0.GetStatContainer();
		if (!statContainer0)
			continue;

		// ==================== 1. 스킬 타겟 검증 ====================
		// 타겟이 타겟팅 불가 상태(죽음, 다잉, Untargetable)인지 확인
		const Entity* skillTarget = GetEntity(c0.skillTarget);
		if (skillTarget && (skillTarget->HasComponent(AbilityComponentUntargetrable::GetComponentType()) ||
			skillTarget->HasComponent(MotionComponentDying::GetComponentType()) ||
			skillTarget->HasComponent(MotionComponentDead::GetComponentType())))
		{
			skillTarget = nullptr;
		}

		// ==================== 2. 타겟이 없는 경우: 새 타겟 탐색 ====================
		if (nullptr == skillTarget)
		{
			physics.SetArrived(MoveCommandLayer::TACTICS); // 이동 상태 초기화
			c0.skillTarget = INVALID_ENTITY_IDP;

			EntityIdp leaderEntityTargetIdp = INVALID_ENTITY_IDP;

			// PC Follower인 경우: Leader의 타겟을 공유 (단, 타겟이 유효한 경우에만)
			if (PcComponent* pc = TryGetComponent<PcComponent>(entityIdp))
			{
				WorldUser& worldUser = worldUserPool.GetData(pc->worldUserId);

				if (worldUser.leaderEntityIdp != INVALID_ENTITY_IDP)
					if (Entity* leaderEntity = GetEntity(worldUser.leaderEntityIdp))
						if (LeaderComponent* leaderComp = TryGetComponent<LeaderComponent>(*leaderEntity))
							if (leaderComp->leaderEntityTargetIdp != INVALID_ENTITY_IDP)
								if (const Entity* targetEntity = GetEntity(leaderComp->leaderEntityTargetIdp))
								{
									// Leader 타겟이 유효한지 검증 (죽음/다잉/타겟불가 상태 제외)
									if (!targetEntity->HasComponent(AbilityComponentUntargetrable::GetComponentType()) &&
										!targetEntity->HasComponent(MotionComponentDying::GetComponentType()) &&
										!targetEntity->HasComponent(MotionComponentDead::GetComponentType()))
									{
										leaderEntityTargetIdp = leaderComp->leaderEntityTargetIdp;
									}
								}
			}

			// 타겟 선정 우선순위:
			// 1. Leader인 경우 → leaderEntityTargetIdp 초기화 + Idle 전환
			// 2. Leader의 타겟이 있는 경우 → Leader 타겟 공유
			// 3. 탐지된 적이 전투 범위 내에 있는 경우 → 탐지된 적 공격
			// 4. 위 모두 해당 없음 → Return 전환 (원래 위치로 복귀)
			if (isPcLeader)
			{
				// Leader의 타겟이 죽었으므로 초기화 (Follower들이 각자 탐지된 적 공격하도록)
				if (LeaderComponent* leaderComp = TryGetComponent<LeaderComponent>(entityIdp))
					leaderComp->leaderEntityTargetIdp = INVALID_ENTITY_IDP;

				ChangeTacticsToIdle(*entity);
				c0.skillTarget = INVALID_ENTITY_IDP;
				continue;
			}
			else if (leaderEntityTargetIdp != INVALID_ENTITY_IDP)
			{
				c0.skillTarget = leaderEntityTargetIdp;
			}
			else if ((t0.e != INVALID_ENTITY_IDP) && (t0.d < combatRangeSq))
			{
				// 탐지된 적이 유효한지 검증 (죽음/다잉/타겟불가 상태 제외)
				const Entity* detectedEnemy = GetEntity(t0.e);
				if (detectedEnemy &&
					!detectedEnemy->HasComponent(AbilityComponentUntargetrable::GetComponentType()) &&
					!detectedEnemy->HasComponent(MotionComponentDying::GetComponentType()) &&
					!detectedEnemy->HasComponent(MotionComponentDead::GetComponentType()))
				{
					c0.skillTarget = t0.e;
				}
				else
				{
					// 탐지된 적이 유효하지 않으면 Return 상태로 전환
					// [FIX] PC Follower가 Dash 중이면 Return 전환 스킵 (TacticsSystemReturn에서 PC 제외되므로)
					if (HasComponent<PcComponent>(entityIdp) && entity->HasComponent(MotionComponentDash::GetComponentType()))
						continue;
					ChangeTacticsToReturn(*entity, position, physics);
					c0.skillTarget = INVALID_ENTITY_IDP;
					continue;
				}
			}
			else if (!physics.IsReactive())
			{
				// [FIX] PC Follower가 Dash 중이면 Return 전환 스킵 (TacticsSystemReturn에서 PC 제외되므로)
				if (HasComponent<PcComponent>(entityIdp) && entity->HasComponent(MotionComponentDash::GetComponentType()))
					continue;
				ChangeTacticsToReturn(*entity, position, physics);
				c0.skillTarget = INVALID_ENTITY_IDP;
				continue;
			}

			continue;
		}

		// ==================== 3. 넉백/피격 이동 중이면 스킵 ====================
		if (physics.IsReactive())
			continue;

		// 키보드 입력 중에도 스킬 타겟은 유지하되, 자동 이동만 건너뜀
		const bool isKeyDown = physics.IsKeyDown();

		// ==================== 4. 타겟 위치 검증 ====================
		const PositionComponent* p1 = TryGetComponent<PositionComponent>(*skillTarget);
		if (!p1)
		{
			c0.skillTarget = INVALID_ENTITY_IDP;
			ChangeTacticsToReturn(*entity, position, physics);
			c0.skillTarget = INVALID_ENTITY_IDP;
			continue;
		}

		// ==================== 5. 전투 범위 체크 ====================
		// 타겟이 전투 범위를 벗어난 경우
		bool isExplicitTarget = false;
		if (isPcLeader)
		{
			if (const LeaderComponent* leaderComp = TryGetComponent<LeaderComponent>(entityIdp))
			{
				if (leaderComp->leaderEntityTargetIdp == c0.skillTarget)
					isExplicitTarget = true;
			}
		}

		const Scalar targetDistanceSq = position.CalcSquaredLength(*p1);

		const Scalar minSkillDistance = staticCharacter->Get_c_skill_distance_best_min() << CELL_SIZE_DESIGN_FACTOR;
		const Scalar maxSkillDistance = staticCharacter->Get_c_skill_distance_best_max() << CELL_SIZE_DESIGN_FACTOR;

		if (!isExplicitTarget)
		{
			// 리더의 명시적 타겟이 아닌경우 거리계산을 해본다
			if (targetDistanceSq > combatRangeSq)
			{
				physics.SetArrived(MoveCommandLayer::TACTICS);
				c0.skillTarget = INVALID_ENTITY_IDP;

				// Leader가 아닌 경우만 Return 전환 (Leader의 타겟만 예외)
				if (!isPcLeader)
					ChangeTacticsToReturn(*entity, position, physics);

				c0.skillTarget = INVALID_ENTITY_IDP;
				continue;
			}

			// 내가 PC의 팔로워고, 스킬 타겟과 리더의 거리가 너무 먼 경우 스킵
			if (FollowerComponent* followerComp = this->TryGetComponent<FollowerComponent>(entityIdp))
			{
				if (PositionComponent* leaderPos = this->TryGetComponent<PositionComponent>(followerComp->leaderEntityIdp))
				{
					const Scalar limitDist = DebugInfo::MOVE_TO_LIMIT_FROM_CETNER_FAR + maxSkillDistance;
					const Scalar targetToLeaderDistSq = leaderPos->CalcSquaredLength(*p1);
					if (targetToLeaderDistSq > MathUtil::Pow(limitDist))
					{
						c0.skillTarget = INVALID_ENTITY_IDP;
						continue;
					}
				}
			}
		}

		// ==================== 6. Follower 전용: 추가 검증 및 도망 판정 ====================
		if (!isPcLeader)
		{
			// [TODO] NPC 전술은 더 정교한 판단이 필요하다(스킬/스탯/성향 기반).
			// [FIX] NPC의 전투 포기 조건(예: physicsCommand 기준 거리)이 Return 전환과 충돌해 Combat <-> Return이 왕복되는 문제가 있다.
			// Return 상태에서도 동일 조건을 재검증하거나, 전투 기준점(앵커)을 별도로 저장해 일관되게 판정해야 한다.
				//if (HasComponent<NpcComponent>(entityIdp))
				//{
				//	const Scalar targetDistance1 = p1->CalcSquaredLength(physics.physicsCommandX, physics.physicsCommandY);
				//	if (targetDistance1 > combatRangeSq)
				//	{
				//		c0.skillTarget = INVALID_ENTITY_IDP;
				//		ChangeTacticsToReturn(*entity, position, physics);
				//		continue;
				//	}
				//}

				// HP 기반 도망 판정
				const StatValue currHp = statContainer0->GetStatValue(StatType::HP);
			const StatValue maxHp = statContainer0->GetStatValue(StatType::HP_MAX);

			// HP가 도망 임계값(flee threshold) 이하인 경우
			if (int64_t(maxHp) * int64_t(staticCharacter->ConstRef_c_hp_threshold_flee()) >= int64_t(currHp) * int64_t(100))
			{
				// HP가 최후 저항 임계값(last stand threshold)보다는 높은 경우
				if (int64_t(maxHp) * int64_t(staticCharacter->ConstRef_c_hp_threshold_last_stand()) <= int64_t(currHp) * int64_t(100))
				{
					// 겁쟁이(AVOIDANT) 성향이거나 전투 제한시간이 다 된 경우 → Flee 전환
					if (tacticsType == TacticsType::AVOIDANT || state0.remainedDuration == 0)
					{
						ChangeTacticsToFlee(*entity);
						continue;
					}
				}
			}
		}

		// ==================== 7. 키보드 입력이 없을 때만 자동 이동 ====================
		if (!isKeyDown)
		{
			// 최적 스킬 사용 거리 유지
			Scalar tx = position.x;
			Scalar ty = position.y;

			// 현재 거리가 최적 거리 범위 내인 경우 → 정지 (이동중이 아닐 때만)
			if (targetDistanceSq > MathUtil::Pow(minSkillDistance) &&
				targetDistanceSq < MathUtil::Pow(maxSkillDistance))
			{
				physics.SetArrived(MoveCommandLayer::TACTICS);
			}
			// 현재 거리가 최적 거리 범위 밖인 경우 → 최적 거리로 이동
			else
			{
				// 최적 거리 중간값 계산
				const int64_t best = (minSkillDistance + maxSkillDistance) >> 1;

				// 타겟에서 나를 향하는 정규화된 방향 벡터 계산
				const Scalar dx = (position.x - p1->x);
				const Scalar dy = (position.y - p1->y);

				auto [sx, sy] = MathUtil::GetVec1024(dx, dy);

				// 타겟 위치에서 최적 거리만큼 떨어진 위치 계산
				tx = p1->x + ((sx * best) >> 10);
				ty = p1->y + ((sy * best) >> 10);

				physics.SetCommand(isPcLeader, position.x, position.y, MoveCommandLayer::TACTICS, tx, ty, DebugInfo::MOVE_LAYER_TACTICS_MAX_LIFETIME, DebugInfo::MOVE_LAYER_TACTICS_MIN_HOLD);
			}
		}
	}
}
