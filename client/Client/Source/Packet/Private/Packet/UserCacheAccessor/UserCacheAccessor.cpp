// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCacheAccessor/UserCacheAccessor.h"

#include "Packet/UserCache/UserAchievement/UserAchievementTable.h"
#include "Packet/UserCache/UserCharacter/UserCharacterTable.h"
#include "Packet/UserCache/UserItem/UserItemTable.h"
#include "Packet/UserCache/UserItemStat/UserItemStatTable.h"
#include "Packet/UserCache/UserMission/UserMissionTable.h"
#include "Packet/UserCache/UserQuest/UserQuestTable.h"


UserCacheAccessor::UserCacheAccessor()
{
	size_t idx = 0;
	UserCacheTableTypes types; // 객체들이 생성과 소멸이 된다는 점에 주의.
	ForEach(types,
		[this, &idx](auto& type)
		{
			RegisterTable<decltype(type)>(idx++);
		}
	);
}
void UserCacheAccessor::SetUser(const ACCOUNT_USER& accountUser, const USER& user)
{
	mAccountUser = PacketKeep<ACCOUNT_USER>(accountUser);
	mUser = PacketKeep<USER>(user);
}

void UserCacheAccessor::ClearUserCacheTableList()
{
	size_t idx = 0;
	UserCacheTableTypes types; // 객체들이 생성과 소멸이 된다는 점에 주의.
	ForEach(types,
		[this, &idx](auto& type)
		{
			auto ptr = Get<decltype(type)>();
			ptr->Clear();
		}
	);
}
