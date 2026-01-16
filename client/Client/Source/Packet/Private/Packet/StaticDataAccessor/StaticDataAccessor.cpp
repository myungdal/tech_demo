// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/StaticDataAccessor/StaticDataAccessor.h"

#include "Packet/StaticData/Achievement/AchievementDocLookup.h"
#include "Packet/StaticData/Building/BuildingDocLookup.h"
#include "Packet/StaticData/Character/CharacterDocLookup.h"
#include "Packet/StaticData/CharacterSkill/CharacterSkillDocLookup.h"
#include "Packet/StaticData/Game/GameDocLookup.h"
#include "Packet/StaticData/Item/ItemDocLookup.h"
#include "Packet/StaticData/ItemStat/ItemStatDocLookup.h"
#include "Packet/StaticData/Map/MapDocLookup.h"
#include "Packet/StaticData/Mission/MissionDocLookup.h"
#include "Packet/StaticData/Quest/QuestDocLookup.h"
#include "Packet/StaticData/Reward/RewardDocLookup.h"
#include "Packet/StaticData/RewardGroup/RewardGroupDocLookup.h"
#include "Packet/StaticData/Stat/StatDocLookup.h"
#include "Packet/StaticData/UserExp/UserExpDocLookup.h"


StaticDataAccessor::StaticDataAccessor()
{
	size_t idx = 0;
	StaticViewTypes types; // 객체들이 생성과 소멸이 된다는 점에 주의.
	ForEach(types,
		[this, &idx](auto& type)
		{
			RegisterView<decltype(type)>(idx++);
		}
	);
}

void StaticDataAccessor::BuildView()
{
	for (const StaticDataViewPtr& view : mStaticDataViewList)
	{
		view->Clear();
	}

	for (const StaticDataViewPtr& view : mStaticDataViewList)
	{
		view->Load();
	}

	for (const StaticDataViewPtr& view : mStaticDataViewList)
	{
		view->Link();
	}
}

#if defined(__UNREAL__)
PACKET_API TSharedPtr<StaticDataAccessor> gStaticDataAccessor = nullptr;
#endif
