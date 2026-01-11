// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Character/RecoverySystem.h"

#include "Common/Stat/statContainer/StatContainer.h"

#include "MmoSync/Util/PhysicsUtil/PhysicsUtil.h"


RecoverySystem::RecoverySystem(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<CharacterComponent>();
	IncludeComponent<AbilityComponentRecovery>();
	// 전투 상태가 아닌 엔티티만 회복 처리
	// TacticsComponentCombat 제외
	//ExcludeComponent<TacticsComponentCombat>();

	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
}

void RecoverySystem::Update()
{
	if (Data().empty())
		return;

	for (EntityIdp entityIdp : Data())
	{
#if defined(UE_EDITOR) || defined(_DEBUG)
		if (entityIdp.first == DebugInfo::DEBUG_ENTITY_ID)
			__nop();
#endif

		CharacterComponent& character0 = GetComponent<CharacterComponent>(entityIdp.first);
		
		if (StatContainer* statContainer0 = character0.GetStatContainer())
		{
			const StatValue currHp = statContainer0->GetStatValue(StatType::HP);
			const StatValue maxHp = statContainer0->GetStatValue(StatType::HP_MAX);
			const StatValue increaseHp = (StatValue)std::min((maxHp - currHp), (StatValue)DebugInfo::RECOVERY_DEFAULT_HEAL);

			if (increaseHp > 0)
				statContainer0->IncreaseStatValue(StatType::HP, increaseHp);

		}
	}
}
