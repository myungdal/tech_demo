// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;
import ServerEnginePacket;

#include "BotContext.h"


BotContext::BotContext()
{
	mUserCacheAccessor = std::make_shared<UserCacheAccessor>();
}
UserItemRowPtr BotContext::FindItemIdByItemSid(ItemSid itemSid) const
{
	if (UserItemTablePtr userItemTable = mUserCacheAccessor->Get<UserItemTable>())
	{
		return userItemTable->FindItemByItemSid(itemSid);
	}
	return nullptr;
}
