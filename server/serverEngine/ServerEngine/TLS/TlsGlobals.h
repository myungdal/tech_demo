// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// TlsGlobals.h - Thread Local Storage 전역 변수들
// pch_serverEngine.h에서 분리하여 모듈에서 export 가능하도록 함

#pragma once

#include "ServerEngine/Clock/TlsClock.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread 관련 타입
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 스레드 ID 타입
using ThreadId = uint32_t;

// 스레드 개수 타입
using ThreadCount = ThreadId;

// 최대 스레드 개수
inline constexpr ThreadCount MAX_THREAD_COUNT = 256;

// 유효하지 않은 스레드 ID
inline constexpr ThreadId INVALID_THREAD_ID = 255;

// ThreadWorker 스마트 포인터
using ThreadWorkerPtr = std::shared_ptr<class ThreadWorker>;

// ThreadWorker 포인터 리스트
using ThreadWorkerPtrList = std::vector<ThreadWorkerPtr>;

// Cron 작업 핸들 타입
using CronHandle = size_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread Local Storage 변수
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 현재 스레드 ID
thread_local inline ThreadId tThreadId = INVALID_THREAD_ID;

// 현재 실행 중인 Worker
thread_local inline std::shared_ptr<class Worker> tWorkingWorker = nullptr;

// Lock 중첩 카운트 (데드락 감지용)
thread_local inline int32_t tLockedCount = 0;

// 시간 관련 TLS
thread_local inline TlsClock tClock;
thread_local inline TlsClockInternalMembers tClockInternalMembers; // TlsClock 내부 접근 전용

// 랜덤 엔진 (32비트)
thread_local inline std::shared_ptr<class Mt19937Random32> tRandomEngine32 = nullptr;

// 랜덤 엔진 (64비트)
thread_local inline std::shared_ptr<class Mt19937Random64> tRandomEngine64 = nullptr;

// DB 저장 프로시저 버퍼 크기
inline constexpr size_t MAX_DB_SP_IN_BUF_SIZE = 256;
inline constexpr size_t MAX_DB_SP_OUT_BUF_SIZE = 256;

// DB 저장 프로시저 입력 버퍼 (SQLLEN 배열)
thread_local inline SQLLEN tDbSpInBuf[MAX_DB_SP_IN_BUF_SIZE];

// DB 저장 프로시저 출력 버퍼 (SQLLEN 배열)
thread_local inline SQLLEN tDbSpOutBuf[MAX_DB_SP_OUT_BUF_SIZE];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 전역 변수
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 전체 ThreadWorker 목록
inline ThreadWorkerPtrList gThreadWorkerPtrList;

// Worker 스레드 개수
inline ThreadId gWorkerThreadCount = 0;

