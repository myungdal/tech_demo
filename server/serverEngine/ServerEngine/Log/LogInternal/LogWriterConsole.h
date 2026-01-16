// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class LogWriterConsole
{
protected:
	HANDLE mConsoleInputHandle = INVALID_HANDLE_VALUE;
	HANDLE mConsoleOutputHandle = INVALID_HANDLE_VALUE;
	uint16_t mConsoleFontColor = 0;

public:
	LogWriterConsole(AppType appType, AppArg appArg, AppId mAppId);
	~LogWriterConsole();

public:
	void WriteToConsole(const Log* log);
};
