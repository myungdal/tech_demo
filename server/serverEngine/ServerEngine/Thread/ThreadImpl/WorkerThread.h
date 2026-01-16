// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Thread/ThreadBase/Thread.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WorkerThread - IOCP + CoWorker 처리 스레드
//
// [WHY] std::stop_token 기반 cooperative cancellation
// - 무한 루프에서 stop_requested() 체크로 graceful shutdown 지원
// - 기존 while(true) 대신 while(!stopToken.stop_requested()) 패턴
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Iocp;

class WorkerThread : public Thread
{
private:
	std::shared_ptr<Iocp> mIocp = nullptr;

public:
	explicit WorkerThread(ThreadId threadId, std::shared_ptr<Iocp> iocp);
	virtual ~WorkerThread();

protected:
	void Run(std::stop_token stopToken) override;
};
