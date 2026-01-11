// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerBase/Worker.h"


class ACCOUNT_USER;
class DbSessionBase;

class DbUserManager : public Worker
{
private:
	Lock mLock;

private:
	using UserDbMap = std::unordered_map<UserId, DbUserPtr>;

	UserDbMap mUserDbMap;

public:
	DbUserPtr FindUser(const UserId userId);
	DbUserPtr CreateUser(DbSessionBase& dbSession, const ACCOUNT_USER& accountUser);

public:
	bool LoadUser(DbSessionBase& dbSession, const DbUserPtr& dbUser, const ACCOUNT_USER& accountUser);
};

inline std::shared_ptr<DbUserManager> gDbUserManager = nullptr;
