// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbSession.h - DB 세션 (커넥션 풀 기반)
#pragma once

#include "ServerEngine/Db/DbSessionBase/DbSessionBase.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DbSession
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DB 세션 (커넥션 풀에서 커넥션 획득/반환)
class DbSession : public DbSessionBase
{
private:
	// 사용할 커넥션 풀
	DbConnectionPool& mPool;

public:
	// 생성자 (커밋 타입 및 커넥션 풀 지정)
	explicit DbSession(CommitType commitType, DbConnectionPool& dbConnectionPool);
	
	// 소멸자
	virtual ~DbSession() override;

protected:
	// 커넥션 획득 (풀에서 Pop)
	DbConnection* OnAcquire() override final;
	
	// 커넥션 반환 (풀로 Push)
	void OnRelease(DbConnection* conn) override final;
};
