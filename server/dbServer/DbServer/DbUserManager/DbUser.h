// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerBase/Worker.h"


class DbUserSocketState;
class UserCacheAccessor;
class UserCacheDiff;
class DbUserContext;

class DbUser : public Worker
{
private:
	Lock mLock;

private:
	UserId mUserId = INVALID_UUID;
	std::shared_ptr<DbUserSocketState> mUserSocketState = nullptr;
	std::shared_ptr<UserCacheAccessor> mUserCacheAccessor = nullptr;
	std::shared_ptr<UserCacheDiff> mUserCacheDiff = nullptr;

public:
	explicit DbUser(UserId userId);

public:
	UserId GetUserId() const { return mUserId; }
	DbUserContext GetContext();
};

