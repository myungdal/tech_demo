// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemProjectile.h"

#include "Common/Stat/statContainer/StatContainer.h"
#include "MmoSync/Component/PositionComponent.h"
#include "MmoSync/Component/PhysicsComponent.h"
#include "MmoSync/Component/SkillComponent.h"
#include "MmoSync/Util/EntityUtil/EntityUtil.h"


SkillSystemProjectile::SkillSystemProjectile(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<SkillComponentProjectile>();
	IncludeComponent<PositionComponent>();
	IncludeComponent<PhysicsComponent>();
	IncludeComponent<TacticsComponentCombat>();
}
void SkillSystemProjectile::Update()
{
	if (Data().empty())
		return;

	const TimeStepUpdateCycle timeDelta = GetTimeStepUpdateCycle();

	for (EntityIdp entityIdp : Data())
	{
		PositionComponent& pos = GetComponent<PositionComponent>(entityIdp.first);
		PhysicsComponent& physics = GetComponent<PhysicsComponent>(entityIdp.first);
		SkillComponentProjectile& proj = GetComponent<SkillComponentProjectile>(entityIdp.first);

		// 새로운 생성하면 참조가 유효하지 않기 때문에 미리 필요한 값을 구한다
		const bool shouldDestroy = physics.IsIdle();
		const Scalar bombX = pos.x;
		const Scalar bombY = pos.y;

		// 새로운 생성하면 참조가 유효하지 않기 때문에 미리 할일을 한다
		if (shouldDestroy)
		{
			if (Entity* entity = GetEntity(entityIdp))
				entity->ReserveDestroy();
		}

		proj.elapsedBombTime += timeDelta;
		if (proj.bombInterval > 0 && proj.elapsedBombTime >= proj.bombInterval)
		{
			proj.elapsedBombTime -= proj.bombInterval;

			EntityDescription bombDesc = EntityUtil::DescriptEntityNormal(
				EntityDescriptorArg{
						.x = bombX,
						.y = bombY,
						.characterSid = 3,
						.characterType = CharacterType::BOMB,
						.worldUserId = INVALID_WORLD_USER_ID,
						.spawnerIdp = INVALID_ENTITY_IDP,
						.commandX = 0,
						.commandY = 0
				});
			MAYBE_UNUSED Entity* bomb = CreateEntityFromDescription(bombDesc);
		}
	}
}
