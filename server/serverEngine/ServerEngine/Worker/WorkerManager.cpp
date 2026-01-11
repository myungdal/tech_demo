// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "WorkerManager.h"


void WorkerManager::CancelCron(CronHandle cronHandle)
{
	gCronWorker->CancelCron(cronHandle);
}
