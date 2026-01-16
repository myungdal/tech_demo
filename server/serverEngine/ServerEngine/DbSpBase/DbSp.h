// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbSp.h - DB 저장 프로시저 기본 클래스
#pragma once

#include "ServerEngine/Db/DbSessionBase/DbSession.h"
#include "ServerEngine/Db/DbConnection/DbConnection.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DbSp
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DB 저장 프로시저 기본 클래스 (파라미터 바인딩 및 실행)
class DbSp
{
private:
	// 입력 파라미터 위치
	SQLUSMALLINT mPosIn = 0;
	
	// 출력 파라미터 위치
	SQLUSMALLINT mPosOut = 0;

	// 사용할 DB 세션
	DbSessionBase& mDbSession;

	// 실행할 SQL 문자열
	const wchar_t* mSql;

public:
	// 생성자
	explicit DbSp(const wchar_t* sql, DbSessionBase& dbSession);
	
	// 가상 소멸자
	virtual ~DbSp();

protected:
	friend struct DbUtil;

	// 저장 프로시저 실행
	bool Execute() const;
	
	// 영향받은 행 개수 조회
	int64_t GetRowCount() const;
	
	// 결과 행 가져오기
	bool Fetch() const;
	
	// 저장 프로시저 반환값 조회 (파생 클래스에서 오버라이드)
	virtual int64_t GetSpResult() const { return 0; }

protected:
	// 입력 파라미터 바인딩 (템플릿)
	template <typename _Type>
	bool BindIn(_Type& v)
	{
		SQLRETURN ret = BindIn(&v, tDbSpInBuf + mPosIn, mPosIn + 1);
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
	
	// 입력 파라미터 바인딩 (날짜)
	bool BindInDate(const TIMESTAMP_STRUCT& v);
	
	// 입력 파라미터 바인딩 (문자열)
	bool BindInStr(const wchar_t* v);
	
	// 입력 파라미터 바인딩 (바이너리)
	bool BindInBin(const BYTE* v, SQLLEN size);

	// 출력 파라미터 바인딩 (템플릿)
	template <typename _Type>
	bool BindOut(_Type& v)
	{
		const SQLRETURN ret = BindOut(&v, tDbSpOutBuf + mPosOut, mPosOut + 1);
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
	
	// 출력 파라미터 바인딩 (날짜)
	bool BindOutDate(TIMESTAMP_STRUCT& v);
	
	// 출력 파라미터 바인딩 (문자열)
	bool BindOutStr(wchar_t* v, SQLLEN len);
	
	// 출력 파라미터 바인딩 (바이너리)
	bool BindOutBin(BYTE* v, SQLLEN size);

private:
	SQLRETURN BindIn(const int8_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindIn(const int16_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindIn(const int32_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindIn(const int64_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindIn(const uint8_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindIn(const uint16_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindIn(const uint32_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindIn(const uint64_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindInDate(const TIMESTAMP_STRUCT* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindInStr(const wchar_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindInBin(const BYTE* p, SQLLEN size, SQLLEN* b, SQLUSMALLINT i) const;

	SQLRETURN BindOut(int8_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindOut(int16_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindOut(int32_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindOut(int64_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindOut(uint8_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindOut(uint16_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindOut(uint32_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindOut(uint64_t* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindOutDate(TIMESTAMP_STRUCT* p, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindOutStr(wchar_t* p, SQLLEN len, SQLLEN* b, SQLUSMALLINT i) const;
	SQLRETURN BindOutBin(BYTE* p, SQLLEN size, SQLLEN* b, SQLUSMALLINT i) const;
};
