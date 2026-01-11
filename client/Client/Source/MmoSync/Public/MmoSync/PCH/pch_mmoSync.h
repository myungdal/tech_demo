// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/PCH/pch_common.h"
#include "Common/MemPool/CompactionPool.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Entity
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Entity;
using EntityPool = CompactionPool<Entity, EntityId>;

constexpr EntityIdp INVALID_ENTITY_IDP{ INVALID_ENTITY_ID , 0 };
//using EntityIdp = std::pair<EntityId, EntityProof>;
//constexpr EntityIdp INVALID_ENTITY_IDP = std::make_pair(INVALID_ENTITY_ID, EntityProof(0));
using EntityIdpList = std::vector<EntityIdp>;

struct EntityIdSortElem
{
	EntityIdp mEntityIdp{ INVALID_ENTITY_IDP };
	Scalar mX{ 0 };
	Scalar mY{ 0 };
};
using EntityIdSortList = std::vector<EntityIdSortElem>;

namespace EntityIdSortElemUtil
{
	inline Scalar CalcSquaredLengthX(const EntityIdSortElem& v0, const EntityIdSortElem& v1) noexcept
	{
		const Scalar d = (v0.mX - v1.mX);
		return d * d;
	}

	inline Scalar CalcSquaredLengthY(const EntityIdSortElem& v0, const EntityIdSortElem& v1) noexcept
	{
		const Scalar d = (v0.mY - v1.mY);
		return d * d;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// System
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using SystemGroupId = int8_t;
constexpr SystemGroupId INVALID_SYSTEM_GROUP_ID = -1;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Component
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using ComponentId = int16_t;
constexpr ComponentId INVALID_COMPONENT_ID = -1;
using ComponentTypeValue = std::bitset<MathUtil::AlignBitCountTo64(_IDX(ComponentType::MAX))>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Damage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using DamageInfoId = int16_t;
constexpr DamageInfoId INVALID_DAMAGE_INFO_ID = -1;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Iff
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class IffRelationType : uint8_t
{
	NONE = 0, // 관계 없음
	SELF = 1, // 동일 객체
	USER_VS_NPC = 2, // 유저 vs NPC인 경우
	SAME_SPECIES = 3, // 동일 종족
	CHAOS = 4, // 혼란 상태(무법자 등)
	HOSTILE = 5, // 적대적 관계
	PARTY_MEMBER = 6, // 파티 멤버
	GUILD_MEMBER = 7, // 길드 멤버
	ALLIED = 8, // 동맹
	ENEMY = 9, // 적대 관계
	MAX = 10
};
