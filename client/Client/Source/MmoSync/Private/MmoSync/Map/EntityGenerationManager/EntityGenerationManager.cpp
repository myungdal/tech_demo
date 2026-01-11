// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Map/EntityGeneration/EntityGenerationManager.h"

#include "MmoSync/Grid/MultiGrid.h"
#include "MmoSync/Map/Configuration/FieldMapConfig.h"
#include "MmoSync/Map/Configuration/DungeonMapConfig.h"
#include "MmoSync/Util/EntityUtil/EntityUtil.h"
#include "MmoSync/Util/GridUtil/GridUtil.h"
#include "MmoSync/Map/EntityGeneration/CastlePosGenerator.h"
#include "MmoSync/Map/EntityGeneration/NestPosGenerator.h"
#include "MmoSync/Component/LeaderComponent.h"
#include "MmoSync/Component/FollowerComponent.h"
#include "MmoSync/Component/NpcComponent.h"
#include "Common/Random/DeterministicRandom/DeterministicRandom64.h"
#include "Packet/StaticDataAccessor/StaticDataAccessor.h"
#include "Packet/StaticData/Character/CharacterDocLookup.h"
#include "Packet/StaticData/Character/CharacterDoc.h"
#include <memory>

// 통합된 엔티티 생성 메서드들
EntityDescriptionList EntityGenerationManager::CreateCastles(const MultiGrid& grid, int castlePairCount, std::deque<ColRowPair>& posList, const AsciiMap& asciiMap, uint64_t randomSeed)
{
	auto posGenerator = std::make_unique<CastlePosGenerator>(grid, posList);
	
	// 위치 생성
	posGenerator->InitCastlePositions(castlePairCount, asciiMap, randomSeed);
	
	// 위치를 기반으로 EntityDescriptionList 생성
	EntityDescriptionList entityList;
	const auto& castlePositions = posGenerator->GetCastlePositions();
	
	// Castle용 characterSid
	const ItemSid castleCharacterSid = 200;
	
	// static_character에서 CharacterType 가져오기
	CharacterType castleCharacterType = CharacterType::ARCHER; // 기본값
	std::shared_ptr<CharacterDocLookup> characterDocLookup = gStaticDataAccessor->Get<CharacterDocLookup>();
	if (characterDocLookup)
	{
		const CharacterDoc* characterDoc = characterDocLookup->Find(castleCharacterSid);
		if (characterDoc && characterDoc->mStaticCharacter)
		{
			castleCharacterType = characterDoc->mStaticCharacter->Get_c_character_type();
		}
	}
	
	for (size_t i = 0; i < castlePositions.size(); i += 2)
	{
		if (i + 1 >= castlePositions.size())
			break;
			
		const ColRowPair cell0 = castlePositions[i];
		const ColRowPair cell1 = castlePositions[i + 1];
		const Scalar x0 = GridUtil::GetCenterX(cell0.first);
		const Scalar y0 = GridUtil::GetCenterY(cell0.second);
		const Scalar x1 = GridUtil::GetCenterX(cell1.first);
		const Scalar y1 = GridUtil::GetCenterY(cell1.second);

		EntityDescription entityDescription0 = EntityUtil::DescriptSpawner(
			EntityDescriptorArg{
				.x = x0,
				.y = y0,
				.characterSid = castleCharacterSid,
				.characterType = castleCharacterType,
				.worldUserId = INVALID_WORLD_USER_ID,
				.spawnerIdp = INVALID_ENTITY_IDP,
				.commandX = x1,
				.commandY = y1
			}
		);

		EntityDescription entityDescription1 = EntityUtil::DescriptSpawner(
			EntityDescriptorArg{
				.x = x1,
				.y = y1,
				.characterSid = castleCharacterSid,
				.characterType = castleCharacterType,
				.worldUserId = INVALID_WORLD_USER_ID,
				.spawnerIdp = INVALID_ENTITY_IDP,
				.commandX = x0,
				.commandY = y0
			}
		);

		entityList.push_back(entityDescription0);
		entityList.push_back(entityDescription1);
	}
	
	return entityList;
}

EntityDescriptionList EntityGenerationManager::CreateNestLeaders(const MultiGrid& grid, int nestCount, std::deque<ColRowPair>& posList, const AsciiMap& asciiMap, uint64_t randomSeed)
{
	auto posGenerator = std::make_unique<NestPosGenerator>(grid, posList);
	
	posGenerator->InitNestPositions(nestCount, asciiMap, randomSeed);

	// 위치를 기반으로 EntityDescriptionList 생성
	EntityDescriptionList entityList;
	const auto& nestPositions = posGenerator->GetNestPositions();
	
	DeterministicRandom64 random(randomSeed);
	
	// CharacterDocLookup 미리 가져오기
	std::shared_ptr<CharacterDocLookup> characterDocLookup = gStaticDataAccessor->Get<CharacterDocLookup>();
	
	for (const auto& position : nestPositions)
	{
		const ColRowPair cell = std::get<0>(position);
		const ItemGrade itemGrade = std::get<1>(position);
		const Scalar centerX = GridUtil::GetCenterX(cell.first);
		const Scalar centerY = GridUtil::GetCenterY(cell.second);
		
		// ItemGrade에 따른 캐릭터 SID 결정
		ItemSid characterSid = 10000;
		if (static_cast<int32_t>(itemGrade) == 0) characterSid = 10000;
		else if (static_cast<int32_t>(itemGrade) == 1) characterSid = 20000;
		else if (static_cast<int32_t>(itemGrade) == 2) characterSid = 30000;
		else if (static_cast<int32_t>(itemGrade) == 3) characterSid = 40000;
		
		// static_character에서 CharacterType 가져오기
		CharacterType characterType = CharacterType::WARRIOR; // 기본값
		if (characterDocLookup)
		{
			const CharacterDoc* characterDoc = characterDocLookup->Find(characterSid);
			if (characterDoc && characterDoc->mStaticCharacter)
			{
				characterType = characterDoc->mStaticCharacter->Get_c_character_type();
			}
		}
		
		// 팔로워 수 결정 (2~5마리)
		const int followerCount = 2 + (random() % 4);
		
		EntityDescription leaderDescription = EntityUtil::DescriptEntityPcNpc(
			EntityDescriptorArg{
				.x = centerX,
				.y = centerY,
				.characterSid = characterSid,
				.characterType = characterType,
				.worldUserId = INVALID_WORLD_USER_ID,
				.spawnerIdp = INVALID_ENTITY_IDP,
				.commandX = 0,
				.commandY = 0
			}
		);
		
		// LeaderComponent 추가
		{
			auto c = EntityUtil::TakeComponent<LeaderComponent>(leaderDescription);
			c->followerCnt = (uint8_t)followerCount;
			c->leaderEntityTargetIdp = INVALID_ENTITY_IDP;
			c->worldUserCommandKeyState = 0;
		}

		entityList.push_back(leaderDescription);       		
	}
	
	return entityList;
}

EntityDescriptionList EntityGenerationManager::CreateNestFollowers(int followerCount, Scalar centerX, Scalar centerY, MAYBE_UNUSED ItemSid characterSid, EntityIdp leaderEntityIdp)
{
	EntityDescriptionList entityList;
	
	// 팔로워 가능한 characterSid 목록
	static const ItemSid followerSidList[] = {
		100, 200, 300, 400, 500, 600, 
		700, 800, 900, 1000, 1100, 1200
	};
	static const int followerSidListSize = sizeof(followerSidList) / sizeof(followerSidList[0]);
	
	// 결정론적 랜덤 생성 (leaderEntityIdp를 시드로 사용)
	DeterministicRandom64 random(leaderEntityIdp.first);
	
	// 이 리더의 팔로워 타입을 한 번만 선택
	const int index = random() % followerSidListSize;
	const ItemSid followerSid = followerSidList[index];
	
	// static_character에서 CharacterType 가져오기
	CharacterType followerType = CharacterType::WARRIOR; // 기본값
	std::shared_ptr<CharacterDocLookup> characterDocLookup = gStaticDataAccessor->Get<CharacterDocLookup>();
	if (characterDocLookup)
	{
		const CharacterDoc* characterDoc = characterDocLookup->Find(followerSid);
		if (characterDoc && characterDoc->mStaticCharacter)
		{
			followerType = characterDoc->mStaticCharacter->Get_c_character_type();
		}
	}
	
	// 모든 팔로워를 같은 타입으로 생성
	for (int i = 0; i < followerCount; ++i)
	{
		EntityDescription followerDescription = EntityUtil::DescriptEntityPcNpc(
			EntityDescriptorArg{
				.x = centerX,
				.y = centerY,
				.characterSid = followerSid,
				.characterType = followerType,
				.worldUserId = INVALID_WORLD_USER_ID,
				.spawnerIdp = INVALID_ENTITY_IDP,
				.commandX = 0,
				.commandY = 0
			}
		);
		
		// FollowerComponent 추가
		{
			auto c = EntityUtil::TakeComponent<FollowerComponent>(followerDescription);
			c->leaderEntityIdp = leaderEntityIdp;
		}		
		// NPC 구성 요소 추가: 동일 leaderEntityIdp를 spawnerIdp로 설정해 세력/그룹 기준으로 활용한다.
		{
			auto c = EntityUtil::TakeComponent<NpcComponent>(followerDescription);
			c->spawnerIdp = leaderEntityIdp;
		}

		entityList.push_back(followerDescription);
	}
	
	return entityList;
}
