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
// MainDbSession
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MainDbSession final : public DbSessionBase
{
public:
	explicit MainDbSession(CommitType commitType);
	virtual ~MainDbSession();

protected:
	DbConnection* OnAcquire() final;
	void OnRelease(DbConnection* conn) final;
};
