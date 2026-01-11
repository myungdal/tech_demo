// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ServerEngine/PCH/pch_serverEngine.h"
#include "LogDbConnPool.h"

// [WHY] 우산 모듈 import는 C1605(obj 4GB 초과)로 빌드 실패 가능.
// 필요한 헤더만 직접 include한다.
#include "ServerEngine/Worker/WorkerManager.h"
#include "ServerEngine/Worker/WorkerImpl/TimerWorker.h"
#include "ServerEngine/Db/DbConnection/DbConnection.h"
#include "ServerEngine/Db/DbConnection/DbConnectionPool.h"
#include "ServerEngine/AppConfigManager/AppConfigManager.h"
#include "ServerEngine/Util/StringUtil/StringUtil.h"


LogDbConnPool::LogDbConnPool(SQLHENV env, int dbPoolSize)
	:
	mDbPoolSize(dbPoolSize)
{
	for (const DbConfigData& dbConfigData : gAppConfigManager->GetDbConfigList())
	{
		if (DbType::LOG != dbConfigData.mDbType)
			continue;
		auto [connStr, connStrLen] = StringUtil::FormatStr(
			L"Driver={{MySQL ODBC 8.0 Unicode Driver}};Server={};Port={};Database={};UID={};PWD={};OPTION=3;",
			dbConfigData.mDbIp,
			dbConfigData.mDbPort,
			dbConfigData.mDbName,
			dbConfigData.mDbUser,
			dbConfigData.mDbPwd
		);

		// [WHY] LOG DB 설정은 1개만 사용한다. 첫 번째 LOG 설정을 찾으면 풀을 생성하고 종료한다.
		mPool = std::make_shared<DbConnectionPool>(env);

		mPool->CreateConnection(*connStr, dbPoolSize);

		break;
	}

	WorkerManager::RunOnTimer(
		DbConnection::CHECK_KEEP_ALIVE_INTERVAL, gTimerWorker->GetTimerWorker(),
		this, &LogDbConnPool::KeepAlive_timer
	);
}

LogDbConnPool::~LogDbConnPool()
{
}

void LogDbConnPool::KeepAlive_timer()
{
	if (DbConnection* dbConn = mPool->PopConn())
	{
		if (true == dbConn->HasError())
		{
			dbConn->Connect();
		}
		else
		{
			dbConn->KeepAlive();
		}

		mPool->PushConn(dbConn);
	}

	WorkerManager::RunOnTimer(
		DbConnection::CHECK_KEEP_ALIVE_INTERVAL, gTimerWorker->GetTimerWorker(), 
		this, &LogDbConnPool::KeepAlive_timer
	);
}

DbConnection* LogDbConnPool::PopDbConn()
{	
	if (!mPool)
		return nullptr;
	
	if (DbConnection* conn = mPool->PopConn())
	{
		// 연결 오류가 있으면 재연결한다.
		if (true == conn->HasError())
			conn->Connect();

		return conn;
	}
	
	return nullptr;
}

void LogDbConnPool::PushDbConn(DbConnection* conn)
{
	return mPool->PushConn(conn);
}

