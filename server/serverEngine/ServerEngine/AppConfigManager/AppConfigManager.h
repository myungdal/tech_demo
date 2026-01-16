// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// AppConfigManager.h - 앱 설정 관리자
#pragma once

#include "ServerEngine/AppConfigManager/AppConfigData.h"
#include "ServerEngine/AppConfigManager/DbConfigData.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 앱 설정 관리자
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 앱 설정 관리자 (JSON 파일에서 설정 로드 및 관리)
class AppConfigManager
{
public:
	// 변수 맵 타입
	using VariableMap = std::map<std::wstring, std::wstring>;

private:
	// 스레드 안전성을 위한 락
	Lock mLock;

	// 앱 ID 테이블
	AppIdTable mAppIdTable;
	
	// 앱 설정 배열
	AppConfigArray mMyAppConfigArray;
	
	// DB 설정 리스트
	DbConfigList mDbConfigList;

	// 봇 테스트 테이블
	BotTestTable mBotTestTable;

	// 변수 맵 (config에서 로드한 변수들)
	VariableMap mVariableMap;

	// 게임 서버용 정적 데이터 인덱스 리스트
	StaticDataIndexList mStaticDataIndexListForGameServer;
	
	// 클라이언트용 정적 데이터 인덱스 리스트
	StaticDataIndexList mStaticDataIndexListForClient;

public:
	// 현재 서버 IP
	std::wstring mMyIp = L"";

public:
	AppConfigManager();

	// 모든 설정 파일 로드
	void Load();

	// 특정 앱 타입의 첫 번째 앱 ID 조회
	AppId GetFirstAppId(AppType appType) const;
	
	// 앱 설정 조회
	const AppConfigData& GetMyAppConfig(AppType appType, AppArg appArg) const;
	
	// DB 설정 리스트 조회
	const DbConfigList& GetDbConfigList() const noexcept { return mDbConfigList; }
	
	// 봇 테스트 리스트 조회
	const BotTestList* GetBotTestList(BotScenarioId botScenarioId) const;

	// 변수 조회 (숫자)
	template<typename _Type>
	_Type GetVariableNum(const wchar_t* key)
	{
		ReadLock lock(mLock);

		const wchar_t* value = GetVariableStr(key);
		return static_cast<_Type>(std::stoi(value));
	}
	
	// 변수 조회 (문자열)
	const wchar_t* GetVariableStr(const wchar_t* key);

	// 게임 서버용 정적 데이터 인덱스 리스트 조회
	const StaticDataIndexList& GetStaticDataIndexListForGameServer() const noexcept { return mStaticDataIndexListForGameServer; }
	
	// 클라이언트용 정적 데이터 인덱스 리스트 조회
	const StaticDataIndexList& GetStaticDataIndexListForClient() const noexcept { return mStaticDataIndexListForClient; }

private:
	// 변수 로드
	void LoadVariable();
	
	// 앱 ID 테이블 로드
	void LoadAppIdTable();
	
	// 앱 설정 로드
	void LoadMyAppConfig();
	
	// 봇 테스트 설정 로드
	void LoadBotTestConfig();
	
	// DB 설정 로드
	void LoadDbConfig();
	
	// 정적 데이터 인덱스 로드
	void LoadStaticDataIndex();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 전역 싱글톤
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::shared_ptr<AppConfigManager> gAppConfigManager = nullptr;
