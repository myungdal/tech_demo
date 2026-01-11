// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_SYSTEM_KEEP_ALIVE : public DbSp
{
public:

	struct Out
	{
	} mOut;

public:
	SP_SYSTEM_KEEP_ALIVE(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_system_keep_alive()}", session)
	{
       if (false == session.IsReady())
           return;



	}
	~SP_SYSTEM_KEEP_ALIVE()
	{
	}
};

