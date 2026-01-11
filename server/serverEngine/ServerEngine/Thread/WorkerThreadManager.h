// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WorkerThreadManager - 워커 스레드 관리자
//
// [WHY] Start/Stop으로 명명 변경 (Resume/Suspend 대신)
// - std::jthread 기반으로 변경되면서 의미가 더 명확해짐
// - Start(): 스레드 시작 신호
// - Stop(): graceful shutdown (stop 요청 + join)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WorkerThreadManager
{
private:
	std::shared_ptr<class Iocp> mIocp = nullptr;

private:
	using WorkerThreadPtr = std::shared_ptr<class WorkerThread>;
	using WorkerThreadPtrList = std::vector<WorkerThreadPtr>;

	WorkerThreadPtrList mWorkerThreadPtrList;

private:
	using IocpThreadPtr = std::shared_ptr<class IocpThread>;

	IocpThreadPtr mIocpThreadPtr = nullptr;

public:
	explicit WorkerThreadManager(ThreadId workerThreadCount, std::shared_ptr<class Iocp> iocp);
	~WorkerThreadManager();

public:
	void CreateIocpThread();

public:
	// 모든 스레드 시작
	void Start();

	// 모든 스레드 종료 요청 및 대기 (graceful shutdown)
	void Stop();

	// 기존 API 호환성 유지 (deprecated, Start/Stop 사용 권장)
	[[deprecated("Use Start() instead")]]
	void Resume() { Start(); }

	[[deprecated("Use Stop() instead")]]
	void Suspend() { Stop(); }
};
