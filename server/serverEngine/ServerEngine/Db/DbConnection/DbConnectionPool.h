// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbConnectionPool.h - DB 커넥션 풀
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DbConnectionPool
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DbConnection;

// DB 커넥션 풀 (Lock-Free 큐 기반)
class DbConnectionPool final
{
private:
	// 타입 별칭
	using DbConnectionPtr = std::atomic<DbConnection*>;
	using DbConnectionList = std::vector<std::unique_ptr<DbConnectionPtr>>;

private:
	// ODBC 환경 핸들
	SQLHENV mEnv = SQL_NULL_HENV;

	// 커넥션 리스트
	DbConnectionList mDbConnectionList;

	// 연결 문자열
	std::wstring mConnStr;

	// Pop 인덱스 (Lock-Free)
	std::atomic<size_t> mPopIdx = 0;
	
	// Push 인덱스 (Lock-Free)
	std::atomic<size_t> mPushIdx = 0;

public:
	// 생성자
	explicit DbConnectionPool(SQLHENV env);
	
	// 소멸자
	~DbConnectionPool();

	// 커넥션 생성 (지정된 개수만큼)
	int CreateConnection(const wchar_t* connStr, int connectionCount);

	// 커넥션 획득
	DbConnection* PopConn();
	
	// 커넥션 반환
	void PushConn(DbConnection* conn);

	// 풀 크기 조회
	size_t GetPoolSize() const { return mDbConnectionList.size(); }
};
