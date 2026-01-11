// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// Worker.h - Worker 기본 클래스
#pragma once

#include "ServerEngine/Worker/WorkerTask/WorkerTask.h"
#include "ServerEngine/Util/MwsrQueue/MwsrQueue.h"
#include "ServerEngine/Worker/WorkerImpl/CoWorker.h"


class WorkerTaskNode;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Worker - 함수 호출 큐잉 기반 비동기 작업 처리 단위
//
// 핵심 설계:
// - 각 Worker는 독립적인 함수 호출 정보 큐를 소유
// - Lock-free MWSR(Multi-Writer Single-Reader) 큐로 동시 작업 추가 지원
// - 동기 실행 최적화: 같은 Worker 내 호출 시 직접 실행 (컨텍스트 스위칭 제거)
//
// 실행 흐름:
// 1. PushTask(): 작업을 큐에 추가
// 2. mTaskCount == 1 (첫 작업): TryWork() 호출로 즉시 실행 시도
// 3. TryWork(): 현재 스레드가 유휴 상태면 작업 처리, 아니면 CoWorker에 위임
//
// 스레드 안전성:
// - mTaskQueue: Lock-free, 여러 스레드에서 동시 Push 가능
// - mTaskCount: atomic, 작업 개수 추적 및 실행 트리거
// - tWorkingWorker: TLS, 현재 스레드가 처리 중인 Worker 추적
//
// [WHY] 왜 함수 호출 큐잉인가?
// - 전통적인 Lock 기반 동기화의 데드락, 성능 저하 문제 회피
// - 함수 호출을 큐에 저장하여 순차 실행, 자연스러운 비동기 프로그래밍 모델 제공
// - 게임 서버의 세션/채널/유저 단위 처리에 적합
//
// [PERF] 성능 특성:
// - Lock-free 큐로 경합 최소화
// - 동기 실행 최적화로 같은 Worker 내 연속 호출 시 오버헤드 제거
// - weak_ptr 사용으로 Worker 수명 관리 및 안전한 지연 실행
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Worker 기본 클래스 (작업 큐 관리 및 실행)
class Worker : public std::enable_shared_from_this<Worker>
{
	friend class WorkerManager;
	friend class CoWorker;
	friend class Lock;

private:
	// 작업 큐 (Multi-Writer Single-Reader Lock-free Queue)
	// [PERF] Windows Interlocked SLIST 기반, CAS 연산으로 Lock 없이 동시 Push
	MwsrQueue<WorkerTaskNode> mTaskQueue;
	
	// 대기 중인 작업 개수 (atomic)
	// [WHY] 첫 작업 도착 시 실행 트리거, 마지막 작업 완료 시 종료 판단
	std::atomic<size_t> mTaskCount{ 0 };

public:
	Worker();
	virtual ~Worker();

	// PushTask - 작업 추가 및 실행 시도
	// 동기 최적화: 현재 스레드가 유휴 상태면 즉시 실행
	void PushTask(WorkerTaskNode* task);
	
	// PushTaskAsyncForced - 작업 추가 (항상 비동기)
	// [WHY] 재귀 호출 방지, 호출 스택 깊이 제한 필요 시 사용
	void PushTaskAsyncForced(WorkerTaskNode* task);

private:
	// TryWork - 작업 처리 시도
	// 조건: tWorkingWorker == nullptr && tLockedCount == 0
	// 실패 시 CoWorker에 위임하여 다른 스레드가 처리하도록 함
	void TryWork();
};
