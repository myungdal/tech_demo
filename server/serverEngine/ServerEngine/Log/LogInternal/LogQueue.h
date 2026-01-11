// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Util/MwsrQueue/MwsrQueue.h"
#include "ServerEngine/Log/Log.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LogQueue
{
private:
	MwsrQueue<Log> mTimerTaskTable;

public:
	template<typename _Color, typename... _Args>
	void Push(_Color color, const wchar_t* function, int line, const std::wformat_string<_Args...>& format, _Args&&... args)
	{
		// 로그 객체를 획득
		Log* log = Log::Acquire();

		if (!log)
			return;

		log->Write(color, function, line, format, std::forward<_Args>(args)...);

		// 로그 출력 및 큐에 푸시
		PrintOutput(log->mStr);
		Push(log);
	}

private:
	void Push(Log* log);

public:
	Log* Pop(OUT size_t& count);
};

inline std::shared_ptr<LogQueue> gLogQueue = nullptr;
