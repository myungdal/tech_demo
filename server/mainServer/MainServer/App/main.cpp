// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;


int wmain(MAYBE_UNUSED int argc, MAYBE_UNUSED wchar_t* argv[])
{
	_PERFORMANCE_TRACE_START;

	gLogQueue = std::make_shared<LogQueue>();

	const long appArg = (argc > 1) ? wcstol(argv[1], nullptr, 10) : 0;

	gMainServerApp = std::make_shared<MainServerApp>(static_cast<AppArg>(appArg));

	gMainServerApp->ResumeWorker();
	gMainServerApp->Run();
	gMainServerApp->SuspendWorker();

	return 0;
}

