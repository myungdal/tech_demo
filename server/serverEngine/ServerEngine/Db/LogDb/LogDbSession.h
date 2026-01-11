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
// LogDbSession
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LogDbSession final : public DbSessionBase
{
public:
	explicit LogDbSession(CommitType commitType);
	virtual ~LogDbSession();

protected:
	DbConnection* OnAcquire() final;
	void OnRelease(DbConnection* conn) final;
};
