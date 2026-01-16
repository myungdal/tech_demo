// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ServerEngineLog.ixx - Log Module (경량)
module;

// GMF - Log
#include "ServerEngine/Log/Log.h"
#include "ServerEngine/Log/LogInternal/LogQueue.h"
#include "ServerEngine/Log/LogInternal/LogWriterConsole.h"
#include "ServerEngine/Log/LogInternal/LogWriterFile.h"

export module ServerEngineLog;

// ============= Log =============
export using ::LogQueue;
export using ::LogWriterConsole;
export using ::LogWriterFile;
export using ::gLogQueue;

