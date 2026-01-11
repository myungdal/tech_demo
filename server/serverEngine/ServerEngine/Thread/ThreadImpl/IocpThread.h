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
// IocpThread - 네트워크 처리 전용 스레드
//
// 모든 WorkerThread가 네트워크 패킷을 대기할 경우를 대비한 백업 스레드.
// DispatchPacket(NetworkPacket& rp)에서 DispatchPacketOnAnyThread로 갈아탄다
//
// [WHY] stop_token 기반 graceful shutdown
// - IOCP DispatchCompletion이 블로킹되더라도 stop 요청 시 종료 가능
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Iocp;

class IocpThread : public Thread
{
private:
	std::shared_ptr<Iocp> mIocp = nullptr;

public:
	explicit IocpThread(std::shared_ptr<Iocp> iocp);
	virtual ~IocpThread();

protected:
	void Run(std::stop_token stopToken) override;
};
