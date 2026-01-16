// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DbSp.h"


DbSp::DbSp(const wchar_t* sql, DbSessionBase& session)
	:
	mDbSession(session),
	mSql(sql)
{
	mDbSession.Free();
}
DbSp::~DbSp()
{
}
bool DbSp::Execute() const
{
	_DEBUG_LOG(YELLOW, L"DbSp: {}", mSql);

	return mDbSession.Execute(mSql);
}

int64_t DbSp::GetRowCount() const
{
	return mDbSession.GetRowCount();
}

bool DbSp::Fetch() const
{	
	if (nullptr == mDbSession.GetDbConn())
		return false;

	const SQLRETURN ret = SQLFetch(mDbSession.GetDbConn()->GetStmt());

	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		mDbSession.GetDbConn()->OnInfo(ret);
		return true;
	}

	if (SQL_SUCCESS == ret)
		return true;

	if (SQL_NO_DATA == ret)
	{
		//ret = SQLMoreResults(mDbSession.GetDbConn()->GetStmt());
		//_ASSERT_CRASH(SQL_SUCCESS == ret);
		return false;
	}

	mDbSession.GetDbConn()->OnError(ret);
	return false;	
}

bool DbSp::BindInDate(const TIMESTAMP_STRUCT& v)
{
	const SQLRETURN ret = BindInDate(&v, tDbSpInBuf + mPosIn, mPosIn + 1);
	++mPosIn;

	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		mDbSession.GetDbConn()->OnInfo(ret);
		return true;
	}

	if (SQL_SUCCESS == ret)
		return true;
	
	mDbSession.GetDbConn()->OnError(ret);
	return false;
}

bool DbSp::BindInStr(const wchar_t* v)
{
	const SQLRETURN ret = BindInStr(v, tDbSpInBuf + mPosIn, mPosIn + 1);
	++mPosIn;

	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		mDbSession.GetDbConn()->OnInfo(ret);
		return true;
	}

	if (SQL_SUCCESS == ret)
		return true;

	mDbSession.GetDbConn()->OnError(ret);
	return false;
}

bool DbSp::BindInBin(const BYTE* v, SQLLEN size)
{
	const SQLRETURN ret = BindInBin(v, size, tDbSpInBuf + mPosIn, mPosIn + 1);
	++mPosIn;

	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		mDbSession.GetDbConn()->OnInfo(ret);
		return true;
	}

	if (SQL_SUCCESS == ret)
		return true;

	mDbSession.GetDbConn()->OnError(ret);
	return false;
}

bool DbSp::BindOutDate(TIMESTAMP_STRUCT& v)
{
	const SQLRETURN ret = BindOutDate(&v, tDbSpOutBuf + mPosOut, mPosOut + 1);
	++mPosOut;

	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		mDbSession.GetDbConn()->OnInfo(ret);
		return true;
	}

	if (SQL_SUCCESS == ret)
		return true;

	mDbSession.GetDbConn()->OnError(ret);
	return false;
}

bool DbSp::BindOutStr(wchar_t* v, SQLLEN len)
{
	const SQLRETURN ret = BindOutStr(v, len, tDbSpOutBuf + mPosOut, mPosOut + 1);
	++mPosOut;

	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		mDbSession.GetDbConn()->OnInfo(ret);
		return true;
	}

	if (SQL_SUCCESS == ret)
		return true;

	mDbSession.GetDbConn()->OnError(ret);
	return false;
}

bool DbSp::BindOutBin(BYTE* v, SQLLEN size)
{
	const SQLRETURN ret = BindOutBin(v, size, tDbSpOutBuf + mPosOut, mPosOut + 1);
	++mPosOut;

	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		mDbSession.GetDbConn()->OnInfo(ret);
		return true;
	}

	if (SQL_SUCCESS == ret)
		return true;

	mDbSession.GetDbConn()->OnError(ret);
	return false;
}

SQLRETURN DbSp::BindIn(const int8_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_CHAR, 0, 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindIn(const int16_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindIn(const int32_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindIn(const int64_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindIn(const uint8_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_CHAR, 0, 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindIn(const uint16_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindIn(const uint32_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindIn(const uint64_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindInDate(const TIMESTAMP_STRUCT* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, sizeof(TIMESTAMP_STRUCT), 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindInStr(const wchar_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	SQLULEN len = static_cast<SQLULEN>(std::wcslen(p));
	_ASSERT_CRASH(65536 > len);
	SQLULEN size = sizeof(wchar_t) * len;
	*b = SQL_NTS;
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, size, 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindInBin(const BYTE* p, SQLLEN size, SQLLEN* b, SQLUSMALLINT i) const
{
	*b = size;
	_ASSERT_CRASH(65536 > size);
	return SQLBindParameter(mDbSession.GetDbConn()->GetStmt(), i, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, size, 0, (SQLPOINTER)p, 0, b);
}

SQLRETURN DbSp::BindOut(int8_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_TINYINT, static_cast<SQLPOINTER>(p), sizeof(int8_t), b);
}

SQLRETURN DbSp::BindOut(int16_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_SHORT, static_cast<SQLPOINTER>(p), sizeof(int16_t), b);
}

SQLRETURN DbSp::BindOut(int32_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_LONG, static_cast<SQLPOINTER>(p), sizeof(int32_t), b);
}

SQLRETURN DbSp::BindOut(int64_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_SBIGINT, static_cast<SQLPOINTER>(p), sizeof(int64_t), b);
}

SQLRETURN DbSp::BindOut(uint8_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_UTINYINT, static_cast<SQLPOINTER>(p), sizeof(uint8_t), b);
}

SQLRETURN DbSp::BindOut(uint16_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_USHORT, static_cast<SQLPOINTER>(p), sizeof(uint16_t), b);
}

SQLRETURN DbSp::BindOut(uint32_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_ULONG, static_cast<SQLPOINTER>(p), sizeof(uint32_t), b);
}

SQLRETURN DbSp::BindOut(uint64_t* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_UBIGINT, static_cast<SQLPOINTER>(p), sizeof(uint64_t), b);
}

SQLRETURN DbSp::BindOutDate(TIMESTAMP_STRUCT* p, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_TYPE_TIMESTAMP, static_cast<SQLPOINTER>(p), sizeof(TIMESTAMP_STRUCT), b);
}

SQLRETURN DbSp::BindOutStr(wchar_t* p, SQLLEN len, SQLLEN* b, SQLUSMALLINT i) const
{	
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_WCHAR, static_cast<SQLPOINTER>(p), len, b);
}

SQLRETURN DbSp::BindOutBin(BYTE* p, SQLLEN size, SQLLEN* b, SQLUSMALLINT i) const
{
	return SQLBindCol(mDbSession.GetDbConn()->GetStmt(), i, SQL_C_BINARY, static_cast<SQLPOINTER>(p), size, b);
}
