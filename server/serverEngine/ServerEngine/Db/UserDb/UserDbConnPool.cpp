// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ServerEngine/PCH/pch_serverEngine.h"
#include "UserDbConnPool.h"

// 紐⑤ ?ъ?遺媛 (C1605: 4GB 珥怨?
// ??? ?ㅻ留 吏? include
#include "ServerEngine/Worker/WorkerManager.h"
#include "ServerEngine/Worker/WorkerImpl/TimerWorker.h"
#include "ServerEngine/Db/DbConnection/DbConnection.h"
#include "ServerEngine/Db/DbConnection/DbConnectionPool.h"
#include "ServerEngine/AppConfigManager/AppConfigManager.h"
#include "ServerEngine/Util/StringUtil/StringUtil.h"


UserDbConnPool::UserDbConnPool(SQLHENV env, int dbPoolSize)
	:
	mDbPoolSize(dbPoolSize)
{
	for (const DbConfigData& dbConfigData : gAppConfigManager->GetDbConfigList())
	{
		if (DbType::USER != dbConfigData.mDbType)
			continue;
		auto [connStr, connStrLen] = StringUtil::FormatStr(
			L"Driver={{MySQL ODBC 8.0 Unicode Driver}};Server={};Port={};Database={};UID={};PWD={};OPTION=3;",
			dbConfigData.mDbIp,
			dbConfigData.mDbPort,
			dbConfigData.mDbName,
			dbConfigData.mDbUser,
			dbConfigData.mDbPwd
		);

		PoolPtr pool = std::make_shared<DbConnectionPool>(env);
		
		pool->CreateConnection(*connStr, dbPoolSize);
		mPoolList.emplace_back(pool);
	}

	WorkerManager::RunOnTimer(
		DbConnection::CHECK_KEEP_ALIVE_INTERVAL, gTimerWorker->GetTimerWorker(), 
		this, &UserDbConnPool::KeepAlive_timer
	);
}

UserDbConnPool::~UserDbConnPool()
{	
}

void UserDbConnPool::KeepAlive_timer()
{
	for (PoolPtr pool : mPoolList)
	{
		if (DbConnection* dbConn = pool->PopConn())
		{
			if (true == dbConn->HasError())
			{
				dbConn->Connect();
			}
			else
			{
				dbConn->KeepAlive();
			}

			pool->PushConn(dbConn);
		}
	}

	WorkerManager::RunOnTimer(
		DbConnection::CHECK_KEEP_ALIVE_INTERVAL, gTimerWorker->GetTimerWorker(), 
		this, &UserDbConnPool::KeepAlive_timer
	);
}

DbConnection* UserDbConnPool::PopDbConn(DbShardIdx shardIdx)
{
	if (shardIdx == INVALID_DB_SHARD_IDX)
		return nullptr;

	if (mPoolList.size() == 0)
		return nullptr;

	const size_t idx = (shardIdx % mPoolList.size());

	PoolPtr pool = mPoolList[idx];
	DbConnection* dbConn = pool->PopConn();

	// ?ъ곌껐 ??.
	if (true == dbConn->HasError())
		dbConn->Connect();

	return dbConn;
}

void UserDbConnPool::PushDbConn(DbConnection* conn)
{
	if (DbConnectionPool* pool = conn->GetDbConnectionPool())
	{
		pool->PushConn(conn);
	}
}

size_t UserDbConnPool::GetUserDbShardCnt() const
{
	return mPoolList.size();
}

