// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "IocpThread.h"

import ServerEngineWorker;
import ServerEngineNetwork;


IocpThread::IocpThread(std::shared_ptr<Iocp> iocp)
	: 
	Thread(INVALID_THREAD_ID),
	mIocp(iocp)
{
}

IocpThread::~IocpThread()
{
}

void IocpThread::Run(std::stop_token stopToken)
{
	// [WHY] stop_requested() 체크로 graceful shutdown 지원
	while (!stopToken.stop_requested())
	{
		mIocp->DispatchCompletion(1);
	}

	TlsCommonBuffer::ReleaseTlsCommonBuffer();
}
