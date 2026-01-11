// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "UserDbSession.h"

import ServerEngineDb;


UserDbSession::UserDbSession(CommitType commitType, DbShardIdx shardIdx)
	:	
	DbSessionBase(commitType),
	mDbShardIdx(shardIdx)
{
	Acquire();
}
UserDbSession::~UserDbSession()
{
	Release();
}
DbConnection* UserDbSession::OnAcquire()
{
	return gUserDbConnPool->PopDbConn(mDbShardIdx);
}
void UserDbSession::OnRelease(DbConnection* conn)
{
	gUserDbConnPool->PushDbConn(conn);
}
