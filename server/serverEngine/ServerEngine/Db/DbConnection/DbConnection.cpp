// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DbConnection.h"


DbConnection::DbConnection(DbConnectionPool* dbConnectionPool)
	:
	mPool(dbConnectionPool)
{
}
DbConnection::~DbConnection()
{
	Disconnect();
}
bool DbConnection::Connect(SQLHENV env, const wchar_t* connStr)
{
	mEnv = env;
	wcsncpy_s(mConnStr, connStr, _TRUNCATE);

	return Connect();
}

bool DbConnection::Connect()
{
	Disconnect();

	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_DBC, mEnv, &mDbc);
	if ((SQL_SUCCESS != ret) && (SQL_SUCCESS_WITH_INFO != ret))
	{
		_DEBUG_RED;
		
		_DEBUG_BREAK;
		return false;
	}

	ret = SQLDriverConnectW(
		mDbc, GetDesktopWindow(), mConnStr, _countof(mConnStr), nullptr, 0, nullptr, SQL_DRIVER_NOPROMPT
	);
	if ((SQL_SUCCESS != ret) && (SQL_SUCCESS_WITH_INFO != ret))
	{
		_DEBUG_RED;
		return false;
	}

	ret = SQLAllocHandle(SQL_HANDLE_STMT, mDbc, &mStmt);
	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		OnInfo(ret);
	}
	else if (SQL_SUCCESS != ret)
	{
		OnError(ret);
		return false;
	}

	ret = SQLSetConnectAttr(mDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);
	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		OnInfo(ret);
	}
	else if (SQL_SUCCESS != ret)
	{
		OnError(ret);
		return false;
	}

	SetAlive();

	return true;
}

bool DbConnection::KeepAlive()
{
	const ClockPoint currentQueryTime = mCurrentQueryTime.load();
	const ClockPoint& now = tClock.GetLocalNow();
	const ClockDuration duration = (now > currentQueryTime) ? (now - currentQueryTime) : 0ms;
	if (CHECK_KEEP_ALIVE_INTERVAL > duration)
		return true;

	if (SQL_NULL_HSTMT == mStmt)
		return false;

	const SQLRETURN ret = SQLExecDirectW(mStmt, (SQLWCHAR*)L"{call sp_system_keep_alive}", SQL_NTSL);
	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		OnInfo(ret);
	}
	else if (SQL_SUCCESS != ret)
	{
		OnError(ret);
		return false;
	}

	SQLFreeStmt(mStmt, SQL_CLOSE);
	SQLFreeStmt(mStmt, SQL_UNBIND);
	SQLFreeStmt(mStmt, SQL_RESET_PARAMS);
	SQLCloseCursor(mStmt);

	SetAlive();

	return true;
}

void DbConnection::SetAlive()
{
	mCurrentQueryTime = tClock.GetLocalNow();
}

void DbConnection::Disconnect()
{
	if (SQL_NULL_HSTMT != mStmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, mStmt);
		mStmt = SQL_NULL_HSTMT;
	}

	if (SQL_NULL_HDBC != mDbc)
	{
		SQLRETURN ret = SQLDisconnect(mDbc);
		if (SQL_SUCCESS_WITH_INFO == ret)
			OnInfo(ret);
		else if (SQL_SUCCESS != ret)
			return;

		SQLFreeHandle(SQL_HANDLE_DBC, mDbc);
		mDbc = SQL_NULL_HDBC;
	}

	mHasError = false;
}

void DbConnection::OnInfo(MAYBE_UNUSED SQLRETURN ret)
{
#if 0	

	_DEBUG_LOG(YELLOW, L"DbConnection::OnInfo, ret: {}", ret);

	if (SQL_NULL_HSTMT == mStmt)
		return;

	wchar_t* state = Buf_wchar_t();
	SQLINTEGER error = 0;
	wchar_t* msg = Buf_wchar_t();
	SQLSMALLINT msgLen = 0;

	SQLSMALLINT index = 1;
	while (true)
	{
		SQLRETURN rec = SQLGetDiagRec(SQL_HANDLE_STMT, mStmt, index, state, &error, msg, 1024, &msgLen);

		if (SQL_NO_DATA == rec)
			break;

		_DEBUG_LOG(YELLOW, L"https://en.wikipedia.org/wiki/SQLSTATE", index, state, error, msg);
		_DEBUG_LOG(YELLOW, L"DbConnection::OnInfo, index: {}, state: {}, error: {}, msg: {}", index, state, error, msg);
		
		++index;
	}
#endif
}

void DbConnection::OnError(MAYBE_UNUSED SQLRETURN ret)
{
	_DEBUG_LOG(RED, L"ret: {}", ret);

	mHasError = true;

	if (SQL_NULL_HSTMT == mStmt)
		return;

	Buf_wchar_t state;
	Buf_wchar_t msg;
	SQLINTEGER error = 0;	
	SQLSMALLINT msgLen = 0;

	SQLSMALLINT index = 1;
	while (true)
	{
		const SQLRETURN rec = SQLGetDiagRec(SQL_HANDLE_STMT, mStmt, index, *state, &error, *msg, 1024, &msgLen);

		if (SQL_NO_DATA == rec)
			break;

		const wchar_t* _state = *state;
		const wchar_t* _msg = *msg;
		_DEBUG_LOG(RED, L"index: {}, state: {}, error: {}, msg: {}", index, _state, error, _msg);

		++index;
	}
}
