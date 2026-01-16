// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/ComponentRegisterer/Components.h"
#include "MmoSync/WorldManager/WorldUserRegistry/WorldUser.h"
#include "MmoSync/Engine/Entity/Entity.h"


struct WorldIffChecker
{
	template <typename _System>
	static bool IsFriend(
		MAYBE_UNUSED const _System& system, 
		const EntityIdp& idp0, 
		const EntityIdp& idp1, 
		const PcComponent* pc0, 
		const NpcComponent* npc0,
		const PcComponent* pc1,
		const NpcComponent* npc1
	) noexcept
	{
		if (idp0 == idp1)
			return true;

		// ========================================================================
		// 같은 리더를 따르는 팔로워들은 친구 (PC/NPC 구분 없이)
		// ========================================================================
		const FollowerComponent* follower0 = system.TryGetComponent<FollowerComponent>(idp0);
		const FollowerComponent* follower1 = system.TryGetComponent<FollowerComponent>(idp1);
		const LeaderComponent* leader0 = system.TryGetComponent<LeaderComponent>(idp0);
		const LeaderComponent* leader1 = system.TryGetComponent<LeaderComponent>(idp1);

		// 둘 다 같은 리더를 따르는 팔로워
		if (follower0 && follower1)
		{
			if (follower0->leaderEntityIdp == follower1->leaderEntityIdp &&
				follower0->leaderEntityIdp != INVALID_ENTITY_IDP)
				return true;
		}

		// 하나는 리더, 하나는 그 리더를 따르는 팔로워
		if (leader0 && follower1)
		{
			if (follower1->leaderEntityIdp == idp0)
				return true;
		}
		if (follower0 && leader1)
		{
			if (follower0->leaderEntityIdp == idp1)
				return true;
		}

		// ========================================================================
		// PC 간 친구 관계 (같은 worldUserId)
		// ========================================================================
		if (pc0 && pc1)
		{
			return (pc0->worldUserId == pc1->worldUserId);
		}

		// ========================================================================
		// NPC 간 친구 관계 (같은 spawner)
		// ========================================================================
		if (npc0 && npc1)
		{
			// SpawneeComponent 또는 SpawnerComponent 확인
			const SpawneeComponent* spawnee0 = system.TryGetComponent<SpawneeComponent>(idp0);
			const SpawnerComponent* spawner0 = system.TryGetComponent<SpawnerComponent>(idp0);
			const SpawneeComponent* spawnee1 = system.TryGetComponent<SpawneeComponent>(idp1);
			const SpawnerComponent* spawner1 = system.TryGetComponent<SpawnerComponent>(idp1);
			
			const bool hasSpawnRelation0 = (spawnee0 != nullptr || spawner0 != nullptr);
			const bool hasSpawnRelation1 = (spawnee1 != nullptr || spawner1 != nullptr);
			
			// 둘 다 spawn 관계가 없으면 서로 친구 (적이 아님)
			if (!hasSpawnRelation0 && !hasSpawnRelation1)
				return true;
			
			// 기존 로직: 같은 spawner에서 나온 경우 친구
			return (npc0->spawnerIdp == npc1->spawnerIdp);
		}

		return false;
	}
};
