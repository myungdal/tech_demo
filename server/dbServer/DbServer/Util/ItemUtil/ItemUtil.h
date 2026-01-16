// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class STATIC_ITEM;
class UserDbSession;
class UserCacheAccessor;

namespace ItemUtil
{
	Result CreateItem(UserDbSession& userDbSession, UserCacheAccessor& userCache, const STATIC_ITEM* staticItem, ItemQuantity itemQuantity);
	Result DeleteItem(UserDbSession& userDbSession, UserCacheAccessor& userCache, const ItemId itemId, ItemQuantity itemQuantity);
	void TrimInventory(UserDbSession& userDbSession, UserCacheAccessor& UserCacheAccessor);
}
