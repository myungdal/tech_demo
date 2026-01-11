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
// StaticDbSession
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class StaticDbSession final : public DbSessionBase
{
public:
	explicit StaticDbSession(SQLHENV env, CommitType commitType);
	virtual ~StaticDbSession();
};
