// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DbConnectionPool.h"

import ServerEngineDb;


DbConnectionPool::DbConnectionPool(SQLHENV env)
	:
	mEnv(env)
{
}
DbConnectionPool::~DbConnectionPool()
{
	for (auto& conn : mDbConnectionList) 
	{
		_DELETE(conn.get()->load());
	}
}
int DbConnectionPool::CreateConnection(const wchar_t* connStr, int connectionCount)
{
	_ASSERT_CRASH(INVALID_THREAD_ID == tThreadId);

	if (SQL_NULL_HENV == mEnv)
		return 0;

	mConnStr = connStr;

	_ASSERT_CRASH(true == mDbConnectionList.empty());

	for (int i = 0; i < connectionCount; ++i)
	{
		DbConnection* conn = _NEW<DbConnection>(this);
		if (false == conn->Connect(mEnv, mConnStr.data()))
		{
			_ASSERT_CRASH(false);
			return 0;
		}		

		auto& result = mDbConnectionList.emplace_back(std::make_unique<DbConnectionPtr>(nullptr));
		result.get()->store(conn);
	}

	return connectionCount;
}

DbConnection* DbConnectionPool::PopConn()
{
	const size_t popIdx = (++mPopIdx) % mDbConnectionList.size();
	
	do
	{
		DbConnection* conn = mDbConnectionList[popIdx].get()->exchange(nullptr);
		if (conn)
			return conn;

		_YIELD_WORKER_THREAD;

	} while (true);
	
	return nullptr;
}

void DbConnectionPool::PushConn(DbConnection* conn)
{
	const size_t pushIdx = (++mPushIdx) % mDbConnectionList.size();

	DbConnection* expected = nullptr;
	if (false == mDbConnectionList[pushIdx].get()->compare_exchange_strong(expected, conn))
	{
		_DEBUG_RED;
		_DELETE(conn);
	}
}
