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

#include "CheatManager.h"

CheatManager::CheatManager()
{
	mCheatDispatcher.Register(
		L"CreateItem", L"ItemSid ItemQuantity",
		std::bind(
			&CheatManager::CreateItem, this,
			std::placeholders::_1, 
			std::placeholders::_2, 
			std::placeholders::_3, 
			std::placeholders::_4, 
			std::placeholders::_5, 
			std::placeholders::_6
		)
	);
	mCheatDispatcher.Register(
		L"CreateMail", L"MailId MailSid RewardSid",
		std::bind(
			&CheatManager::CreateMail, this,
			std::placeholders::_1, 
			std::placeholders::_2, 
			std::placeholders::_3, 
			std::placeholders::_4, 
			std::placeholders::_5, 
			std::placeholders::_6
		)
	);
}
Result CheatManager::DispatchCommand(
	const wchar_t* command,
	SocketDbFromFront& frontSocket,
	DbUserSocketState& userSocketState,
	UserCacheAccessor& userCache,
	MainDbSession& mainDbSession,
	UserDbSession& userDbSession
)
{
	return mCheatDispatcher.Dispatch(command, frontSocket, userSocketState, userCache, mainDbSession, userDbSession);
}

// 치트 - 아이템 생성
Result CheatManager::CreateItem(
	ArgList& argList, 
	MAYBE_UNUSED SocketDbFromFront& frontSocket,
	MAYBE_UNUSED DbUserSocketState& userSocketState,
	MAYBE_UNUSED UserCacheAccessor& userCache,
	MAYBE_UNUSED MainDbSession& mainDbSession,
	MAYBE_UNUSED UserDbSession& userDbSession
)
{
	const ItemSid itemSid = argList.GetInt<ItemSid>(1, INVALID_SID);
	const ItemQuantity itemQuantity = argList.GetInt<ItemQuantity>(2, ItemQuantity(0));

	std::shared_ptr<ItemDocLookup> itemDocLookup = gStaticDataAccessor->Get<ItemDocLookup>();
	const ItemDoc* itemDoc = itemDocLookup->Find(itemSid);

	if (nullptr == itemDoc)
		return Result::INVALID_REQ;

	const STATIC_ITEM* staticItem = itemDoc->mStaticItem;
	return ItemUtil::CreateItem(userDbSession, userCache, staticItem, itemQuantity);
}

// 치트 - 우편 생성
Result CheatManager::CreateMail(
	ArgList& argList,
	MAYBE_UNUSED SocketDbFromFront& frontSocket,
	MAYBE_UNUSED DbUserSocketState& userSocketState,
	MAYBE_UNUSED UserCacheAccessor& userCache,
	MAYBE_UNUSED MainDbSession& mainDbSession,
	MAYBE_UNUSED UserDbSession& userDbSession
)
{
	const MailId mailId = argList.GetInt<MailId>(1, INVALID_UUID);
	const ItemSid mailSid = argList.GetInt<ItemSid>(2, INVALID_SID);
	const int64_t rewardSid = argList.GetInt<int64_t>(3, 0);

	if (INVALID_UUID == mailId)
		return Result::INVALID_REQ;

	USER& user = userCache.GetUser();
	const UserId userId = user.Get_c_user_id();
	if (INVALID_UUID == userId)
		return Result::INVALID_REQ;

	const wchar_t* title = L"[BOT] TEST MAIL";
	const wchar_t* msg = L"BOT created mail for regression test.";
	const wchar_t* msgArgs = L"";

	const time_t nowTt = tClock.GetGlobalNowTt();
	const TIMESTAMP_STRUCT nowTs = tClock.GetGlobalNowTs();
	const TIMESTAMP_STRUCT expireTs = TimeUtil::TS_FROM_TT(nowTt + 7 * 24 * 60 * 60);

	SP_MAIL_INSERT spMailInsert(
		userDbSession,
		mailId.GetData(),
		userId.GetData(),
		mailSid,
		title,
		msg,
		msgArgs,
		rewardSid,
		expireTs,
		nowTs
	);

	return DbUtil::ExecuteSp(spMailInsert);
}

