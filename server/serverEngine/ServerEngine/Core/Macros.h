// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// Macros.h - 기본 매크로 정의
// (모듈 export 대상 아님 - 전역 매크로)

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 파일 경로
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 문자열 결합 매크로 (컴파일 타임)
#define MARGE_FILE_PATH(a, b) a ## b

// 서버 데이터 루트 경로
#define SERVER_DATA_PATH "../../data/server/"

// 서버 데이터 파일 경로 생성
#define MAKE_SERVER_DATA_FILE_PATH(filename) MARGE_FILE_PATH(SERVER_DATA_PATH, filename)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 성능 추적
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// _PERFORMANCE_TRACE_START: 함수 시작부에 배치
// _PERFORMANCE_TRACE: 추적할 스코프(주로 함수 본문)에 배치
#ifdef _DEBUG
#   define _PERFORMANCE_TRACE_START PerformanceRecorderContainer _performanceRecorderContainer
#   define _PERFORMANCE_TRACE PerformanceTracer _performanceTracer(__FUNCTIONW__)
#else
#   define _PERFORMANCE_TRACE_START __noop
#   define _PERFORMANCE_TRACE __noop
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread Yield
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 스레드 양보 매크로 (상황별 yield 정책)
// 대안: SwitchToThread(), Sleep(1), YieldProcessor(), std::this_thread::yield() 등
#define _YIELD_APP Sleep(1)
#define _YIELD_CONSOLE_THREAD Sleep(1)
#define _YIELD_WORKER_THREAD Sleep(1)
#define _YIELD_SPIN std::this_thread::yield()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 로그
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef _DEBUG_LOG
#undef _DEBUG_RED
#undef _DEBUG_WHITE

// 컬러 로그 출력 (함수명/라인 번호 자동 포함)
#define _DEBUG_LOG(color, format, ...) gLogQueue->Push(color, __FUNCTIONW__, __LINE__, format, ##__VA_ARGS__)

// 빨간색 마커(빈 문자열 로그)
#define _DEBUG_RED gLogQueue->Push(RED, __FUNCTIONW__, __LINE__, L"")

// 흰색 마커(빈 문자열 로그)
#define _DEBUG_WHITE gLogQueue->Push(WHITE, __FUNCTIONW__, __LINE__, L"")

