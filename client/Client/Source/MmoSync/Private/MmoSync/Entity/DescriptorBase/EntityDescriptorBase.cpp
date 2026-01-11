// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Entity/DescriptorBase/EntityDescriptorBase.h"

#include "MmoSync/ComponentRegisterer/Components.h"
#include "MmoSync/WorldManager/WorldUserRegistry/WorldUser.h"
#include "MmoSync/Util/EntityUtil/EntityUtil.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"


// [TODO] DescriptCharacterBase는 책임이 모호하므로 EntityDescriptor_*로 이동/통합하는 방향으로 정리한다.
void EntityDescriptorBase::DescriptCharacterBase(OUT EntityDescription& entityDescription, const EntityDescriptorArg& arg) const
{
	{
		MAYBE_UNUSED std::shared_ptr<PositionComponent> c = EntityUtil::TakeComponent<PositionComponent>(entityDescription);
		c->x = arg.x;
		c->y = arg.y;
	}
	{
		MAYBE_UNUSED std::shared_ptr<CharacterComponent> c = EntityUtil::TakeComponent<CharacterComponent>(entityDescription);
		c->characterSid = arg.characterSid;
	}
	{
		MAYBE_UNUSED std::shared_ptr<PhysicsComponent> c = EntityUtil::TakeComponent<PhysicsComponent>(entityDescription);

		// homeX/homeY 초기화 (복귀 기준 좌표, 불변)
		if (arg.commandX)
		{
			c->homeX = arg.commandX;
			c->homeY = arg.commandY;
			// 초기 명령이 있으면 home으로 이동 시작 (생성 시점이므로 isPcLeader = false)
			c->SetCommand(false, arg.x, arg.y, MoveCommandLayer::TACTICS, c->homeX, c->homeY, DebugInfo::MOVE_LAYER_TACTICS_MAX_LIFETIME, DebugInfo::MOVE_LAYER_TACTICS_MIN_HOLD);
		}
		else
		{
			c->homeX = arg.x;
			c->homeY = arg.y;
			// 초기 명령이 없으면 제자리 스폰 (TACTICS 슬롯은 NONE 유지)
		}
	}
	{
		MAYBE_UNUSED std::shared_ptr<AbilityComponentRecovery> c = EntityUtil::TakeComponent<AbilityComponentRecovery>(entityDescription);
	}
}

void EntityDescriptorBase::DescriptCharacterPcNpc(OUT EntityDescription& entityDescription, const EntityDescriptorArg& arg) const
{
	// EntityKind 또는 worldUserId로 PC/NPC 결정
	if (arg.IsNpc())
	{
		MAYBE_UNUSED std::shared_ptr<NpcComponent> c = EntityUtil::TakeComponent<NpcComponent>(entityDescription);
		c->spawnerIdp = arg.spawnerIdp;
	}
	else
	{
		MAYBE_UNUSED std::shared_ptr<PcComponent> c = EntityUtil::TakeComponent<PcComponent>(entityDescription);
		c->worldUserId = arg.worldUserId;
	}
}

void EntityDescriptorBase::DescriptCharacterSpawner(OUT EntityDescription& entityDescription, const EntityDescriptorArg& arg) const
{
	{
		// 스포너는 진형과 물리이동이 없다
		std::erase_if(entityDescription, [](const ComponentDescription& desc) {
			return
				desc.mComponentType == FormationComponentCrowd::GetComponentType() ||
				desc.mComponentType == PhysicsComponent::GetComponentType() ||
				desc.mComponentType == DetectionComponentPartner::GetComponentType();
			});
	}
	{
		MAYBE_UNUSED std::shared_ptr<AbilityComponentGridBlock> c = EntityUtil::TakeComponent<AbilityComponentGridBlock>(entityDescription);
	}
	//{
	//	MAYBE_UNUSED std::shared_ptr<AbilityComponentUntargetrable> c = EntityUtil::TakeComponent<AbilityComponentUntargetrable>(entityDescription);
	//}
	{
		MAYBE_UNUSED std::shared_ptr<SpawnerComponent> c = EntityUtil::TakeComponent<SpawnerComponent>(entityDescription);
		c->defaultCommandX = arg.commandX;
		c->defaultCommandY = arg.commandY;
	}
}

void EntityDescriptorBase::DescriptCharacterSpawnee(OUT EntityDescription& entityDescription, MAYBE_UNUSED const EntityDescriptorArg& arg) const
{
	{
		MAYBE_UNUSED std::shared_ptr<SpawneeComponent> c = EntityUtil::TakeComponent<SpawneeComponent>(entityDescription);
		c->spawnerIdp = arg.spawnerIdp;
	}
	// NpcComponent는 DescriptCharacterPcNpc에서 처리됨 (worldUserId가 INVALID일 때만)
	// PC Spawnee의 경우 NpcComponent를 추가하지 않음 (PcComponent만 가짐)
}

void EntityDescriptorBase::DescriptCharacterArrivalMotion(OUT EntityDescription& entityDescription, MAYBE_UNUSED const EntityDescriptorArg& arg) const
{
	{
		MAYBE_UNUSED std::shared_ptr<AbilityComponentUntargetrable> c = EntityUtil::TakeComponent<AbilityComponentUntargetrable>(entityDescription);
	}
	{
		MAYBE_UNUSED std::shared_ptr<MotionComponentArrival> c = EntityUtil::TakeComponent<MotionComponentArrival>(entityDescription);
		c->duration = DebugInfo::MOTION_DURATION_ARRIVAL;
	}
}
