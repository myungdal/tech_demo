// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbConfigData.h - DB 설정 데이터 구조체
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DB 설정 데이터
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DB 설정 데이터 (db_config.json에서 로드)
struct DbConfigData
{
	DbType mDbType;                 // DB 타입
	std::wstring mDbName = L"";     // DB 이름
	DbShardIdx mDbShardIdx = -1;    // DB 샤드 인덱스
	std::wstring mDbIp = L"";       // DB 서버 IP
	uint16_t mDbPort = 0;           // DB 서버 포트
	std::wstring mDbUser = L"";     // DB 사용자명
	std::wstring mDbPwd = L"";      // DB 비밀번호
};

// DB 설정 리스트
using DbConfigList = std::vector<DbConfigData>;
