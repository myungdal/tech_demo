// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DbSessionBase.h"

import ServerEngineDb;


DbSessionBase::DbSessionBase(CommitType commitType)
	:
	mCommitType(commitType)
{
}
DbSessionBase::~DbSessionBase()
{
	Release();
	if (mConn)
		_DELETE(mConn);
}
void DbSessionBase::Acquire()
{
	mConn = OnAcquire();

	if (nullptr == mConn)
		return;

	if (CommitType::AUTO == mCommitType)
	{
		const SQLRETURN ret = SQLSetConnectAttr(mConn->GetDbc(), SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);
		if (SQL_SUCCESS_WITH_INFO == ret)
		{
			mConn->OnInfo(ret);
		}
		else if (SQL_SUCCESS != ret)
		{
			mConn->OnError(ret);
		}
	}
	else
	{
		const SQLRETURN ret = SQLSetConnectAttr(mConn->GetDbc(), SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);
		if (SQL_SUCCESS_WITH_INFO == ret)
		{
			mConn->OnInfo(ret);
		}
		else if (SQL_SUCCESS != ret)
		{
			mConn->OnError(ret);
		}
	}

	mConn->SetAlive();
}

void DbSessionBase::Release()
{
	if (nullptr == mConn)
		return;

	Free();

	OnRelease(mConn);

	mConn = nullptr;
}

bool DbSessionBase::Execute(const wchar_t* sql)
{
	if (nullptr == mConn)
		return false;

	if (mConn->HasError())
		return false;

	mIsFree = false;

	const SQLRETURN ret = SQLExecDirectW(mConn->GetStmt(), const_cast<SQLWCHAR*>(sql), SQL_NTSL);

	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		mConn->OnInfo(ret);
		return true;
	}

	if (SQL_SUCCESS == ret)
	{
		return true;
	}

	mConn->OnError(ret);
	return false;
}

bool DbSessionBase::Commit()
{
	if (nullptr == mConn)
	{
		_ASSERT_CRASH(false);
		return false;
	}

	Free();

	_ASSERT_CRASH(CommitType::MANUAL == mCommitType);

	const SQLRETURN ret = SQLEndTran(SQL_HANDLE_DBC, mConn->GetDbc(), SQL_COMMIT);
	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		mConn->OnInfo(ret);
	}
	else if (SQL_SUCCESS != ret)
	{
		mConn->OnError(ret);
	}

	return (SQL_SUCCESS_WITH_INFO == ret) || (SQL_SUCCESS == ret);
}

bool DbSessionBase::Rollback()
{
	if (nullptr == mConn)
	{
		_ASSERT_CRASH(false);
		return false;
	}

	Free();

	_ASSERT_CRASH(CommitType::MANUAL == mCommitType);

	const SQLRETURN ret = SQLEndTran(SQL_HANDLE_DBC, mConn->GetDbc(), SQL_ROLLBACK);
	if (SQL_SUCCESS_WITH_INFO == ret)
	{
		mConn->OnInfo(ret);
	}
	else if (SQL_SUCCESS != ret)
	{
		mConn->OnError(ret);
	}

	return (SQL_SUCCESS_WITH_INFO == ret) || (SQL_SUCCESS == ret);
}

void DbSessionBase::Free()
{
	if (nullptr == mConn)
		return;

	if (true == mIsFree)
		return;

	SQLFreeStmt(mConn->GetStmt(), SQL_CLOSE);
	SQLFreeStmt(mConn->GetStmt(), SQL_UNBIND);
	SQLFreeStmt(mConn->GetStmt(), SQL_RESET_PARAMS);

	SQLCloseCursor(mConn->GetStmt());

	mIsFree = true;
}

int64_t DbSessionBase::GetRowCount() const
{
	if (mConn == nullptr)
		return 0;

	SQLLEN rowCount = 0;
	const SQLRETURN ret = SQLRowCount(mConn->GetStmt(), &rowCount);
	return (SQL_SUCCESS == ret || SQL_SUCCESS_WITH_INFO == ret) ? static_cast<int64_t>(rowCount) : -1;
}
