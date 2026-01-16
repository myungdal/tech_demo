// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;

#include "DbUser.h"


DbUser::DbUser(UserId userId)
	:
	mUserId(userId),
	mUserSocketState(std::make_shared<DbUserSocketState>()),
	mUserCacheAccessor(std::make_shared<UserCacheAccessor>()),
	mUserCacheDiff(std::make_shared<UserCacheDiff>(mUserCacheAccessor))
{
}
DbUserContext DbUser::GetContext()
{
	WriteLock lock(mLock);
	// 재화 변화를 구하기 위해 체크아웃.
	mUserCacheDiff->Checkout();

	DbUserContext userContext(lock, *mUserSocketState, *mUserCacheAccessor, *mUserCacheDiff);
	return userContext;
}

