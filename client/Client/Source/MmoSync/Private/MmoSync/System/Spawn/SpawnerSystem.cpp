// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Spawn/SpawnerSystem.h"

#include "MmoSync/Util/EntityUtil/EntityUtil.h"
#include "MmoSync/Entity/Description/EntityDescription.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"


SpawnerSystem::SpawnerSystem(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<PositionComponent>();
	IncludeComponent<SpawnerComponent>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
	mCharacterRatioTable.Add(std::make_pair(100, CharacterType::WARRIOR), 1);
	mCharacterRatioTable.Add(std::make_pair(200, CharacterType::ARCHER), 1);
	mCharacterRatioTable.Add(std::make_pair(300, CharacterType::ASSASSIN), 1);
	mCharacterRatioTable.Add(std::make_pair(400, CharacterType::BAT), 1);
	mCharacterRatioTable.Add(std::make_pair(500, CharacterType::BEAR), 1);
	mCharacterRatioTable.Add(std::make_pair(600, CharacterType::SPIDER), 1);
	mCharacterRatioTable.Add(std::make_pair(700, CharacterType::ZEUS), 1);
	mCharacterRatioTable.Add(std::make_pair(800, CharacterType::HADES), 1);
	mCharacterRatioTable.Add(std::make_pair(900, CharacterType::CUPID), 1);
	mCharacterRatioTable.Add(std::make_pair(1000, CharacterType::GOLEM), 1);
	mCharacterRatioTable.Add(std::make_pair(1100, CharacterType::PHOENIX), 1);
	mCharacterRatioTable.Add(std::make_pair(1200, CharacterType::DRAGON), 1);
}
void SpawnerSystem::Update()
{
	if (Data().empty())
		return;

	const TimeStepUpdateCycle timeDelta = GetTimeStepUpdateCycle();

	for (EntityIdp entityIdp : Data())
	{
		// PC 소속 Castle은 Spawnee 생성 스킵
		if (TryGetComponent<PcComponent>(entityIdp))
			continue;

		PositionComponent& position0 = GetComponent<PositionComponent>(entityIdp.first);
		SpawnerComponent& spawner0 = GetComponent<SpawnerComponent>(entityIdp.first);		

		if (spawner0.currentActiveSpawnCount >= DebugInfo::SPAWNER_MAX_COUNT)
			continue;
		
		if (spawner0.totalAccumulatedSpawnTime + timeDelta < DebugInfo::SPAWNER_MAX_COST_TIME)
			spawner0.totalAccumulatedSpawnTime += timeDelta;
		else 
			spawner0.totalAccumulatedSpawnTime = DebugInfo::SPAWNER_MAX_COST_TIME;

		const Span costTime = (DebugInfo::SPAWNER_COST_TIME + DebugInfo::SPAWNER_COUNT_PENALTY * spawner0.currentActiveSpawnCount);

		if (spawner0.totalAccumulatedSpawnTime >= costTime)
		{
			spawner0.totalAccumulatedSpawnTime -= costTime;

			// 페어 캐슬이 있는 경우, 페어 캐슬의 스폰 카운트가 내 스폰 카운트보다 작으면 생산 스킵
			if (spawner0.pairCastleIdp != INVALID_ENTITY_IDP)
				if (Entity* pairCastle = GetEntity(spawner0.pairCastleIdp))
					if (SpawnerComponent* pairSpawner = TryGetComponent<SpawnerComponent>(*pairCastle))
						if (pairSpawner->currentActiveSpawnCount < spawner0.currentActiveSpawnCount)
							continue;

			std::pair<ItemSid, CharacterType> characterInfo;
			const int randIdx = (entityIdp.first % 3);
			if (randIdx == 0)
			{
				if (false == mCharacterRatioTable.At(0, OUT characterInfo))
					continue;
			}
			else if (randIdx == 1)
			{
				if (false == mCharacterRatioTable.At(1, OUT characterInfo))
					continue;
			}
			else
			{
				if (false == mCharacterRatioTable.Pick(OUT characterInfo, [this](size_t range) { return this->GetRandom((int32_t)range); }))
					continue;
			}

			WorldUserId worldUserId = INVALID_WORLD_USER_ID;
			if (PcComponent* pc0 = TryGetComponent<PcComponent>(entityIdp))
				worldUserId = pc0->worldUserId;

			// 새로운 생성하면 참조가 유효하지 않기 때문에 미리 필요한 값을 구한다
			const Scalar spawnX = position0.x;
			const Scalar spawnY = position0.y;
			const Scalar commandX = spawner0.defaultCommandX;
			const Scalar commandY = spawner0.defaultCommandY;

			EntityDescription entityDescription = EntityUtil::DescriptSpawnee(
				EntityDescriptorArg{ 
					.x = spawnX,
					.y = spawnY,	
					.characterSid = characterInfo.first,
					.characterType = characterInfo.second,
					.worldUserId = worldUserId,
					.spawnerIdp = entityIdp,
					.commandX = commandX,
					.commandY = commandY
				});
			MAYBE_UNUSED Entity* spawnee = CreateEntityFromDescription(entityDescription);

			// 새로운 생성 후 참조가 유효하지 않으므로 다시 가져온다
			if (SpawnerComponent* spawner = TryGetComponent<SpawnerComponent>(entityIdp))
				++spawner->currentActiveSpawnCount;
		}
	}
}
