// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// AppListManager.h - 앱 목록 관리자
#pragma once

#include "ServerEngine/AppListManager/AppInfo.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 앱 목록 관리자
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 앱 목록 관리자 (연결된 모든 앱 정보 관리)
class AppListManager
{
private:
	// 스레드 안전성을 위한 락
	mutable Lock mLock;

	// 앱 목록 (AppId 인덱스)
	AppList mAppList;
	
	// 앱 ID 테이블 (AppType별 AppId 목록)
	AppIdTable mAppIdTable;

public:
	AppListManager();

	// 모든 앱 정보 제거 (WriteLock 반환)
	WriteLock ClearAppInfoAll();
	
	// 앱 정보 추가
	std::pair<AppInfo*, WriteLock> AddAppInfo(const APP_DATA& appData);
	
	// 앱 정보 제거
	bool RemoveAppInfo(AppId appId);

	// 앱 정보 검색
	std::pair<AppInfo*, ReadLock> FindAppInfo(AppId appId);
	
	// 특정 타입의 앱 ID 리스트 조회
	std::pair<AppIdList*, ReadLock> GetAppIdList(AppType appType);
	
	// 전체 앱 리스트 조회
	std::pair<AppList*, ReadLock> GetAppList();

	// Main 서버 앱 정보 조회
	std::pair<AppInfo*, ReadLock> GetMainServerAppInfo();
	
	// Shell 앱 정보 조회
	std::pair<AppInfo*, ReadLock> GetShellAppInfo();
	
	// DB 서버 앱 정보 조회 (AccountId 기반)
	std::pair<AppInfo*, ReadLock> GetDbServerAppInfo(const AccountId accountId);
	
	// DB 서버 앱 정보 선택 (로드 밸런싱)
	std::pair<AppInfo*, ReadLock> PickDbServerAppInfo();
	
	// Bridge 서버 앱 정보 선택 (로드 밸런싱)
	std::pair<AppInfo*, ReadLock> PickBridgeServerAppInfo();
	
	// Game 서버 앱 정보 선택 (서버 ID 기반)
	std::pair<AppInfo*, ReadLock> PickGameServerAppInfo(size_t serverId);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 전역 싱글톤
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::shared_ptr<AppListManager> gAppListManager = nullptr;

