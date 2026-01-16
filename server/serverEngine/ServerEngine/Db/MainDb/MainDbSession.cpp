// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MainDbSession.h"

import ServerEngineDb;


MainDbSession::MainDbSession(CommitType commitType)
	:
	DbSessionBase(commitType)
{
	Acquire();
}
MainDbSession::~MainDbSession()
{
	Release();
}
DbConnection* MainDbSession::OnAcquire()
{
	return gMainDbConnPool->PopDbConn();
}
void MainDbSession::OnRelease(DbConnection* conn)
{
	gMainDbConnPool->PushDbConn(conn);
}
