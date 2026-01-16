// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DbSession.h"

import ServerEngineDb;


DbSession::DbSession(CommitType commitType, DbConnectionPool& dbConnectionPool)
	:
	DbSessionBase(commitType),
	mPool(dbConnectionPool)
{
	Acquire();
}
DbSession::~DbSession()
{
	Release();
}
DbConnection* DbSession::OnAcquire()
{
	return mPool.PopConn();
}
void DbSession::OnRelease(DbConnection* conn)
{
	mPool.PushConn(conn);
}
