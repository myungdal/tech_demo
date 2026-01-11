// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Entity/DescriptorBase/EntityDescriptorArg.h"
#include "MmoSync/Entity/Description/EntityDescription.h"


// EntityDescriptorBase - 엔티티 생성 설명자 기반 클래스
// - 엔티티 생성 시 필요한 컴포넌트 구성 정의
// - 캐릭터 타입별로 파생 클래스 구현 (ARCHER, WARRIOR 등)
class EntityDescriptorBase
{
public:
	// 캐릭터 기본 컴포넌트 설정 (Position, Motion, Physics 등)
	void DescriptCharacterBase(OUT EntityDescription& entityDescription, const EntityDescriptorArg& arg) const;

public:
	// PC/NPC 구분 컴포넌트 설정
	void DescriptCharacterPcNpc(OUT EntityDescription& entityDescription, const EntityDescriptorArg& arg) const;

public:
	// 캐릭터 타입별 고유 설정 (파생 클래스에서 구현)
	virtual void DescriptCharacterType(OUT EntityDescription& entityDescription, MAYBE_UNUSED const EntityDescriptorArg& arg) const = 0;

public:
	// Spawner 컴포넌트 설정 (소환자)
	void DescriptCharacterSpawner(OUT EntityDescription& entityDescription, const EntityDescriptorArg& arg) const;

	// Spawnee 컴포넌트 설정 (소환된 엔티티)
	void DescriptCharacterSpawnee(OUT EntityDescription& entityDescription, MAYBE_UNUSED const EntityDescriptorArg& arg) const;

public:
	// 도착 모션 설정
	void DescriptCharacterArrivalMotion(OUT EntityDescription& entityDescription, MAYBE_UNUSED const EntityDescriptorArg& arg) const;
};
