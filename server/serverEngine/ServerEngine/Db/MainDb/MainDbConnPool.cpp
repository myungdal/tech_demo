// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ServerEngine/PCH/pch_serverEngine.h"
#include "MainDbConnPool.h"

// 모듈 사용 방지 (C1605: 4GB 초과)
// 필요한 것만 직접 include
#include "ServerEngine/Worker/WorkerManager.h"
#include "ServerEngine/Worker/WorkerImpl/TimerWorker.h"
#include "ServerEngine/Db/DbConnection/DbConnection.h"
#include "ServerEngine/Db/DbConnection/DbConnectionPool.h"
#include "ServerEngine/AppConfigManager/AppConfigManager.h"
#include "ServerEngine/Util/StringUtil/StringUtil.h"


MainDbConnPool::MainDbConnPool(SQLHENV env, int dbPoolSize)
	:
	mDbPoolSize(dbPoolSize)
{
	for (const DbConfigData& dbConfigData : gAppConfigManager->GetDbConfigList())
	{
		if (DbType::MAIN != dbConfigData.mDbType)
			continue;
		auto [connStr, connStrLen] = StringUtil::FormatStr(
			L"Driver={{MySQL ODBC 8.0 Unicode Driver}};Server={};Port={};Database={};UID={};PWD={};OPTION=3;",
			dbConfigData.mDbIp,
			dbConfigData.mDbPort,
			dbConfigData.mDbName,
			dbConfigData.mDbUser,
			dbConfigData.mDbPwd
		);

		// 메인 DB를 찾으면 하나의 풀만 생성하고 break
		mPool = std::make_shared<DbConnectionPool>(env);

		mPool->CreateConnection(*connStr, dbPoolSize);

		break;
	}

	WorkerManager::RunOnTimer(
		DbConnection::CHECK_KEEP_ALIVE_INTERVAL, gTimerWorker->GetTimerWorker(),
		this, &MainDbConnPool::KeepAlive_timer
	);
}

MainDbConnPool::~MainDbConnPool()
{
}

void MainDbConnPool::KeepAlive_timer()
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
		this, &MainDbConnPool::KeepAlive_timer
	);
}

DbConnection* MainDbConnPool::PopDbConn()
{	
	if (!mPool)
		return nullptr;
	
	if (DbConnection* conn = mPool->PopConn())
	{
		// 재접속 시도
		if (true == conn->HasError())
			conn->Connect();

		return conn;
	}
	
	return nullptr;
}

void MainDbConnPool::PushDbConn(DbConnection* conn)
{
	return mPool->PushConn(conn);
}
