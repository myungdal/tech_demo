// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;
import ServerEngineDb;

#include "DbUserManager.h"


DbUserPtr DbUserManager::FindUser(const UserId userId)
{
	ReadLock lock(mLock);
	auto it = mUserDbMap.find(userId);

	if (mUserDbMap.end() != it)
		return it->second; 

	return nullptr;
}

DbUserPtr DbUserManager::CreateUser(DbSessionBase& dbSession, const ACCOUNT_USER& accountUser)
{
	DbUserPtr dbUser = std::make_shared<DbUser>(accountUser.Get_c_user_id());
	if (true == LoadUser(dbSession, dbUser, accountUser))
		return dbUser;

	{
		SP_USER_INSERT spUserInsert(dbSession,
			accountUser.Get_c_account_id(),
			accountUser.Get_c_user_id(),
			UserExp(0),
			UserExp(0),
			tClock.GetGlobalNowTs()
		);
		Result result = DbUtil::ExecuteSp(spUserInsert);
		if (Result::SUCCEEDED != result)
			return nullptr;
	}

	{
		SP_ITEM_INSERT spItemInsert(dbSession,
			accountUser.Get_c_user_id(), // p_user_id
			accountUser.Get_c_user_id(), // p_new_item_id (User ID와 동일하게)
			INVALID_UUID,
			INVALID_UUID, // p_parent_mail_id
			INVALID_SID, // p_item_sid
			0, // p_inventory_slot_index
			0, // p_quick_slot_index
			0, // p_equip_slot_index
			0, // p_item_quantity
			0, // p_item_level
			0, // p_item_color_union
			0, // p_item_stat_union
			0, // p_item_enchant_union
			(int64_t)ItemStateFlag::NONE, // p_item_state_flag
			TimeUtil::TS_FROM_TT(0), // p_date_cooltime
			TimeUtil::TS_FROM_TT(0), // p_date_expire
			tClock.GetGlobalNowTs(), // p_date_updated
			tClock.GetGlobalNowTs()  // p_date_created
		);
		Result result = DbUtil::ExecuteSp(spItemInsert);
		if (Result::SUCCEEDED != result)
			return nullptr;
	}

	if (false == LoadUser(dbSession, dbUser, accountUser))
		return nullptr;

	return dbUser;
}

bool DbUserManager::LoadUser(DbSessionBase& dbSession, const DbUserPtr& dbUser, const ACCOUNT_USER& accountUser)
{
	DbUserContext userContext = dbUser->GetContext();
	//DbUserSocketState& userSocketState = userContext.GetUserSocketState();
	UserCacheAccessor& userCache = userContext.GetUserCache();
	//UserCacheDiff& userCacheDiff = userContext.GetUserCacheDiff();

	const UserId userId = dbUser->GetUserId();

	{
		UserAchievementTablePtr table = userCache.Get<UserAchievementTable>();
		table->Clear();

		SP_ACHIEVEMENT_SELECT sp(dbSession, userId);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				ACHIEVEMENT::Writer achievement = DbUtil::SpToPacket(sp);
				UserAchievementRowPtr row = std::make_shared<UserAchievementRow>(
					PacketKeep<ACHIEVEMENT>(achievement.GetPacket())
				);
				table->Upsert(row);
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		UserItemTablePtr table = userCache.Get<UserItemTable>();
		table->Clear();

		SP_ITEM_SELECT_BY_USER sp(dbSession, userId);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				ITEM::Writer item = DbUtil::SpToPacket(sp);
				UserItemRowPtr row = std::make_shared<UserItemRow>(
					PacketKeep<ITEM>(item.GetPacket()),
					nullptr
				);
				table->Upsert(row);
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		UserItemStatTablePtr table = userCache.Get<UserItemStatTable>();
		table->Clear();

		SP_ITEM_STAT_SELECT_BY_USER sp(dbSession, userId);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				ITEM_STAT::Writer itemStat = DbUtil::SpToPacket(sp);
				UserItemStatRowPtr row = std::make_shared<UserItemStatRow>(
					PacketKeep<ITEM_STAT>(itemStat.GetPacket())
				);
				table->Upsert(row);
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_USER_SELECT sp(dbSession, userId);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				USER::Writer user = DbUtil::SpToPacket(sp);
				userCache.SetUser(
					accountUser,
					user.GetPacket()
				);
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;

		if (userCache.IsEmpty())
			return false;
	}

	{
		WriteLock lock(mLock);

		mUserDbMap.emplace(userId, dbUser);
	}

	return true;
}

