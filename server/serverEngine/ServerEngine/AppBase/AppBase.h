// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// AppBase.h - 서버 애플리케이션 기본 클래스
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AppBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct AppConfigData;

// 서버 애플리케이션 기본 클래스 (모든 서버 앱의 베이스)
class AppBase
{
private:
	// 앱 타입 (불변)
	const AppType mAppType = AppType::NONE;
	
	// 앱 실행 인자 (불변)
	const AppArg mAppArg = DEFAULT_APP_ARG;

protected:
	// IOCP 핸들러 (파생 클래스 접근 허용)
	std::shared_ptr<class Iocp> mIocp = nullptr;
	
	// Worker 스레드 관리자 (파생 클래스 접근 허용)
	std::shared_ptr<class WorkerThreadManager> mWorkerThreadManager = nullptr;

private:
	// 콘솔 로그 출력기
	std::shared_ptr<class LogWriterConsole> mLogWriterConsole = nullptr;
	
	// 파일 로그 출력기
	std::shared_ptr<class LogWriterFile> mLogWriterFile = nullptr;

public:
	// 생성자 (앱 타입 및 인자 지정)
	explicit AppBase(AppType appType, AppArg appArg);
	
	// 가상 소멸자
	virtual ~AppBase();

	// IOCP 스레드 생성
	void CreateIocpThread();

	// Worker 재개
	void ResumeWorker() const;
	
	// Worker 일시정지
	void SuspendWorker() const;

	// 메인 루프 실행 (파생 클래스에서 오버라이드 가능)
	virtual void Run();

	// 현재 앱 설정 조회
	const AppConfigData& GetMyAppConfig() const;
	
	// 현재 앱 ID 조회
	AppId GetMyAppId() const;
	
	// 현재 앱 타입 조회
	AppType GetMyAppType() const { return mAppType; }

protected:
	// 초기화 (파생 클래스에서 호출)
	void Initialize();

	// 로그 갱신 (주기적 호출)
	void UpdateLog();
};
