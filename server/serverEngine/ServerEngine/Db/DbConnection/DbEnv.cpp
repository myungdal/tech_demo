// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DbEnv.h"


DbEnv::DbEnv()
{
	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mHandle);
	_ASSERT_CRASH(SQL_SUCCESS == ret);
	ret = SQLSetEnvAttr(mHandle, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3_80), 0);
	_ASSERT_CRASH(SQL_SUCCESS == ret);
}
DbEnv::~DbEnv()
{
	if (SQL_NULL_HENV == mHandle)
		return;
	SQLFreeHandle(SQL_HANDLE_ENV, mHandle);
	mHandle = SQL_NULL_HENV;
}
