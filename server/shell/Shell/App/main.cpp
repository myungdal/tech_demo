// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Shell;


int wmain(MAYBE_UNUSED int argc, MAYBE_UNUSED wchar_t* argv[])
{
	_PERFORMANCE_TRACE_START;

	gLogQueue = std::make_shared<LogQueue>();

	const long appArg = (argc > 1) ? wcstol(argv[1], nullptr, 10) : 0;

	if (argc > 2)
		SetCurrentDirectoryW(argv[2]);

	gShellApp = std::make_shared<ShellApp>(appArg);

	gShellApp->ResumeWorker();
	gShellApp->Run();
	gShellApp->SuspendWorker();

	return 0;
}
