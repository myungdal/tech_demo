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

class MainDbConnPool final
{
private:
	int mDbPoolSize = 1;

private:
	std::shared_ptr<DbConnectionPool> mPool = nullptr;

public:
	explicit MainDbConnPool(SQLHENV env, int dbPoolSize);
	~MainDbConnPool();

public:
	void KeepAlive_timer();

public:
	DbConnection* PopDbConn();
	void PushDbConn(DbConnection* conn);
};

inline std::shared_ptr<MainDbConnPool> gMainDbConnPool = nullptr;

