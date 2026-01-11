// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


struct WorldUser;

// ============================================================================
// EntityKind: 엔티티의 종류를 명시적으로 지정
// ============================================================================
// PC/NPC 결정 정책:
// - Auto: worldUserId로 자동 결정 (기존 동작, INVALID이면 NPC)
// - Pc/Npc: 명시적으로 지정
// - Spawner/Spawnee: 해당 컴포넌트를 부착할지 여부 (PC/NPC와 조합 가능)
//
enum class EntityKind : uint8_t
{
	Auto,      // worldUserId가 INVALID이면 NPC, 아니면 PC (기존 동작)
	Pc,        // 강제로 PC로 설정
	Npc,       // 강제로 NPC로 설정
};

struct EntityDescriptorArg
{
	Scalar x = 0;
	Scalar y = 0;
	ItemSid characterSid = INVALID_SID;
	CharacterType characterType = CharacterType::NONE;
	WorldUserId worldUserId = INVALID_WORLD_USER_ID;
	EntityIdp spawnerIdp = INVALID_ENTITY_IDP;
	Scalar commandX = 0;
	Scalar commandY = 0;

	// 엔티티 종류 (기본: Auto - worldUserId로 결정)
	EntityKind kind = EntityKind::Auto;

	// PC/NPC 여부 판별 헬퍼
	bool IsPc() const
	{
		if (kind == EntityKind::Pc) return true;
		if (kind == EntityKind::Npc) return false;
		return worldUserId != INVALID_WORLD_USER_ID; // Auto: worldUserId로 결정
	}

	bool IsNpc() const { return !IsPc(); }
};
