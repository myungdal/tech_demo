// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Db/DbSessionBase/DbSessionBase.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UserDbSession
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UserDbSession final : public DbSessionBase
{
private:
	DbShardIdx mDbShardIdx = INVALID_DB_SHARD_IDX;

public:
	explicit UserDbSession(CommitType commitType, DbShardIdx shardIdx);
	virtual ~UserDbSession();

protected:
	DbConnection* OnAcquire() final;
	void OnRelease(DbConnection* conn) final;
};
