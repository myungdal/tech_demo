// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Motion/MotionSystemDead.h"

#include "Packet/StaticData/Character/CharacterDoc.h"
#include "Common/Stat/statContainer/StatContainer.h"

#include "MmoSync/DebugControlTool/DebugInfo.h"
#include "MmoSync/Util/EntityUtil/EntityUtil.h"
#include "MmoSync/Util/CharacterUtil/CharacterUtil.h"
#include "MmoSync/Component/PcComponent.h"
#include "MmoSync/Component/LeaderComponent.h"
#include "MmoSync/Component/FollowerComponent.h"
#include "MmoSync/Component/AbilityComponent.h"
#include "MmoSync/Component/MotionComponent.h"

MotionSystemDead::MotionSystemDead(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	DurationSystemBase<MotionSystemDead, MotionComponentDead>(engine, timeStepUpdateCycle, game)
{
}

// ============================================================================
// Update: DurationSystemBase::Update 확장 (포획 충돌 체크)
// ============================================================================
void MotionSystemDead::Update()
{
	// 기본 Duration 처리 (motion.now 증가, 완료 시 OnMotionCompleted 호출)
	DurationSystemBase<MotionSystemDead, MotionComponentDead>::Update();

	// Dead 상태인 캐릭터들에 대해 PcLeader와의 충돌 체크
	for (EntityIdp entityIdp : Data())
	{
		Entity* entityPtr = GetEntity(entityIdp);
		if (!entityPtr)
			continue;

		Entity& entity = *entityPtr;

		// 아직 capturerIdp가 설정되지 않은 경우에만 체크
		MotionComponentDead* dead = TryGetComponent<MotionComponentDead>(entity);
		if (!dead)
			continue;

		// 이미 포획자가 결정됐으면 스킵
		if (dead->capturerIdp != INVALID_ENTITY_IDP)
			continue;

		// Leader는 포획 대상이 아님 (PC Leader, NPC Leader 모두 해당 없음... 단, NPC Leader는 예외)
		LeaderComponent* leader = TryGetComponent<LeaderComponent>(entity);
		NpcComponent* npc = TryGetComponent<NpcComponent>(entity);
		PcComponent* pc = TryGetComponent<PcComponent>(entity);

		// PC Leader는 포획 대상 아님
		if (pc && leader)
			continue;

		// NPC: Leader 또는 Follower 모두 포획 가능 (조건에 따라)
		if (npc)
		{
			// NPC Follower인 경우: 리더가 살아있으면 포획 불가
			if (!leader) // Follower인 경우
			{
				CharacterComponent* character = TryGetComponent<CharacterComponent>(entity);
				if (character && character->originalNpcLeaderIdp != INVALID_ENTITY_IDP)
				{
					Entity* originalLeader = GetEntity(character->originalNpcLeaderIdp);
					// 리더가 존재하고 살아있음 (Dead/Dying 상태가 아님) → 포획 불가
					if (originalLeader &&
						!originalLeader->HasComponent(MotionComponentDead::GetComponentType()) &&
						!originalLeader->HasComponent(MotionComponentDying::GetComponentType()))
					{
						continue;
					}
				}
			}
			// NPC Leader인 경우: 항상 포획 가능 (Dead 상태이므로)
			CheckCaptureCollision(entity, *dead);
			continue;
		}

		// PC Follower: 항상 포획 가능 (다른 PC Leader에게)
		if (pc && !leader)
		{
			CheckCaptureCollision(entity, *dead);
			continue;
		}
	}
}

// ============================================================================
// CheckCaptureCollision: Dead 상태 캐릭터와 PcLeader 충돌 체크
// ============================================================================
void MotionSystemDead::CheckCaptureCollision(Entity& deadEntity, MotionComponentDead& dead)
{
	PositionComponent* deadPos = TryGetComponent<PositionComponent>(deadEntity);
	PhysicsComponent* deadPhysics = TryGetComponent<PhysicsComponent>(deadEntity);
	if (!deadPos)
		return;

	const Scalar deadX = deadPos->x;
	const Scalar deadY = deadPos->y;
	const Scalar deadRadius = deadPhysics ? deadPhysics->radius : 0;

	// 현재 리더 확인 (자신의 현재 리더에게는 다시 포획되지 않음)
	FollowerComponent* deadFollower = TryGetComponent<FollowerComponent>(deadEntity);
	const EntityIdp currentLeaderIdp = deadFollower ? deadFollower->leaderEntityIdp : INVALID_ENTITY_IDP;

	// 모든 엔티티 순회하여 PcLeader 찾기
	mEngine.ForEachEntity([&](Entity& otherEntity)
	{
		// 이미 포획자가 결정됐으면 중단
		if (dead.capturerIdp != INVALID_ENTITY_IDP)
			return;

		// 자기 자신 스킵
		if (otherEntity.GetEntityIdp() == deadEntity.GetEntityIdp())
			return;

		// 현재 리더 스킵 (자신의 리더에게 다시 포획되지 않음)
		if (currentLeaderIdp != INVALID_ENTITY_IDP && otherEntity.GetEntityIdp() == currentLeaderIdp)
			return;

		// PcLeader인지 확인
		PcComponent* pc = TryGetComponent<PcComponent>(otherEntity);
		if (!pc)
			return;

		LeaderComponent* leader = TryGetComponent<LeaderComponent>(otherEntity);
		if (!leader)
			return;

		// Dead/Dying 상태면 스킵
		if (otherEntity.HasComponent(MotionComponentDead::GetComponentType()) ||
			otherEntity.HasComponent(MotionComponentDying::GetComponentType()))
			return;

		// 위치 확인
		PositionComponent* otherPos = TryGetComponent<PositionComponent>(otherEntity);
		PhysicsComponent* otherPhysics = TryGetComponent<PhysicsComponent>(otherEntity);
		if (!otherPos)
			return;

		const Scalar otherX = otherPos->x;
		const Scalar otherY = otherPos->y;
		const Scalar otherRadius = otherPhysics ? otherPhysics->radius : 0;

		// 충돌 거리 계산 (두 반지름의 합)
		const Scalar collisionDistance = deadRadius + otherRadius;
		const Scalar collisionDistanceSq = MathUtil::Pow(collisionDistance);

		// 거리 계산
		const Scalar distX = deadX - otherX;
		const Scalar distY = deadY - otherY;
		const Scalar distSq = MathUtil::DistanceSq(distX, distY);

		// 충돌 판정
		if (distSq <= collisionDistanceSq)
		{
			// 포획! capturerIdp 설정 + 즉시 부활 (Dead 모션 즉시 완료)
			dead.capturerIdp = otherEntity.GetEntityIdp();
			dead.now = dead.duration;  // 다음 Update에서 OnMotionCompleted 호출됨
		}
	});
}

// ============================================================================
// OnMotionCompleted: 메인 진입점
// ============================================================================
bool MotionSystemDead::OnMotionCompleted(Entity& entity, CharacterComponent& character)
{
	if (entity.IsDestroyReserved())
		return true;

#if defined(UE_EDITOR) || defined(_DEBUG)
	if (entity.GetEntityIdp().first == DebugInfo::DEBUG_ENTITY_ID)
		__nop();
#endif

	// 유형 판별
	const ReviveType reviveType = DetermineReviveType(entity, character);

	// 유형별 처리
	switch (reviveType)
	{
	// PC Leader
	case ReviveType::PcLeaderDestroy:
		return RevivePcLeaderDestroy(entity, character);

	// PC Follower
	case ReviveType::PcFollowerToPc:
		return RevivePcFollowerToPc(entity, character);
	case ReviveType::PcFollowerRepeatDead:
		// Dead 모션 반복 (포획 대기)
		if (MotionComponentDead* dead = TryGetComponent<MotionComponentDead>(entity))
		{
			dead->now = 0;
			dead->duration = GetRandom(DebugInfo::MOTION_DURATION_DEAD_MIN, DebugInfo::MOTION_DURATION_DEAD_MAX);
		}
		return false;  // false 반환으로 모션 지속

	// NPC Follower
	case ReviveType::NpcFollowerToPc:
		return ReviveNpcFollowerToPc(entity, character);
	case ReviveType::NpcFollowerToNpc:
		return ReviveNpcFollowerToNpc(entity, character);
	case ReviveType::NpcFollowerRepeatDead:
		// Dead 모션 반복 (리더 죽어있고 포획자 없음 - 포획 대기 중)
		if (MotionComponentDead* dead = TryGetComponent<MotionComponentDead>(entity))
		{
			dead->now = 0;
			dead->duration = GetRandom(DebugInfo::MOTION_DURATION_DEAD_MIN, DebugInfo::MOTION_DURATION_DEAD_MAX);
		}
		return false;  // false 반환으로 모션 지속

	// NPC Leader
	case ReviveType::NpcLeaderToPc:
		return ReviveNpcLeaderToPc(entity, character);
	case ReviveType::NpcLeaderRepeatDead:
		// Dead 모션 반복 (포획 대기)
		if (MotionComponentDead* dead = TryGetComponent<MotionComponentDead>(entity))
		{
			dead->now = 0;
			dead->duration = GetRandom(DebugInfo::MOTION_DURATION_DEAD_MIN, DebugInfo::MOTION_DURATION_DEAD_MAX);
		}
		return false;  // false 반환으로 모션 지속

	// Legacy (미사용)
	case ReviveType::PcFollowerToNpc:
		return RevivePcFollowerToNpc(entity, character);
	case ReviveType::PcFollowerLeaderToNpcLeader:
		return RevivePcFollowerLeaderToNpcLeader(entity, character);
	case ReviveType::NpcLeaderToNpc:
		return ReviveNpcLeaderToNpc(entity, character);

	// Spawner
	case ReviveType::PcSpawnerToPc:
		return RevivePcSpawnerToPc(entity, character);
	case ReviveType::PcSpawnerToNpc:
		return RevivePcSpawnerToNpc(entity, character);
	case ReviveType::NpcSpawnerToPc:
		return ReviveNpcSpawnerToPc(entity, character);
	case ReviveType::NpcSpawnerToNpc:
		return ReviveNpcSpawnerToNpc(entity, character);

	case ReviveType::None:
	default:
		// 분류되지 않은 엔티티 (Rock 등)는 부활 없이 Destroy
		entity.ReserveDestroy();
		break;
	}

	return true;
}

// ============================================================================
// DetermineReviveType: 부활 유형 판별
// ============================================================================
// 
// [ReviveType 매핑 규칙 테이블 - 단순화됨]
// ┌─────────────────────────┬─────────────────────────┬──────────────────────────────┐
// │ Entity Type             │ Condition               │ ReviveType                   │
// ├─────────────────────────┼─────────────────────────┼──────────────────────────────┤
// │ Spawner (PC)            │ killedByPc              │ PcSpawnerToPc                │
// │ Spawner (PC)            │ killedByNpc             │ PcSpawnerToNpc               │
// │ Spawner (NPC)           │ killedByPc              │ NpcSpawnerToPc               │
// │ Spawner (NPC)           │ killedByNpc             │ NpcSpawnerToNpc              │
// │ Spawnee                 │ *                       │ None (파괴)                  │
// │ PC Leader               │ *                       │ PcLeaderDestroy              │
// │ PC Follower             │ 포획자 있음             │ PcFollowerToPc               │
// │ PC Follower             │ 포획자 없음             │ PcFollowerRepeatDead         │
// │ NPC Leader              │ 포획자 있음             │ NpcLeaderToPc                │
// │ NPC Leader              │ 포획자 없음             │ NpcLeaderRepeatDead          │
// │ NPC Follower            │ Leader 살아있음         │ NpcFollowerToNpc             │
// │ NPC Follower            │ Leader 죽음/없음+포획有 │ NpcFollowerToPc              │
// │ NPC Follower            │ Leader 죽음/없음+포획無 │ NpcFollowerRepeatDead        │
// └─────────────────────────┴─────────────────────────┴──────────────────────────────┘
//
MotionSystemDead::ReviveType MotionSystemDead::DetermineReviveType(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	// DeathContext 빌드
	EntityIdp killerEntityIdp = INVALID_ENTITY_IDP;
	EntityIdp capturerIdp = INVALID_ENTITY_IDP;
	if (MotionComponentDead* dead = TryGetComponent<MotionComponentDead>(entity))
	{
		killerEntityIdp = dead->killerEntityIdp;
		capturerIdp = dead->capturerIdp;
	}

	const DeathContext ctx = DeathContext::Build(mEngine, entity, killerEntityIdp);

	// [1] Spawner 부활 (REVIVE_SPAWNER 플래그 필요)
	if (DebugInfo::REVIVE_SPAWNER && ctx.isSpawner)
	{
		if (ctx.isPc)
			return ctx.killedByPc ? ReviveType::PcSpawnerToPc : ReviveType::PcSpawnerToNpc;
		else
			return ctx.killedByPc ? ReviveType::NpcSpawnerToPc : ReviveType::NpcSpawnerToNpc;
	}

	// [2] Spawnee → 파괴 (부활 없음)
	if (ctx.isSpawnee)
		return ReviveType::None;

	// [3] PC Leader → Destroy
	if (ctx.isPcLeader)
		return ReviveType::PcLeaderDestroy;

	// [4] NPC 관련 부활 (REVIVE_NPC 플래그 필요)
	if (DebugInfo::REVIVE_NPC)
	{
		// PC Follower: 포획자 있으면 해당 PC로, 없으면 Dead 반복 (포획 대기)
		if (ctx.isPcFollower)
		{
			const bool hasCapturer = (capturerIdp != INVALID_ENTITY_IDP);
			return hasCapturer ? ReviveType::PcFollowerToPc : ReviveType::PcFollowerRepeatDead;
		}
		
		// NPC Leader: 포획자 있으면 PC Follower, 없으면 Dead 반복
		if (ctx.isNpcLeader)
		{
			const bool hasCapturer = (capturerIdp != INVALID_ENTITY_IDP);
			return hasCapturer ? ReviveType::NpcLeaderToPc : ReviveType::NpcLeaderRepeatDead;
		}
		
		// NPC Follower: 원래 리더가 살아있으면 부활, 아니면 포획 대기
		if (ctx.isNpcFollower)
		{
			// 원래 NPC 리더 Entity 확인
			Entity* originalLeader = GetEntity(character.originalNpcLeaderIdp);
			
			// 리더가 존재하고 살아있으면 즉시 부활
			const bool leaderAlive = (originalLeader != nullptr) &&
				!originalLeader->HasComponent(MotionComponentDead::GetComponentType()) &&
				!originalLeader->HasComponent(MotionComponentDying::GetComponentType());
			
			if (leaderAlive)
			{
				return ReviveType::NpcFollowerToNpc;
			}
			
			// 리더가 죽어있거나 없음 → 포획 가능 상태
			const bool hasCapturer = (capturerIdp != INVALID_ENTITY_IDP);
			return hasCapturer ? ReviveType::NpcFollowerToPc : ReviveType::NpcFollowerRepeatDead;
		}
	}

	return ReviveType::None;
}

// ============================================================================
// RevivePcLeaderDestroy: PC Leader → Destroy
// ============================================================================
bool MotionSystemDead::RevivePcLeaderDestroy(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	if (PcComponent* pc = TryGetComponent<PcComponent>(entity))
		if (WorldUser* worldUser = FindWorldUser(pc->worldUserId))
			worldUser->leaderEntityIdp = INVALID_ENTITY_IDP;

	entity.ReserveDestroy();
	return true;
}

// ============================================================================
// RevivePcFollowerToPc: PC Follower → 다른 PC Follower (포획)
// Dead 중 다른 PC Leader와 충돌하여 소유권 이전
// ============================================================================
bool MotionSystemDead::RevivePcFollowerToPc(Entity& entity, CharacterComponent& character)
{
	PositionComponent* position = TryGetComponent<PositionComponent>(entity);
	MotionComponentDead* dead = TryGetComponent<MotionComponentDead>(entity);

	if (!position || !dead)
	{
		entity.ReserveDestroy();
		return true;
	}

	// Capturer(Dead 중 충돌한 PcLeader)의 WorldUserId와 LeaderIdp 결정
	WorldUserId newWorldUserId = INVALID_WORLD_USER_ID;
	EntityIdp newLeaderIdp = INVALID_ENTITY_IDP;

	// capturerIdp 사용 (Dead 기간 중 충돌한 PcLeader)
	if (Entity* capturerEntity = GetEntity(dead->capturerIdp))
	{
		if (PcComponent* capturerPc = TryGetComponent<PcComponent>(*capturerEntity))
		{
			newWorldUserId = capturerPc->worldUserId;
			if (WorldUser* worldUser = FindWorldUser(capturerPc->worldUserId))
				newLeaderIdp = worldUser->leaderEntityIdp;
		}
	}

	// PC Follower → 다른 PC Follower: 현재 위치에서 부활, 새 PC 리더에 귀속
	return ReviveFollowerCommon(entity, character, FollowerReviveParams{
		.spawnX = position->x,
		.spawnY = position->y,
		.newWorldUserId = newWorldUserId,
		.newLeaderIdp = newLeaderIdp,
		.resolveNpcLeader = false
	});
}

// ============================================================================
// RevivePcFollowerToNpc: PC Follower → NPC Follower (복귀) [Legacy - 미사용]
// ============================================================================
bool MotionSystemDead::RevivePcFollowerToNpc(Entity& entity, CharacterComponent& character)
{
	// 원래 NPC Leader가 없으면 부활하지 않고 파괴
	if (character.originalNpcLeaderIdp == INVALID_ENTITY_IDP)
	{
		CleanupLeaderFollowerRelations(entity);
		entity.ReserveDestroy();
		return true;
	}

	// PC Follower → NPC Follower: 원래 스폰 위치로 복귀, NPC 리더 자동 결정
	return ReviveFollowerCommon(entity, character, FollowerReviveParams{
		.spawnX = character.originalSpawnX,
		.spawnY = character.originalSpawnY,
		.newWorldUserId = INVALID_WORLD_USER_ID,
		.newLeaderIdp = INVALID_ENTITY_IDP,
		.resolveNpcLeader = true
	});
}

// ============================================================================
// RevivePcFollowerLeaderToNpcLeader: PC Follower (원래 NPC Leader) → NPC Leader 복귀
// 원래 위치에 NPC Leader로 부활하고 새 Follower들 생성
// ============================================================================
bool MotionSystemDead::RevivePcFollowerLeaderToNpcLeader(Entity& entity, CharacterComponent& character)
{
	PositionComponent* position = TryGetComponent<PositionComponent>(entity);
	const STATIC_CHARACTER* staticCharacter = character.GetCharacterDoc().mStaticCharacter;

	if (!position || !staticCharacter)
	{
		entity.ReserveDestroy();
		return true;
	}

	const Scalar originalSpawnX = character.originalSpawnX;
	const Scalar originalSpawnY = character.originalSpawnY;
	EntityIdp originalSpawnerIdp = character.originalSpawnerIdp;
	if (originalSpawnerIdp == INVALID_ENTITY_IDP)
	{
		if (NpcComponent* npc = TryGetComponent<NpcComponent>(entity))
			originalSpawnerIdp = npc->spawnerIdp;
	}

	// 기존 관계 정리 및 파괴
	CleanupLeaderFollowerRelations(entity);
	entity.ReserveDestroy();

	// 원래 위치에 NPC Leader로 새 Entity 생성
	EntityDescription entityDescription = EntityUtil::DescriptEntityPcNpc(
		EntityDescriptorArg{
			.x = originalSpawnX,
			.y = originalSpawnY,
			.characterSid = staticCharacter->Get_c_character_item_sid(),
			.characterType = staticCharacter->Get_c_character_type(),
			.worldUserId = INVALID_WORLD_USER_ID,
			.spawnerIdp = originalSpawnerIdp,
			.commandX = 0,
			.commandY = 0
		});

	Entity* newEntity = CreateEntityFromDescription(entityDescription);
	if (!newEntity)
		return true;

	// LeaderComponent 부착
	LeaderComponent& leaderComp = AttachComponent<LeaderComponent>(*newEntity);
	leaderComp.followerCnt = 0;
	leaderComp.leaderEntityTargetIdp = INVALID_ENTITY_IDP;
	leaderComp.worldUserCommandKeyState = 0;

	// 원래 NPC 정보 설정
	if (CharacterComponent* newChar = TryGetComponent<CharacterComponent>(*newEntity))
	{
		newChar->originalSpawnX = originalSpawnX;
		newChar->originalSpawnY = originalSpawnY;
		newChar->originalSpawnerIdp = originalSpawnerIdp;
		newChar->wasNpcLeader = false;  // NPC Leader로 돌아왔으므로 리셋
		newChar->clientCharacterState |= CLIENT_CHARACTER_STATE_SMOOTH_RESET;
		CharacterUtil::FillHpToMax(*newChar);
	}

	return true;
}

// ============================================================================
// ReviveNpcFollowerToPc: NPC Follower → PC Follower (포획)
// Dead 기간 중 충돌한 PcLeader (capturerIdp)에게 귀속
// ============================================================================
bool MotionSystemDead::ReviveNpcFollowerToPc(Entity& entity, CharacterComponent& character)
{
	PositionComponent* position = TryGetComponent<PositionComponent>(entity);
	MotionComponentDead* dead = TryGetComponent<MotionComponentDead>(entity);

	if (!position || !dead)
	{
		entity.ReserveDestroy();
		return true;
	}

	// Capturer(Dead 중 충돌한 PcLeader)의 WorldUserId와 LeaderIdp 결정
	WorldUserId newWorldUserId = INVALID_WORLD_USER_ID;
	EntityIdp newLeaderIdp = INVALID_ENTITY_IDP;

	// capturerIdp 사용 (Dead 기간 중 충돌한 PcLeader)
	if (Entity* capturerEntity = GetEntity(dead->capturerIdp))
	{
		if (PcComponent* capturerPc = TryGetComponent<PcComponent>(*capturerEntity))
		{
			newWorldUserId = capturerPc->worldUserId;
			if (WorldUser* worldUser = FindWorldUser(capturerPc->worldUserId))
				newLeaderIdp = worldUser->leaderEntityIdp;
		}
	}

	// NPC Follower → PC Follower: 현재 위치에서 부활, PC 리더에 귀속
	return ReviveFollowerCommon(entity, character, FollowerReviveParams{
		.spawnX = position->x,
		.spawnY = position->y,
		.newWorldUserId = newWorldUserId,
		.newLeaderIdp = newLeaderIdp,
		.resolveNpcLeader = false
	});
}

// ============================================================================
// ReviveNpcFollowerToNpc: NPC Follower → NPC Follower (그대로)
// ============================================================================
bool MotionSystemDead::ReviveNpcFollowerToNpc(Entity& entity, CharacterComponent& character)
{
	// NPC Follower → NPC Follower: 원래 스폰 위치로 복귀, NPC 리더 자동 결정
	return ReviveFollowerCommon(entity, character, FollowerReviveParams{
		.spawnX = character.originalSpawnX,
		.spawnY = character.originalSpawnY,
		.newWorldUserId = INVALID_WORLD_USER_ID,
		.newLeaderIdp = INVALID_ENTITY_IDP,
		.resolveNpcLeader = true
	});
}

// ============================================================================
// ReviveNpcLeaderToPc: NPC Leader → PC Follower (포획)
// PC에게 죽은 NPC Leader가 PC Follower로 전환 (원래 위치 정보 보존)
// ============================================================================
bool MotionSystemDead::ReviveNpcLeaderToPc(Entity& entity, CharacterComponent& character)
{
	PositionComponent* position = TryGetComponent<PositionComponent>(entity);
	MotionComponentDead* dead = TryGetComponent<MotionComponentDead>(entity);
	const STATIC_CHARACTER* staticCharacter = character.GetCharacterDoc().mStaticCharacter;

	if (!position || !dead || !staticCharacter)
	{
		entity.ReserveDestroy();
		return true;
	}

	// Killer의 WorldUserId와 LeaderIdp 결정
	WorldUserId newWorldUserId = INVALID_WORLD_USER_ID;
	EntityIdp newLeaderIdp = INVALID_ENTITY_IDP;

	if (Entity* killerEntity = GetEntity(dead->killerEntityIdp))
	{
		if (PcComponent* killerPc = TryGetComponent<PcComponent>(*killerEntity))
		{
			newWorldUserId = killerPc->worldUserId;
			if (WorldUser* worldUser = FindWorldUser(killerPc->worldUserId))
				newLeaderIdp = worldUser->leaderEntityIdp;
		}
	}

	// 유효한 PC 정보가 없으면 NPC로 제자리 부활
	if (newWorldUserId == INVALID_WORLD_USER_ID)
	{
		return ReviveNpcLeaderToNpc(entity, character);
	}

	// 최대 NPC Follower 수 제한 체크
	int32_t currentNpcFollowerCount = 0;
	mEngine.ForEachEntity([&](Entity& otherEntity)
	{
		PcComponent* pc = TryGetComponent<PcComponent>(otherEntity);
		if (!pc || pc->worldUserId != newWorldUserId)
			return;

		// Leader가 아닌 Follower만 카운트
		if (TryGetComponent<LeaderComponent>(otherEntity))
			return;

		// 죽어가는 중이거나 죽은 상태는 제외
		if (otherEntity.HasComponent(MotionComponentDead::GetComponentType()) ||
			otherEntity.HasComponent(MotionComponentDying::GetComponentType()))
			return;

		// 원래 NPC였던 캐릭터만 카운트
		CharacterComponent* otherChar = TryGetComponent<CharacterComponent>(otherEntity);
		if (otherChar && (otherChar->wasNpcLeader || otherChar->originalNpcLeaderIdp != INVALID_ENTITY_IDP))
		{
			++currentNpcFollowerCount;
		}
	});

	// 최대치에 도달했으면 포획하지 않고 NPC로 제자리 부활
	if (currentNpcFollowerCount >= DebugInfo::MAX_NPC_FOLLOWER)
	{
		return ReviveNpcLeaderToNpc(entity, character);
	}

	const Scalar originalSpawnX = character.originalSpawnX;
	const Scalar originalSpawnY = character.originalSpawnY;
	EntityIdp originalSpawnerIdp = character.originalSpawnerIdp;
	if (originalSpawnerIdp == INVALID_ENTITY_IDP)
	{
		if (NpcComponent* npc = TryGetComponent<NpcComponent>(entity))
			originalSpawnerIdp = npc->spawnerIdp;
	}

	// 기존 관계 정리 및 파괴
	CleanupLeaderFollowerRelations(entity);
	entity.ReserveDestroy();

	// 현재 위치에 PC Follower로 새 Entity 생성
	EntityDescription entityDescription = EntityUtil::DescriptEntityPcNpc(
		EntityDescriptorArg{
			.x = position->x,
			.y = position->y,
			.characterSid = staticCharacter->Get_c_character_item_sid(),
			.characterType = staticCharacter->Get_c_character_type(),
			.worldUserId = newWorldUserId,
			.spawnerIdp = originalSpawnerIdp,
			.commandX = 0,
			.commandY = 0
		});

	Entity* newEntity = CreateEntityFromDescription(entityDescription);
	if (!newEntity)
		return true;

	// 원래 NPC 정보 전파 (부활 시 NPC Leader로 돌아가기 위해)
	PropagateOriginalNpcInfo(newEntity, originalSpawnX, originalSpawnY, INVALID_ENTITY_IDP, originalSpawnerIdp);

	// wasNpcLeader 플래그 설정 (죽으면 NPC Leader로 부활하도록)
	if (CharacterComponent* newChar = TryGetComponent<CharacterComponent>(*newEntity))
	{
		newChar->wasNpcLeader = true;
		newChar->clientCharacterState |= CLIENT_CHARACTER_STATE_SMOOTH_RESET;
		CharacterUtil::FillHpToMax(*newChar);
	}

	// Follower 관계 설정
	SetupFollowerRelation(newEntity, newLeaderIdp);

	return true;
}

// ============================================================================
// ReviveFollowerCommon: Follower 부활 공통 로직
// ============================================================================
bool MotionSystemDead::ReviveFollowerCommon(Entity& entity, CharacterComponent& character, const FollowerReviveParams& params)
{
	const STATIC_CHARACTER* staticCharacter = character.GetCharacterDoc().mStaticCharacter;
	if (!staticCharacter)
	{
		entity.ReserveDestroy();
		return true;
	}

	// 원래 NPC 정보 보존
	const Scalar originalSpawnX = character.originalSpawnX;
	const Scalar originalSpawnY = character.originalSpawnY;
	EntityIdp originalNpcLeaderIdp = character.originalNpcLeaderIdp;
	EntityIdp originalSpawnerIdp = character.originalSpawnerIdp;

	// originalSpawnerIdp 보완
	if (originalSpawnerIdp == INVALID_ENTITY_IDP)
	{
		if (NpcComponent* npc = TryGetComponent<NpcComponent>(entity))
			originalSpawnerIdp = npc->spawnerIdp;
	}

	// 기존 관계 정리 및 파괴
	CleanupLeaderFollowerRelations(entity);
	entity.ReserveDestroy();

	// 새 리더 결정
	EntityIdp newLeaderIdp = params.newLeaderIdp;
	if (params.resolveNpcLeader && newLeaderIdp == INVALID_ENTITY_IDP)
	{
		// NPC 리더 자동 결정: 원래 리더 또는 같은 Spawner 그룹의 리더 찾기
		if (originalNpcLeaderIdp != INVALID_ENTITY_IDP)
		{
			Entity* originalLeaderEntity = GetEntity(originalNpcLeaderIdp);
			if (originalLeaderEntity && TryGetComponent<LeaderComponent>(*originalLeaderEntity))
			{
				newLeaderIdp = originalNpcLeaderIdp;
				// 리더가 Dead 상태면 즉시 부활시킴
				if (MotionComponentDead* leaderDead = TryGetComponent<MotionComponentDead>(*originalLeaderEntity))
					leaderDead->now = leaderDead->duration;
			}
			else
			{
				newLeaderIdp = FindNpcLeaderInSpawnerGroup(originalSpawnerIdp);
				if (newLeaderIdp != INVALID_ENTITY_IDP)
					originalNpcLeaderIdp = newLeaderIdp;
			}
		}
	}

	// 새 Entity 생성
	EntityDescription entityDescription = EntityUtil::DescriptEntityPcNpc(
		EntityDescriptorArg{
			.x = params.spawnX,
			.y = params.spawnY,
			.characterSid = staticCharacter->Get_c_character_item_sid(),
			.characterType = staticCharacter->Get_c_character_type(),
			.worldUserId = params.newWorldUserId,
			.spawnerIdp = originalSpawnerIdp,
			.commandX = 0,
			.commandY = 0
		});

	Entity* newEntity = CreateEntityFromDescription(entityDescription);

	// 원래 NPC 정보 전파 및 관계 설정
	PropagateOriginalNpcInfo(newEntity, originalSpawnX, originalSpawnY, originalNpcLeaderIdp, originalSpawnerIdp);
	SetupFollowerRelation(newEntity, newLeaderIdp);

	// HP 회복 및 스무스 리셋
	if (newEntity)
	{
		if (CharacterComponent* newChar = TryGetComponent<CharacterComponent>(*newEntity))
		{
			CharacterUtil::FillHpToMax(*newChar);
			newChar->clientCharacterState |= CLIENT_CHARACTER_STATE_SMOOTH_RESET;
		}
	}

	return true;
}

// ============================================================================
// ReviveNpcLeaderToNpc: NPC Leader → NPC Leader (제자리 부활)
// ============================================================================
bool MotionSystemDead::ReviveNpcLeaderToNpc(Entity& entity, CharacterComponent& character)
{
	PositionComponent* position = TryGetComponent<PositionComponent>(entity);
	const STATIC_CHARACTER* staticCharacter = character.GetCharacterDoc().mStaticCharacter;

	if (!position || !staticCharacter)
	{
		entity.ReserveDestroy();
		return true;
	}

	const Scalar originalSpawnX = character.originalSpawnX;
	const Scalar originalSpawnY = character.originalSpawnY;
	EntityIdp originalSpawnerIdp = character.originalSpawnerIdp;
	if (originalSpawnerIdp == INVALID_ENTITY_IDP)
	{
		if (NpcComponent* npc = TryGetComponent<NpcComponent>(entity))
			originalSpawnerIdp = npc->spawnerIdp;
	}

	// NPC Leader 부활 시, Dead 상태인 NPC Follower들도 함께 부활시킴
	const EntityIdp leaderIdp = entity.GetEntityIdp();
	mEngine.ForEachEntity([&](Entity& followerEntity)
	{
		CharacterComponent* followerChar = TryGetComponent<CharacterComponent>(followerEntity);
		if (!followerChar)
			return;
		
		// 원래 이 리더에 속했던 Follower인지?
		if (followerChar->originalNpcLeaderIdp != leaderIdp)
			return;
		
		// NPC Follower인지? (PC에게 포획되지 않음)
		if (!TryGetComponent<NpcComponent>(followerEntity))
			return;
		
		// Dead 상태면 부활 트리거 (dead.now = duration)
		if (MotionComponentDead* followerDead = TryGetComponent<MotionComponentDead>(followerEntity))
		{
			followerDead->now = followerDead->duration;  // 다음 Update에서 부활
		}
	});

	// In-place 부활
	DetachComponent<AbilityComponentUntargetrable>(entity);

	position->x = originalSpawnX;
	position->y = originalSpawnY;
	character.clientCharacterState |= CLIENT_CHARACTER_STATE_SMOOTH_RESET;

	CharacterUtil::FillHpToMax(character);

	character.skillTarget = INVALID_ENTITY_IDP;
	if (LeaderComponent* leaderComp = TryGetComponent<LeaderComponent>(entity))
		leaderComp->leaderEntityTargetIdp = INVALID_ENTITY_IDP;

	if (PhysicsComponent* physics = TryGetComponent<PhysicsComponent>(entity))
		physics->SetAllArrived();

	return true;
}

// ============================================================================
// Spawner 공통 로직
// ============================================================================
bool MotionSystemDead::ReviveSpawnerCommon(Entity& entity, CharacterComponent& character, WorldUserId newWorldUserId)
{
	PositionComponent* position = TryGetComponent<PositionComponent>(entity);
	SpawnerComponent* spawner = TryGetComponent<SpawnerComponent>(entity);
	const STATIC_CHARACTER* staticCharacter = character.GetCharacterDoc().mStaticCharacter;

	if (!position || !spawner || !staticCharacter)
	{
		entity.ReserveDestroy();
		return true;
	}

	entity.ReserveDestroy();

	const Scalar originalSpawnX = character.originalSpawnX;
	const Scalar originalSpawnY = character.originalSpawnY;
	const Scalar defaultCommandX = spawner->defaultCommandX;
	const Scalar defaultCommandY = spawner->defaultCommandY;
	const EntityIdp pairCastleIdp = spawner->pairCastleIdp;

	EntityDescription entityDescription = EntityUtil::DescriptSpawner(
		EntityDescriptorArg{
			.x = originalSpawnX,
			.y = originalSpawnY,
			.characterSid = staticCharacter->Get_c_character_item_sid(),
			.characterType = staticCharacter->Get_c_character_type(),
			.worldUserId = newWorldUserId,
			.spawnerIdp = INVALID_ENTITY_IDP,
			.commandX = defaultCommandX,
			.commandY = defaultCommandY
		});

	Entity* newEntity = CreateEntityFromDescription(entityDescription);

	if (newEntity)
	{
		if (CharacterComponent* newChar = TryGetComponent<CharacterComponent>(*newEntity))
		{
			newChar->originalSpawnX = originalSpawnX;
			newChar->originalSpawnY = originalSpawnY;
			newChar->clientCharacterState |= CLIENT_CHARACTER_STATE_SMOOTH_RESET;
		}

		if (SpawnerComponent* newSpawner = TryGetComponent<SpawnerComponent>(*newEntity))
		{
			newSpawner->defaultCommandX = defaultCommandX;
			newSpawner->defaultCommandY = defaultCommandY;
			newSpawner->pairCastleIdp = pairCastleIdp;
		}

		// NPC Spawner의 spawnerIdp 설정은 EntityFactoryBase::InitEntity에서 처리됨

		if (CharacterComponent* newChar = TryGetComponent<CharacterComponent>(*newEntity))
			CharacterUtil::FillHpToMax(*newChar);
	}

	return true;
}

// ============================================================================
// RevivePcSpawnerToPc: PC Spawner → PC Spawner (소유권 이전)
// ============================================================================
bool MotionSystemDead::RevivePcSpawnerToPc(Entity& entity, CharacterComponent& character)
{
	MotionComponentDead* dead = TryGetComponent<MotionComponentDead>(entity);
	if (!dead)
	{
		entity.ReserveDestroy();
		return true;
	}

	WorldUserId newWorldUserId = INVALID_WORLD_USER_ID;
	if (Entity* killerEntity = GetEntity(dead->killerEntityIdp))
	{
		if (PcComponent* killerPc = TryGetComponent<PcComponent>(*killerEntity))
			newWorldUserId = killerPc->worldUserId;
	}

	return ReviveSpawnerCommon(entity, character, newWorldUserId);
}

// ============================================================================
// RevivePcSpawnerToNpc: PC Spawner → NPC Spawner
// ============================================================================
bool MotionSystemDead::RevivePcSpawnerToNpc(Entity& entity, CharacterComponent& character)
{
	return ReviveSpawnerCommon(entity, character, INVALID_WORLD_USER_ID);
}

// ============================================================================
// ReviveNpcSpawnerToPc: NPC Spawner → PC Spawner
// ============================================================================
bool MotionSystemDead::ReviveNpcSpawnerToPc(Entity& entity, CharacterComponent& character)
{
	MotionComponentDead* dead = TryGetComponent<MotionComponentDead>(entity);
	if (!dead)
	{
		entity.ReserveDestroy();
		return true;
	}

	WorldUserId newWorldUserId = INVALID_WORLD_USER_ID;
	if (Entity* killerEntity = GetEntity(dead->killerEntityIdp))
	{
		if (PcComponent* killerPc = TryGetComponent<PcComponent>(*killerEntity))
			newWorldUserId = killerPc->worldUserId;
	}

	return ReviveSpawnerCommon(entity, character, newWorldUserId);
}

// ============================================================================
// ReviveNpcSpawnerToNpc: NPC Spawner → NPC Spawner
// ============================================================================
bool MotionSystemDead::ReviveNpcSpawnerToNpc(Entity& entity, CharacterComponent& character)
{
	return ReviveSpawnerCommon(entity, character, INVALID_WORLD_USER_ID);
}

// ============================================================================
// 공통 헬퍼 함수들
// ============================================================================

void MotionSystemDead::CleanupLeaderFollowerRelations(Entity& entity)
{
	const EntityIdp dyingEntityIdp = entity.GetEntityIdp();

	if (TryGetComponent<LeaderComponent>(entity))
	{
		mEngine.ForEachEntity([&](Entity& followerEntity)
		{
			if (FollowerComponent* followerComp = TryGetComponent<FollowerComponent>(followerEntity))
			{
				if (followerComp->leaderEntityIdp == dyingEntityIdp)
					followerComp->leaderEntityIdp = INVALID_ENTITY_IDP;
			}
		});
	}

	if (FollowerComponent* oldFollowerComp = TryGetComponent<FollowerComponent>(entity))
	{
		if (Entity* oldLeaderEntity = GetEntity(oldFollowerComp->leaderEntityIdp))
		{
			if (LeaderComponent* oldLeaderComp = TryGetComponent<LeaderComponent>(*oldLeaderEntity))
			{
				if (oldLeaderComp->followerCnt > 0)
					--oldLeaderComp->followerCnt;
			}
		}
	}
}

void MotionSystemDead::PropagateOriginalNpcInfo(Entity* newEntity, Scalar originalSpawnX, Scalar originalSpawnY,
	EntityIdp originalNpcLeaderIdp, EntityIdp originalSpawnerIdp)
{
	if (!newEntity)
		return;

	if (CharacterComponent* newChar = TryGetComponent<CharacterComponent>(*newEntity))
	{
		newChar->originalSpawnX = originalSpawnX;
		newChar->originalSpawnY = originalSpawnY;
		newChar->originalNpcLeaderIdp = originalNpcLeaderIdp;
		newChar->originalSpawnerIdp = originalSpawnerIdp;
	}
}

void MotionSystemDead::SetupFollowerRelation(Entity* newEntity, EntityIdp leaderIdp)
{
	if (!newEntity || leaderIdp == INVALID_ENTITY_IDP)
		return;

	FollowerComponent& followerComp = AttachComponent<FollowerComponent>(*newEntity);
	followerComp.leaderEntityIdp = leaderIdp;

	if (LeaderComponent* leaderComp = TryGetComponent<LeaderComponent>(leaderIdp))
		++leaderComp->followerCnt;
}

EntityIdp MotionSystemDead::FindNpcLeaderInSpawnerGroup(EntityIdp originalSpawnerIdp)
{
	if (originalSpawnerIdp == INVALID_ENTITY_IDP)
		return INVALID_ENTITY_IDP;

	EntityIdp foundLeaderIdp = INVALID_ENTITY_IDP;

	mEngine.ForEachEntity([&](Entity& otherEntity)
	{
		if (foundLeaderIdp != INVALID_ENTITY_IDP)
			return;

		if (!TryGetComponent<LeaderComponent>(otherEntity))
			return;

		NpcComponent* otherNpc = TryGetComponent<NpcComponent>(otherEntity);
		if (!otherNpc)
			return;

		if (otherNpc->spawnerIdp == originalSpawnerIdp)
			foundLeaderIdp = otherEntity.GetEntityIdp();
	});

	return foundLeaderIdp;
}

// ============================================================================
// CheckNpcLeaderReviveCondition: NPC Leader 부활 조건 체크
// ============================================================================
// 조건: 살아있는 PC Follower가 없고, Dead NPC Follower도 없음
bool MotionSystemDead::CheckNpcLeaderReviveCondition(EntityIdp leaderIdp)
{
	bool hasPcFollower = false;        // 살아있는 PC Follower가 있는가?
	bool hasDeadNpcFollower = false;   // Dead/Dying NPC Follower가 있는가?

	mEngine.ForEachEntity([&](Entity& otherEntity)
	{
		CharacterComponent* otherChar = TryGetComponent<CharacterComponent>(otherEntity);
		if (!otherChar)
			return;

		// 원래 이 리더에 속했던 Follower인지?
		if (otherChar->originalNpcLeaderIdp != leaderIdp)
			return;

		const bool isDead = otherEntity.HasComponent(MotionComponentDead::GetComponentType()) ||
			otherEntity.HasComponent(MotionComponentDying::GetComponentType());

		// PC에게 포획된 상태인지?
		if (TryGetComponent<PcComponent>(otherEntity))
		{
			// Dead/Dying 상태인 PC Follower는 "돌아오는 중"이므로 포획 중으로 간주하지 않음
			if (!isDead)
				hasPcFollower = true;
			return;
		}

		// NPC Follower인데 Dead/Dying 상태인지?
		if (isDead)
		{
			hasDeadNpcFollower = true;
		}
	});

	// 부활 조건: 살아있는 PC Follower가 없고, Dead NPC Follower도 없음
	return !hasPcFollower && !hasDeadNpcFollower;
}

// ============================================================================
// TryTriggerNpcLeaderRevive: Follower 상태 변경 시 NPC Leader 부활 트리거
// ============================================================================
void MotionSystemDead::TryTriggerNpcLeaderRevive(EntityIdp originalNpcLeaderIdp)
{
	if (originalNpcLeaderIdp == INVALID_ENTITY_IDP)
		return;

	Entity* leaderEntity = GetEntity(originalNpcLeaderIdp);
	if (!leaderEntity)
		return;

	// Leader가 Dead 상태가 아니면 트리거 불필요
	MotionComponentDead* leaderDead = TryGetComponent<MotionComponentDead>(*leaderEntity);
	if (!leaderDead)
		return;

	// 부활 조건 체크
	if (CheckNpcLeaderReviveCondition(originalNpcLeaderIdp))
	{
		// 조건 충족 → Leader Dead 모션 즉시 완료
		leaderDead->now = leaderDead->duration;
	}
}
