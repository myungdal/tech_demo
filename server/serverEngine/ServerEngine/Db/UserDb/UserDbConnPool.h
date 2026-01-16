// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class DbConnectionPool;
class DbConnection;

class UserDbConnPool final
{
private:
	int mDbPoolSize = 1;

private:
	using PoolPtr = std::shared_ptr<DbConnectionPool>;
	using PoolPtrList = std::vector<PoolPtr>;

	PoolPtrList mPoolList;

public:
	explicit UserDbConnPool(SQLHENV env, int dbPoolSize);
	~UserDbConnPool();

public:
	void KeepAlive_timer();

public:	
	DbConnection* PopDbConn(DbShardIdx shardIdx);
	void PushDbConn(DbConnection* conn);

public:
	size_t GetUserDbShardCnt() const;
};

inline std::shared_ptr<UserDbConnPool> gUserDbConnPool = nullptr;

