// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/DbAndPacketTransactor.h"


class DbUser;
class DbUserContext;
class DbUserSocketState;
class UserCacheAccessor;
class UserCacheDiff;

class UserAndPacketTransactor : public DbAndPacketTransactor
{
private:
	DbUser& mDbUser;

	std::shared_ptr<DbUserContext> mUserContext = nullptr;

	DbUserSocketState& mUserSocketState;
	UserCacheAccessor& mUserCacheAccessor;
	UserCacheDiff& mUserCacheDiff;

	USER_CACHE_DIFF::Writer mUserCacheDiffWp;

public:
	explicit UserAndPacketTransactor(
		DbUser& dbUser, DbUserContext& userContext, PacketHeader& packetHeader
	);

protected:
	UserId GetUserId() const;
	DbSeq AddDbSeq();

protected:
	DbUserContext& GetUserContext() { return *mUserContext; }

	DbUserSocketState& GetUserSocketState() { return mUserSocketState; }
	UserCacheAccessor& GetUserCache() { return mUserCacheAccessor; }
	UserCacheDiff& GetUserCacheDiff() { return mUserCacheDiff; }

	const USER_CACHE_DIFF::Writer& GetUserCacheDiffWp() const { return mUserCacheDiffWp; }	

protected:
	Result OnPreValidate() override;
	void OnPreUpdate() override;
	bool OnCommit() override;
	bool OnRollback() override;
};

