// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Shell;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ShellMcpThread::ShellMcpThread()
	: Thread(INVALID_THREAD_ID)
{
}

ShellMcpThread::~ShellMcpThread()
{
}

void ShellMcpThread::Run(std::stop_token stopToken)
{
	if (gShellMcpManager)
		gShellMcpManager->Run(stopToken);
}
