// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Motion/MotionSystemDying.h"
#include "MmoSync/System/Motion/MotionSystemDead.h"

#include "MmoSync/DebugControlTool/DebugInfo.h"
#include "MmoSync/Util/EntityUtil/EntityUtil.h"
#include "MmoSync/Component/PcComponent.h"
#include "MmoSync/Component/CharacterComponent.h"
#include "MmoSync/Component/LeaderComponent.h"
#include "MmoSync/Component/MotionComponent.h"
#include "MmoSync/Grid/MultiGrid.h"


MotionSystemDying::MotionSystemDying(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	DurationSystemBase<MotionSystemDying, MotionComponentDying>(engine, timeStepUpdateCycle, game)
{
}
// ============================================================================
// OnMotionCompleted: 메인 진입점
// ============================================================================
bool MotionSystemDying::OnMotionCompleted(Entity& entity, CharacterComponent& character)
{
	// 유형 판별
	const DyingType dyingType = DetermineDyingType(entity, character);

	// 유형별 처리
	switch (dyingType)
	{
	case DyingType::Golem:
		return ProcessGolem(entity, character);

	case DyingType::Spawnee:
		return ProcessSpawnee(entity, character);

	case DyingType::Spawner:
		return ProcessSpawner(entity, character);

	case DyingType::CapturedByPc:
		return ProcessCapturedByPc(entity, character);

	case DyingType::Normal:
	default:
		return ProcessNormal(entity, character);
	}
}

// ============================================================================
// DetermineDyingType: 사망 유형 판별
// ============================================================================
// 
// [DyingType 매핑 규칙 테이블]
// ┌─────────────────┬────────────────────────────────┬──────────────────┐
// │ Condition       │ Description                    │ DyingType        │
// ├─────────────────┼────────────────────────────────┼──────────────────┤
// │ Blocking Skill  │ 골렘 → Rock 생성 후 Dead 전환  │ Golem            │
// │ Spawnee         │ Spawner 카운트 감소 후 Destroy │ Spawnee          │
// │ Spawner         │ Spawnee들 죽이고 Dead 전환     │ Spawner          │
// │ NPC killed by PC│ 빠른 Dead 전환 (포획)          │ CapturedByPc     │
// │ Otherwise       │ 일반 Dead 전환                 │ Normal           │
// └─────────────────┴────────────────────────────────┴──────────────────┘
//
MotionSystemDying::DyingType MotionSystemDying::DetermineDyingType(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	// DeathContext 빌드
	EntityIdp killerEntityIdp = INVALID_ENTITY_IDP;
	if (MotionComponentDying* dying = TryGetComponent<MotionComponentDying>(entity))
		killerEntityIdp = dying->killerEntityIdp;

	const DeathContext ctx = DeathContext::Build(mEngine, entity, killerEntityIdp);

	// 골렘은 Rock 생성 후 Dead 전환 (Spawnee보다 먼저 체크 - 스포너가 생성한 골렘도 Rock 생성 필요)
	if (entity.HasComponent(SkillComponentBlocking::GetComponentType()))
		return DyingType::Golem;

	// Spawnee는 바로 Destroy
	if (ctx.isSpawnee)
		return DyingType::Spawnee;

	// Spawner(Castle)는 Spawnee들을 죽이고 Dead 전환
	if (ctx.isSpawner)
		return DyingType::Spawner;

	// NPC가 PC에게 죽음 → 빠른 Dead 전환 (포획) (NPC Leader 제외)
	if (!ctx.isNpcLeader && ctx.killedByPc)
		return DyingType::CapturedByPc;

	return DyingType::Normal;
}

// ============================================================================
// ProcessGolem: 골렘 사망 → Rock 생성 후 Dead 전환 또는 Destroy
// ============================================================================
bool MotionSystemDying::ProcessGolem(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	// [FIX] EntityIdp를 먼저 저장 (CreateEntityFromDescription 호출 시 mDataList 재할당으로 entity 참조가 무효화될 수 있음)
	const EntityIdp entityIdp = entity.GetEntityIdp();

	// Rock 생성
	if (PositionComponent* position = TryGetComponent<PositionComponent>(entityIdp))
	{
		Scalar x = GridUtil::GetCenterX(GridUtil::ConvToCol(position->x));
		Scalar y = GridUtil::GetCenterY(GridUtil::ConvToRow(position->y));

		EntityDescription entityDescription = EntityUtil::DescriptEntityNormal(
			EntityDescriptorArg{
				.x = x,
				.y = y,
				.characterSid = 1,
				.characterType = CharacterType::ROCK,
				.worldUserId = INVALID_WORLD_USER_ID,
				.spawnerIdp = INVALID_ENTITY_IDP,
				.commandX = 0,
				.commandY = 0
			});
		MAYBE_UNUSED Entity* rock = CreateEntityFromDescription(entityDescription);
	}

	// [FIX] CreateEntityFromDescription 후 entity 참조가 무효화되었을 수 있으므로 다시 가져옴
	Entity* entityPtr = GetEntity(entityIdp);
	if (!entityPtr)
		return true;

	// Spawnee인 경우: 카운트 감소 후 바로 Destroy (부활하지 않음)
	if (TryGetComponent<SpawneeComponent>(*entityPtr))
	{
		DecreaseSpawnerCount(*entityPtr);
		entityPtr->ReserveDestroy();
		return true;
	}

	// Dead 전환 (일반 duration)
	EntityIdp killerEntityIdp = INVALID_ENTITY_IDP;
	if (MotionComponentDying* dying = TryGetComponent<MotionComponentDying>(*entityPtr))
		killerEntityIdp = dying->killerEntityIdp;

	const int32_t duration = GetRandom(DebugInfo::MOTION_DURATION_DEAD_MIN, DebugInfo::MOTION_DURATION_DEAD_MAX);
	TransitionToDead(*entityPtr, killerEntityIdp, duration);

	return true;
}

// ============================================================================
// ProcessSpawnee: Spawnee 사망 → Spawner 카운트 감소 후 Destroy
// ============================================================================
bool MotionSystemDying::ProcessSpawnee(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	DecreaseSpawnerCount(entity);
	entity.ReserveDestroy();
	return true;
}

// ============================================================================
// ProcessSpawner: Spawner(Castle) 사망 → Spawnee들 죽이고 Dead 전환
// ============================================================================
bool MotionSystemDying::ProcessSpawner(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	const EntityIdp spawnerIdp = entity.GetEntityIdp();

	// killerEntityIdp 가져오기
	EntityIdp killerEntityIdp = INVALID_ENTITY_IDP;
	if (MotionComponentDying* dying = TryGetComponent<MotionComponentDying>(entity))
		killerEntityIdp = dying->killerEntityIdp;

	// 모든 Spawnee 죽이기
	KillAllSpawnees(spawnerIdp, killerEntityIdp);

	// Dead 전환 (일반 duration)
	const int32_t duration = GetRandom(DebugInfo::MOTION_DURATION_DEAD_MIN, DebugInfo::MOTION_DURATION_DEAD_MAX);
	TransitionToDead(entity, killerEntityIdp, duration);

	return true;
}

// ============================================================================
// ProcessCapturedByPc: NPC가 PC에게 죽음 → 빠른 Dead 전환 (포획)
// ============================================================================
bool MotionSystemDying::ProcessCapturedByPc(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	EntityIdp killerEntityIdp = INVALID_ENTITY_IDP;
	if (MotionComponentDying* dying = TryGetComponent<MotionComponentDying>(entity))
		killerEntityIdp = dying->killerEntityIdp;

	// 빠른 부활을 위해 duration을 1로 설정
	TransitionToDead(entity, killerEntityIdp, 1);

	return true;
}

// ============================================================================
// ProcessNormal: 일반 사망 → Dead 전환
// ============================================================================
bool MotionSystemDying::ProcessNormal(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	EntityIdp killerEntityIdp = INVALID_ENTITY_IDP;
	if (MotionComponentDying* dying = TryGetComponent<MotionComponentDying>(entity))
		killerEntityIdp = dying->killerEntityIdp;

	const int32_t duration = GetRandom(DebugInfo::MOTION_DURATION_DEAD_MIN, DebugInfo::MOTION_DURATION_DEAD_MAX);
	TransitionToDead(entity, killerEntityIdp, duration);

	return true;
}

// ============================================================================
// 공통 헬퍼 함수들
// ============================================================================

void MotionSystemDying::TransitionToDead(Entity& entity, EntityIdp killerEntityIdp, int32_t duration)
{
	MotionComponentDead& deadComp = AttachComponent<MotionComponentDead>(entity);
	deadComp.now = 0;
	deadComp.killerEntityIdp = killerEntityIdp;
	deadComp.duration = duration;
}

void MotionSystemDying::DecreaseSpawnerCount(Entity& spawneeEntity)
{
	SpawneeComponent* spawnee = TryGetComponent<SpawneeComponent>(spawneeEntity);
	if (!spawnee)
		return;

	Entity* spawnerEntity = GetEntity(spawnee->spawnerIdp);
	if (!spawnerEntity)
		return;

	SpawnerComponent* spawner = TryGetComponent<SpawnerComponent>(*spawnerEntity);
	if (!spawner)
		return;

	if (spawner->currentActiveSpawnCount > 0)
		--spawner->currentActiveSpawnCount;
}

void MotionSystemDying::KillAllSpawnees(EntityIdp spawnerIdp, EntityIdp killerEntityIdp)
{
	// Spawner 사망 시 호출: 모든 Spawnee를 Dead 상태로 전환
	// 카운트 감소 불필요: Spawner 부활 시 새 Entity가 되어 currentActiveSpawnCount가 0으로 초기화됨
	mEngine.ForEachEntity([&](Entity& spawneeEntity)
	{
		// 이미 죽어가거나 죽은 상태면 스킵
		if (spawneeEntity.HasComponent(MotionComponentDying::GetComponentType()) ||
			spawneeEntity.HasComponent(MotionComponentDead::GetComponentType()))
			return;

		// 이 Spawner의 Spawnee인지 확인
		SpawneeComponent* spawneeComp = TryGetComponent<SpawneeComponent>(spawneeEntity);
		if (!spawneeComp || spawneeComp->spawnerIdp != spawnerIdp)
			return;

		// spawnerIdp 무효화 (Spawner가 죽었으므로)
		spawneeComp->spawnerIdp = INVALID_ENTITY_IDP;

		// Spawnee를 즉시 Dead 상태로 전환 (Dying 건너뜀)
		AttachComponent<AbilityComponentUntargetrable>(spawneeEntity);
		MotionComponentDead& spawneeDead = AttachComponent<MotionComponentDead>(spawneeEntity);
		spawneeDead.now = 0;
		spawneeDead.duration = GetRandom(DebugInfo::MOTION_DURATION_DEAD_MIN, DebugInfo::MOTION_DURATION_DEAD_MAX);
		spawneeDead.killerEntityIdp = killerEntityIdp;
	});
}
