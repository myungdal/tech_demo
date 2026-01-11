// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "StaticDbSession.h"

import ServerEngineDb;
import ServerEngineApp;
import ServerEngineUtil;


StaticDbSession::StaticDbSession(SQLHENV env, CommitType commitType)
	:
	DbSessionBase(commitType)
{
	for (const DbConfigData& dbConfigData : gAppConfigManager->GetDbConfigList())
	{
		if (DbType::STATIC != dbConfigData.mDbType)
			continue;

		auto [connStr, connStrLen] = StringUtil::FormatStr(
			L"Driver={{MySQL ODBC 8.0 Unicode Driver}};Server={};Port={};Database={};UID={};PWD={};OPTION=3;",
			dbConfigData.mDbIp,
			dbConfigData.mDbPort,
			dbConfigData.mDbName,
			dbConfigData.mDbUser,
			dbConfigData.mDbPwd
		);

		// 풀링을 사용하지 않고 한번 사용할 연결 생성.
		mConn = _NEW<DbConnection>(nullptr);
		
		if (mConn)
		{
			mConn->Connect(env, *connStr);
		}

		break;
	}
}

StaticDbSession::~StaticDbSession()
{
}
