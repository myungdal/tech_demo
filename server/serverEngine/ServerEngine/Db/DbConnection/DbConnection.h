// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbConnection.h - DB 연결 클래스
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DbConnection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DbConnectionPool;

// DB 연결 (ODBC 연결 래퍼)
class DbConnection final
{
public:
	// Keep-Alive 체크 주기
	static constexpr ClockMs CHECK_KEEP_ALIVE_INTERVAL = 10 * 1000ms;

private:
	// 소속 커넥션 풀
	DbConnectionPool* mPool = nullptr;

	// ODBC 환경 핸들
	SQLHENV mEnv = nullptr;
	
	// 연결 문자열
	wchar_t mConnStr[MAX_PATH] = { 0 };

	// ODBC 연결 핸들
	SQLHDBC mDbc = SQL_NULL_HDBC;
	
	// ODBC 구문 핸들
	SQLHSTMT mStmt = SQL_NULL_HSTMT;

	// 현재 쿼리 시작 시간 (타임아웃 감지용)
	std::atomic<ClockPoint> mCurrentQueryTime = ClockPoint::min();

	// 에러 발생 여부
	bool mHasError = false;

public:
	// 생성자
	explicit DbConnection(DbConnectionPool* dbConnectionPool);
	
	// 소멸자
	~DbConnection();

	// DB 연결 (환경 및 연결 문자열 지정)
	bool Connect(SQLHENV env, const wchar_t* connStr);
	
	// DB 연결 (기존 정보 사용)
	bool Connect();

	// Keep-Alive 체크 (연결 유지 확인)
	bool KeepAlive();
	
	// 연결 활성 상태로 설정
	void SetAlive();

	// ODBC 정보 메시지 처리
	void OnInfo(SQLRETURN ret);
	
	// ODBC 에러 처리
	void OnError(SQLRETURN ret);

	// 소속 커넥션 풀 조회
	DbConnectionPool* GetDbConnectionPool() const { return mPool; }
	
	// ODBC 연결 핸들 조회
	SQLHDBC GetDbc() const noexcept { return mDbc; }
	
	// ODBC 구문 핸들 조회
	SQLHSTMT GetStmt() const noexcept { return mStmt; }
	
	// 에러 상태 확인
	bool HasError() const noexcept { return mHasError; }

private:
	// DB 연결 해제
	void Disconnect();
};
