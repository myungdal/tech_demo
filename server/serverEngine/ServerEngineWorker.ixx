// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ServerEngineWorker.ixx - Worker Module
// Worker, Thread 제공

module;

// GMF - Worker
#include "ServerEngine/Worker/WorkerTask/WorkerTask.h"
#include "ServerEngine/Worker/WorkerTask/WorkerTaskNode.h"
#include "ServerEngine/Worker/WorkerTask/WorkerTaskImpl.h"
#include "ServerEngine/Worker/WorkerBase/Worker.h"
#include "ServerEngine/Worker/WorkerImpl/CoWorker.h"
#include "ServerEngine/Worker/WorkerImpl/CronWorker.h"
#include "ServerEngine/Worker/WorkerImpl/ThreadWorker.h"
#include "ServerEngine/Worker/WorkerImpl/TimerWorker.h"
#include "ServerEngine/Worker/WorkerManager.h"
#include "ServerEngine/Util/MwsrQueue/MwsrQueue.h"

// GMF - Thread
#include "ServerEngine/Thread/ThreadBase/Thread.h"
#include "ServerEngine/Thread/ThreadImpl/WorkerThread.h"
#include "ServerEngine/Thread/ThreadImpl/IocpThread.h"
#include "ServerEngine/Thread/ThreadImpl/ConsoleThread.h"
#include "ServerEngine/Thread/WorkerThreadManager.h"

export module ServerEngineWorker;

// ============= Worker =============
export using ::WorkerTaskNode;
export using ::Worker;
export using ::CoWorker;
export using ::CronWorker;
export using ::TimerWorker;
export using ::ThreadWorker;
export using ::WorkerManager;
export using ::gCoWorker;
export using ::gCronWorker;
export using ::gTimerWorker;
export using ::gAnyThreadId;
export using ::gCurrentCronHandle;

// ============= Thread =============
export using ::Thread;
export using ::WorkerThread;
export using ::IocpThread;
export using ::ConsoleThread;
export using ::WorkerThreadManager;

