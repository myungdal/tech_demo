// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Exception.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 내부 헬퍼 함수
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 덤프 파일명 생성 (타임스탬프 포함)
const wchar_t* MakeDumpFilename(const wchar_t* ext)
{
	tm localTm{};
	_localtime64_s(&localTm, &tClock.GetGlobalNowTt());

	const int dumpFileYear = localTm.tm_year - 100;
	const int dumpFileMon = localTm.tm_mon + 1;
	const int dumpFileDay = localTm.tm_mday;
	const int dumpFileHour = localTm.tm_hour;
	const int dumpFileMin = localTm.tm_min;
	const int dumpFileSec = localTm.tm_sec;

	static wchar_t gDumpFilename[MAX_PATH] = { 0 };

	swprintf_s(
		gDumpFilename,
		L"../../dump/%s_%02d%02d%02d_%02d%02d%02d.%s",
		gAppName,
		dumpFileYear,
		dumpFileMon,
		dumpFileDay,
		dumpFileHour,
		dumpFileMin,
		dumpFileSec,
		ext
	);

	return gDumpFilename;
}

// 미니덤프 파일 생성
void MakeDumpFile(const wchar_t* dumpFilename, EXCEPTION_POINTERS* e)
{
	const HANDLE dumpFileHandle = CreateFileW(
		dumpFilename, 
		GENERIC_WRITE, 
		FILE_SHARE_READ, 
		nullptr, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		nullptr
	);

	if (INVALID_HANDLE_VALUE == dumpFileHandle)
		return;

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
	exceptionInfo.ThreadId = GetCurrentThreadId();
	exceptionInfo.ExceptionPointers = e;
	exceptionInfo.ClientPointers = FALSE;

	MiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		dumpFileHandle,
		static_cast<MINIDUMP_TYPE>(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory | MiniDumpWithFullMemory),
		e ? &exceptionInfo : nullptr,
		nullptr,
		nullptr
	);

	FlushFileBuffers(dumpFileHandle);
	CloseHandle(dumpFileHandle);
}

// 콜스택 텍스트 파일에 기록
void WriteCallStack(std::wofstream& outFile, HANDLE threadHandle, CONTEXT* context)
{
	char buf[1024];

	sprintf_s(buf, "ThreadId: %lu", GetThreadId(threadHandle));
	outFile << buf << std::endl << std::endl;
	outFile.flush();

	constexpr int MAX_STACK_TRACE = 128;
	DWORD64 stackTrace[MAX_STACK_TRACE];
	memset(stackTrace, 0, sizeof(stackTrace));

	STACKFRAME64 stackFrame64;
	memset(&stackFrame64, 0, sizeof(stackFrame64));
	stackFrame64.AddrPC.Offset = context->Rip;
	stackFrame64.AddrPC.Mode = AddrModeFlat;
	stackFrame64.AddrStack.Offset = context->Rsp;
	stackFrame64.AddrStack.Mode = AddrModeFlat;
	stackFrame64.AddrFrame.Offset = context->Rbp;
	stackFrame64.AddrFrame.Mode = AddrModeFlat;

	for (int i = 0; i < MAX_STACK_TRACE; ++i)
	{
		const bool succeeded = StackWalk64(
			IMAGE_FILE_MACHINE_AMD64,
			GetCurrentProcess(),
			threadHandle,
			&stackFrame64,
			context,
			nullptr,
			SymFunctionTableAccess64,
			SymGetModuleBase64,
			nullptr
		);

		if (false == succeeded)
			break;

		if (0 == stackFrame64.AddrFrame.Offset)
			break;

		const DWORD64 address = stackFrame64.AddrPC.Offset;
		if (0 == address)
			break;

		char symbolBuffer[sizeof(IMAGEHLP_SYMBOL64) + 512];
		PIMAGEHLP_SYMBOL64 symbol = reinterpret_cast<PIMAGEHLP_SYMBOL64>(symbolBuffer);
		symbol->SizeOfStruct = sizeof(symbolBuffer);
		symbol->MaxNameLength = 512;

		DWORD displacement = 0;
		DWORD64 displacement64 = 0;

		if (!SymGetSymFromAddr64(GetCurrentProcess(), address, &displacement64, symbol))
			continue;

		int offset = 0;
		while (symbol->Name[offset] < 32)
			offset++;

		sprintf_s(buf, "\t%s()", static_cast<const char*>(symbol->Name + offset));
		outFile << buf << std::endl;
		
		IMAGEHLP_LINE64 line64;
		line64.SizeOfStruct = sizeof(line64);

		if (SymGetLineFromAddr64(GetCurrentProcess(), address, &displacement, &line64))
		{
			sprintf_s(buf, "\t\t%s(%d)", static_cast<const char*>(line64.FileName), line64.LineNumber);
			outFile << buf << std::endl;
		}		

		IMAGEHLP_MODULE64 imageHelpModule;
		imageHelpModule.SizeOfStruct = sizeof(imageHelpModule);
		if (SymGetModuleInfo64(GetCurrentProcess(), address, &imageHelpModule))
		{
			sprintf_s(buf, "\t\t%s", static_cast<const char*>(imageHelpModule.ImageName));
			outFile << buf << std::endl;
		}

		outFile << std::endl << std::endl;
		outFile.flush();
	}

	outFile << std::endl;
	outFile.flush();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 공개 함수
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 최상위 예외 필터 (덤프 및 콜스택 생성)
LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* e)
{
	if (IsDebuggerPresent())
		return EXCEPTION_CONTINUE_SEARCH;

	static std::atomic<int> cnt = 0;
	if (0 != (++cnt) - 1)
		return EXCEPTION_CONTINUE_SEARCH;

	const HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		ExitProcess(1);
	THREADENTRY32 te32;
	te32.dwSize = sizeof(THREADENTRY32);
	if (FALSE == Thread32First(hThreadSnap, &te32))
		ExitProcess(1);
	std::list<HANDLE> threadHandleList;
	do
	{
		if (te32.th32OwnerProcessID == GetCurrentProcessId())
		{
			HANDLE threadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);
			//if (GetCurrentThread() != threadHandle)
				//SuspendThread(threadHandle);
			threadHandleList.push_back(threadHandle);
		}			

	} while (Thread32Next(hThreadSnap, &te32));
	CloseHandle(hThreadSnap);

	const wchar_t* dumpFilename = MakeDumpFilename(L"dmp");
	MakeDumpFile(dumpFilename, e);

	const wchar_t* callStackFilename = MakeDumpFilename(L"txt");
	std::wofstream callStackFile(callStackFilename, std::ofstream::out);

	WriteCallStack(callStackFile, GetCurrentThread(), e->ContextRecord);

	for (HANDLE threadHandle : threadHandleList)
	{
		CONTEXT threadContext;
		ZeroMemory(&threadContext, sizeof(CONTEXT));
		threadContext.ContextFlags = CONTEXT_CONTROL;

		if (GetThreadContext(threadHandle, &threadContext))
			WriteCallStack(callStackFile, threadHandle, &threadContext);
		
		CloseHandle(threadHandle);
	}
	
	callStackFile.flush();
	callStackFile.close();

	ExitProcess(1);
}

// Pure Virtual 함수 호출 시 크래시 처리
void WINAPI PureCallHandler()
{
	Crash();
}

// 잘못된 파라미터 전달 시 처리
void WINAPI InvalidParameterHandler(
	const wchar_t*,
	const wchar_t*,
	const wchar_t*,
	uint32_t,
	uintptr_t
)
{
	//Crash();
}

// 예외 처리 시스템 초기화 (핸들러 등록 및 심볼 로드)
void InitException()
{
	SetUnhandledExceptionFilter(ExceptionFilter);
	_set_purecall_handler(PureCallHandler);
	_set_invalid_parameter_handler(InvalidParameterHandler);
	_CrtSetReportMode(_CRT_ASSERT, 0);

	SymSetOptions(SymGetOptions() | SYMOPT_LOAD_LINES);
	SymInitialize(GetCurrentProcess(), nullptr, TRUE);
}
