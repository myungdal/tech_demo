// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Component/CharacterComponent.h"
#include "Common/Stat/StatContainer/StatContainer.h"

namespace CharacterUtil
{
	inline void FillHpToMax(CharacterComponent& character)
	{
		StatContainer* stat = character.GetStatContainer();
		if (!stat)
			return;

		const StatValue hp = stat->GetStatValue(StatType::HP);
		const StatValue hpMax = stat->GetStatValue(StatType::HP_MAX);
		if (hp < hpMax)
			stat->IncreaseStatValue(StatType::HP, hpMax - hp);
	}
}

