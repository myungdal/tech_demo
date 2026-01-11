// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "LogDbSession.h"

import ServerEngineDb;


LogDbSession::LogDbSession(CommitType commitType)
	:
	DbSessionBase(commitType)
{
	Acquire();
}
LogDbSession::~LogDbSession()
{
	Release();
}
DbConnection* LogDbSession::OnAcquire()
{
	return gLogDbConnPool->PopDbConn();
}
void LogDbSession::OnRelease(DbConnection* conn)
{
	gLogDbConnPool->PushDbConn(conn);
}
