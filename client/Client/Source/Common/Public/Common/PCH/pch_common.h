// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#if defined(__UNREAL__)
#	include "CoreMinimal.h"
#else
#	define COMMON_API
#	define MMOSYNC_API
#	define PACKET_API
#	define SHAREDLIB_API
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// tcmalloc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(__UNREAL__) && !defined(_DEBUG)
#	pragma comment(lib, "libtcmalloc_minimal")
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 개발 편의
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr unsigned short RED = 3; // 에러
constexpr unsigned short GREEN = 2; // 알림
constexpr unsigned short YELLOW = 1; // 정보
constexpr unsigned short WHITE = 0; // 개발

#define _SHOW_LOG_COLOR_LEVEL WHITE
#define _PACKET_SEND_LOG 1
#define _PACKET_RECV_LOG 1
#define _BOT_HISTORY_SEND_RECV 0
#define _SOCKET_HISTORY_REF_COUNT 0
#define _SOCKET_HISTORY_CONNECTION 0

#define _DETECT_MEMORY_VIOLATION 0 // 바이트 정렬이 되지 않아서 오류가 발생한다, 수정하기 전에는 사용 금지

#if defined(__UNREAL__)
#	undef _DETECT_MEMORY_VIOLATION 
#	define _DETECT_MEMORY_VIOLATION 0
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 디버그 레벨
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _HAS_ITERATOR_DEBUGGING
#	undef _HAS_ITERATOR_DEBUGGING
#endif
#define _HAS_ITERATOR_DEBUGGING 0

#ifdef _ITERATOR_DEBUG_LEVEL
#	undef _ITERATOR_DEBUG_LEVEL
#endif
#define _ITERATOR_DEBUG_LEVEL 0

#ifdef _MSVC_STL_HARDENING
#	undef _MSVC_STL_HARDENING
#endif
#define _MSVC_STL_HARDENING 0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 안드로이드 IOS 컴파일 문제
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(__UNREAL__)
#	ifdef DLLEXPORT
#		define _DLL_EXPORT DLLEXPORT
#	else
#		define _DLL_EXPORT __attribute__((visibility("default")))
#	endif
#else
#	define _DLL_EXPORT
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 많이 사용하는 외부 헤더
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <stdint.h>
#include <math.h>

#include <cstdlib>

#ifndef __ANDROID__
#	include <crtdbg.h>
#endif

#include <cwchar>
#include <cwctype>
#include <stdarg.h>

#include <limits>
#include <utility>
#include <list>
#include <vector>
#include <deque>
#include <stack>
#include <array>
#include <bitset>
#include <functional>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>
#include <atomic>
#include <mutex>
#include <future>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <regex>
#include <optional>
#include <algorithm>
#include <typeinfo>
#include <typeindex>
#include <locale>
#include <codecvt>
#include <exception>

#if !defined(__UNREAL__)
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <winsock2.h>
#	include <mswsock.h>
#	include <mstcpip.h>
#	include <ws2tcpip.h>
#	include <iphlpapi.h>
#	include <windows.h>
#	include <process.h>
#	include <conio.h>
#	include <fstream>
#	include <filesystem>
#	include <sql.h>
#	include <sqlext.h>
#	include <dbghelp.h>
#	include <tlhelp32.h>
#	include <iomanip> 
#	include <winhttp.h>
#	include <format>
#	include <thread>
#endif

#include "Common/StdHelper/StdHelper.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// tls
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 패킷 로그 출력용 탭
thread_local inline std::wstring tTabStr{ L"" };
thread_local inline class WorldManager* tWorldManager = nullptr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 시간
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace std::chrono;

// 시간, uint64_t, time_t로 변환
using ClockPoint = system_clock::time_point;

constexpr ClockPoint INVALID_CLOCK_POINT = system_clock::time_point::min();

// 초, double, 1.230s
using ClockDuration = duration<double>;
// 밀리초, uint64_t, 1230ms
using ClockMs = milliseconds;
constexpr ClockMs INVALID_CLOCK_MS = milliseconds::max();

#define CLOCK_NOW clock_cast<system_clock>(utc_clock::now())
#define CLOCK_POINT_TO_TT(x) system_clock::to_time_t(x)
#define CLOCK_POINT_FROM_TT(x) system_clock::from_time_t(x)
#define CLOCK_MS_FROM_DR(x) duration_cast<ClockMs>(x)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  게임 시간
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using TimeStep = uint64_t;
constexpr TimeStep INVALID_TIME_STEP = 0;
constexpr TimeStep STOP_TIME_STEP = UINT64_MAX;
constexpr int64_t TIME_STEP_INTERVAL_FACTOR = 4;
constexpr int64_t TIME_STEP_INTERVAL = 16; // 엔진은 해당 ms 마다 업데이트 된다.
constexpr TimeStep TIME_STEP_GENERATOR_LIMIT = 8; // TimeStepGenerator가 한번에 생성하는 TIME_STEP_INTERVAL 생성 개수, 16 * 8 = 128ms에 한번 전송을 한다
using TimeStepUpdateCycle = uint16_t; // 1 이라면, TIME_STEP_INTERVAL 마다 실행되는 시스템이다, 1000 이라면, TIME_STEP_INTERVAL 초 마다 실행되는 시스템이다.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 수학
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using Angle = int16_t;
constexpr Angle ANGLE_0 = 0;	//   0°
constexpr Angle ANGLE_30 = 5461;	//  30°
constexpr Angle ANGLE_45 = 8192;	//  45°
constexpr Angle ANGLE_60 = 10923;	//  60°
constexpr Angle ANGLE_90 = 16384;	//  90°
constexpr Angle ANGLE_120 = 21845;	// 120°
constexpr Angle ANGLE_135 = 24576;	// 135°
constexpr Angle ANGLE_150 = 27307;	// 150°
constexpr Angle ANGLE_180 = -32768;	// 180°
constexpr Angle ANGLE_210 = -27307;	// 210°
constexpr Angle ANGLE_225 = -24576;	// 225°
constexpr Angle ANGLE_240 = -21845;	// 240°
constexpr Angle ANGLE_270 = -16384;	// 270°
constexpr Angle ANGLE_300 = -10923;	// 300°
constexpr Angle ANGLE_315 = -8192;	// 315°
constexpr Angle ANGLE_330 = -5461;	// 330°

constexpr float FLOAT_PI = (3.1415926535897932f);
constexpr float FLOAT_PI_HALF = (FLOAT_PI * 0.5f);
constexpr float ANGLE_TO_RADIAN = (FLOAT_PI / 32768.0f);      // 미리 계산해 두면 곱셈 1회
constexpr float RADIAN_TO_ANGLE = (32768.0f / FLOAT_PI);      // 미리 계산해 두면 곱셈 1회

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 자주 사용하는 외부 헤더
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Common/Dev/macro.h"
#include "Common/Dev/debug.h"

#include "Common/Key/Checksum/Checksum.h"
#include "Common/Key/StringKey/StringKey.h"
#include "Common/Key/Uuid/Uuid.h"
#include "Common/Key/RpcId/RpcId.h"

#include "Common/PCH/pch_world.h"
