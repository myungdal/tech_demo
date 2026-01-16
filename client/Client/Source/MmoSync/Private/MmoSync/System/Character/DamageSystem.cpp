// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Character/DamageSystem.h"

#include "Common/Stat/statContainer/StatContainer.h"

#include "MmoSync/DebugControlTool/DebugInfo.h"


DamageSystem::DamageSystem(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<CharacterComponent>();
	IncludeComponent<DamageComponent>();
	ExcludeComponent<MotionComponentGroupC>();
}
void DamageSystem::Update()
{
	if (Data().empty())
		return;

	for (EntityIdp entityIdp : Data())
	{
#if defined(UE_EDITOR) || defined(_DEBUG)
		if (entityIdp.first == DebugInfo::DEBUG_ENTITY_ID)
			__nop();
#endif

		MAYBE_UNUSED CharacterComponent& character0 = GetComponent<CharacterComponent>(entityIdp.first);
		MAYBE_UNUSED DamageComponent& damage0 = GetComponent<DamageComponent>(entityIdp.first);
		Entity& entity0 = *GetEntity(entityIdp);

		DamageInfoId infoId = damage0.damageInfoId;
		damage0.damageInfoId = INVALID_DAMAGE_INFO_ID;

		// 성능 측정 후 미삭제 하거나 지연 삭제 할 수도...
		DetachComponent<DamageComponent>(entity0);

		if (infoId == INVALID_DAMAGE_INFO_ID)
			continue;

		while (infoId != INVALID_DAMAGE_INFO_ID)
		{
			DamageInfo* info = TryGetDamageInfo(infoId);
			DamageInfoId nextId = INVALID_DAMAGE_INFO_ID;
			if (info)
				nextId = info->next;

			if (!info)
			{
				infoId = nextId;
				continue;
			}

			StatValue currDamage = info->damage;
			if (DebugInfo::DEBUG_DAMAGE >= 0)
				currDamage = DebugInfo::DEBUG_DAMAGE;

			EntityIdp attacker = info->lastAttacker;
			if (HasComponent<AbilityComponentUntargetrable>(attacker))
				attacker = INVALID_ENTITY_IDP;

			// RemoveDamageInfo 전에 필요한 값 저장 (삭제 후 접근 불가)
			const bool isPushBack = info->pushBack;
			const bool isStun = info->stun;

			RemoveDamageInfo(infoId);
			infoId = nextId;

			if (currDamage == 0)
				continue;

			int64_t physicsDamage = DebugInfo::PHYSICS_DAMAGE_NORMAL;

			if (StatContainer* statContainer0 = character0.GetStatContainer())
			{
				const StatValue currHp = statContainer0->GetStatValue(StatType::HP);
				const StatValue decreaseHp = (currHp > currDamage) ? currDamage : currHp;
				statContainer0->DecreaseStatValue(StatType::HP, decreaseHp);
				const StatValue newHp = statContainer0->GetStatValue(StatType::HP);

				if (newHp == 0)
				{
					physicsDamage = DebugInfo::PHYSICS_DAMAGE_DYING;

					// 다잉 모션
					AttachComponent<AbilityComponentUntargetrable>(entity0);
					MotionComponentDying& dyingComp = AttachComponent<MotionComponentDying>(entity0);
					dyingComp.now = 0;
					dyingComp.duration = DebugInfo::MOTION_DURATION_DYING;
					// 죽인 Entity 기록 (PC/NPC 모두 저장). Revive 정책(PvP 등)에 사용 가능.
					dyingComp.killerEntityIdp = attacker;

					// NPC를 죽인 공격자(PC) 정보 저장
					EntityIdp killerIdpToPropagate = INVALID_ENTITY_IDP;
					const bool isDyingLeader = (TryGetComponent<LeaderComponent>(entity0) != nullptr);

					if (TryGetComponent<NpcComponent>(entity0))
					{
						if (Entity* attackerEntity = GetEntity(attacker))
						{
							if (TryGetComponent<PcComponent>(*attackerEntity))
							{
								killerIdpToPropagate = attacker;
							}
						}
					}

					// 죽은 Entity가 리더인지 확인
					const EntityIdp dyingEntityIdp = entity0.GetEntityIdp();

					if (isDyingLeader)
					{
						// PC 리더가 죽은 경우
						if (PcComponent* pc = TryGetComponent<PcComponent>(entity0))
						{
							if (WorldUser* worldUser = FindWorldUser(pc->worldUserId))
							{
								// PC 팔로워들 죽이기
								mEngine.ForEachEntity(
									[&](Entity& entity1)
									{
										if (PcComponent* pc1 = TryGetComponent<PcComponent>(entity1))
										{
											if (pc1->worldUserId == pc->worldUserId)
											{
												if (CharacterComponent* c1 = TryGetComponent<CharacterComponent>(entity1))
												{
													if (StatContainer* statContainer1 = c1->GetStatContainer())
													{
														const StatValue currHp1 = statContainer1->GetStatValue(StatType::HP);
														statContainer1->DecreaseStatValue(StatType::HP, currHp1);
													}
												}

												AttachComponent<AbilityComponentUntargetrable>(entity1);
												MotionComponentDying& followerDying = AttachComponent<MotionComponentDying>(entity1);
												followerDying.now = 0;
												followerDying.duration = DebugInfo::MOTION_DURATION_DYING;
												// PC 팔로워는 killerEntityIdp를 물려받지 않음 (PC는 부활 안 함)
											}
										}
									}
								);
							}
						}
						// NPC 리더가 죽은 경우
						else if (TryGetComponent<NpcComponent>(entity0))
						{
							// NPC 리더가 죽으면 NPC Follower들도 함께 죽임
							mEngine.ForEachEntity(
								[&](Entity& entity1)
								{
									// NPC Follower만 대상
									if (!TryGetComponent<NpcComponent>(entity1))
										return;

									// 이 리더의 Follower인지 확인
									if (FollowerComponent* followerComp = TryGetComponent<FollowerComponent>(entity1))
									{
										if (followerComp->leaderEntityIdp != dyingEntityIdp)
											return;
									}
									else
									{
										return;  // FollowerComponent 없으면 스킵
									}

									// 이미 죽어있거나 죽어가는 중이면 스킵
									if (HasComponent<MotionComponentDying>(entity1.GetEntityIdp()) ||
										HasComponent<MotionComponentDead>(entity1.GetEntityIdp()))
										return;

									// Follower HP를 0으로 만들고 Dying 상태로 전환
									if (CharacterComponent* c1 = TryGetComponent<CharacterComponent>(entity1))
									{
										if (StatContainer* statContainer1 = c1->GetStatContainer())
										{
											const StatValue currHp1 = statContainer1->GetStatValue(StatType::HP);
											statContainer1->DecreaseStatValue(StatType::HP, currHp1);
										}
									}

									AttachComponent<AbilityComponentUntargetrable>(entity1);
									MotionComponentDying& followerDying = AttachComponent<MotionComponentDying>(entity1);
									followerDying.now = 0;
									followerDying.duration = DebugInfo::MOTION_DURATION_DYING;
									// NPC Follower는 리더와 함께 죽으므로 부활 없음 (Destroy 처리됨)
								}
							);
						}
					}
				}
				else
				{
					// 대미지 모션
					{
						MotionComponentDamage& c = AttachComponent<MotionComponentDamage>(entity0);
						if (c.now > 0)
						{
							c.duration += DebugInfo::MOTION_DURATION_DAMAGE;
						}
						else
						{
							c.now = 0;
							c.duration = DebugInfo::MOTION_DURATION_DAMAGE;
						}
					}

					// 스턴
					if (isStun)
					{
						MotionComponentStun& c = AttachComponent<MotionComponentStun>(entity0);
						if (c.now > 0)
						{
							c.duration += DebugInfo::MOTION_DURATION_DAMAGE;
						}
						else
						{
							c.now = 0;
							c.duration = DebugInfo::MOTION_DURATION_DAMAGE;
						}
					}

					// 넉백 (일반 데미지)
					if (isPushBack)
						physicsDamage = DebugInfo::PHYSICS_DAMAGE_PUSH_BACK;

					// 날 때린놈이 타겟
					if (attacker != INVALID_ENTITY_IDP)
					{
						// 기존 skillTarget이 유효한지 확인 (죽었거나 사라졌으면 무효)
						bool hasValidTarget = false;
						if (character0.skillTarget != INVALID_ENTITY_IDP)
						{
							Entity* targetEntity = GetEntity(character0.skillTarget);
							if (targetEntity &&
								!targetEntity->HasComponent(MotionComponentDead::GetComponentType()) &&
								!targetEntity->HasComponent(MotionComponentDying::GetComponentType()))
							{
								hasValidTarget = true;
							}
							else
							{
								// 타겟이 죽었거나 사라졌으면 해제
								character0.skillTarget = INVALID_ENTITY_IDP;
							}
						}

						// 유효한 타겟이 없으면 공격자를 타겟으로 설정
						if (!hasValidTarget)
						{
							PhysicsComponent* physics0 = TryGetComponent<PhysicsComponent>(entity0);
							if (physics0 && !physics0->IsReactive())
							{
								PositionComponent* position0 = TryGetComponent<PositionComponent>(entity0);
								PositionComponent* attackerPos = TryGetComponent<PositionComponent>(attacker);
								if (position0 && attackerPos)
								{
									const bool isPcLeader0 = HasComponent<PcComponent>(entity0.GetEntityIdp()) && HasComponent<LeaderComponent>(entity0.GetEntityIdp());
									character0.skillTarget = info->lastAttacker;
									physics0->SetCommand(isPcLeader0, position0->x, position0->y, MoveCommandLayer::REACTIVE, attackerPos->x, attackerPos->y, DebugInfo::MOVE_LAYER_REACTIVE_MAX_LIFETIME, DebugInfo::MOVE_LAYER_REACTIVE_MIN_HOLD);
									ChangeTacticsToCombat(entity0);
								}
							}
						}
					}
				}
			}

			if (attacker != INVALID_ENTITY_IDP)
			{
				if (physicsDamage > 0)
					if (PhysicsComponent* physics0 = TryGetComponent<PhysicsComponent>(entity0))
						if (PositionComponent* position0 = TryGetComponent<PositionComponent>(entity0))
							if (Entity* attackerEntity = GetEntity(attacker))
								if (PositionComponent* attackerPos = TryGetComponent<PositionComponent>(*attackerEntity))
								{
									// 공격자에서 피격자로 향하는 방향 벡터 계산
									const Scalar dx = position0->x - attackerPos->x;
									const Scalar dy = position0->y - attackerPos->y;

									// 정규화된 방향 벡터 (1024 스케일)
									auto [nx, ny] = MathUtil::GetVec1024(dx, dy);

									// ex, ey에 추가 (friction의 영향을 받지 않고 방향 유지)
									physics0->ex = (physicsDamage * nx) >> 10;
									physics0->ey = (physicsDamage * ny) >> 10;

									// 방향
									character0.clientCharacterState |= CLIENT_CHARACTER_STATE_SMOOTH_RADIAN_RESET;
								}

				// Spawnee의 Spawner에게 타겟 전달
				//SpawneeComponent* spawnee = TryGetComponent<SpawneeComponent>(entity0);
				//NpcComponent* npc = TryGetComponent<NpcComponent>(entity0);
				//if (spawnee && npc)
				//{
				//	if (CharacterComponent* spawnerCharacter = TryGetComponent<CharacterComponent>(npc->spawnerIdp))
				//		spawnerCharacter->skillTarget = info->lastAttacker;
				//}

				// 파트너에게 공격 알림
				DetectionComponentPartner* partnerComponent = TryGetComponent<DetectionComponentPartner>(entity0);
				if (partnerComponent)
				{
					Entity* entity1 = GetEntity(partnerComponent->e);
					if (entity1 && (entity1->HasComponent(TacticsComponentIdle::GetComponentType()) ||
						entity1->HasComponent(TacticsComponentRoaming::GetComponentType())))
					{
						PositionComponent* position0 = TryGetComponent<PositionComponent>(entity0);
						PositionComponent* position1 = TryGetComponent<PositionComponent>(*entity1);
						PhysicsComponent* physics1 = TryGetComponent<PhysicsComponent>(*entity1);
						if (position0 && position1 && physics1 && !physics1->IsReactive())
						{
							const bool isPcLeader1 = HasComponent<PcComponent>(entity1->GetEntityIdp()) && HasComponent<LeaderComponent>(entity1->GetEntityIdp());
							physics1->SetCommand(isPcLeader1, position1->x, position1->y, MoveCommandLayer::REACTIVE, position0->x, position0->y, DebugInfo::MOVE_LAYER_REACTIVE_MAX_LIFETIME, DebugInfo::MOVE_LAYER_REACTIVE_MIN_HOLD);
							ChangeTacticsToCombat(*entity1);
						}
					}
				}
			}
		}
	}
}
