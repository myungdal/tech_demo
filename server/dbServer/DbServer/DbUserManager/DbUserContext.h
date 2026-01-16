// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class DbUserSocketState;
class UserCacheAccessor;
class UserCacheDiff;

class DbUserContext
{
private:
	WriteLock& mLock;
	DbUserSocketState& mUserSocketState;
	UserCacheAccessor& mUserCacheAccessor;
	UserCacheDiff& mUserCacheDiff;

public:
	explicit DbUserContext(
		WriteLock& lock,
		DbUserSocketState& userSocketState,
		UserCacheAccessor& userCache,
		UserCacheDiff& userCacheDiff
	)
		:
		mLock(lock),
		mUserSocketState(userSocketState),
		mUserCacheAccessor(userCache),
		mUserCacheDiff(userCacheDiff)
	{
	}

public:
	DbUserSocketState& GetUserSocketState() { return mUserSocketState; }
	UserCacheAccessor& GetUserCache() { return mUserCacheAccessor; }
	UserCacheDiff& GetUserCacheDiff() { return mUserCacheDiff; }
};
