// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WorkerTaskNode - 작업 노드 기본 인터페이스
//
// 설계 목표:
// - Lock-free 큐(SLIST)에서 사용 가능한 작업 노드 정의
// - 다양한 실행 컨텍스트 지원 (Worker, Thread, Timer)
//
// 상속 구조:
// SLIST_ENTRY (Windows Interlocked SLIST)
//   └─ WorkerTaskNode (인터페이스)
//         └─ WorkerTaskImpl<...> (템플릿 구현)
//               └─ WorkerTask<...> (메모리 풀링 지원)
//
// 인터페이스:
// - Execute(): Worker 컨텍스트에서 실행 (Worker 존재 확인)
// - ExecuteOnThread(): 스레드 컨텍스트에서 실행 (Worker 무관)
// - Dispose(): 메모리 풀에 반환
// - Run(): Worker에 작업 재등록 (Timer에서 사용)
//
// [WHY] 왜 가상 함수인가?
// - Lock-free 큐는 타입 소거(type erasure) 필요
// - 템플릿 인스턴스화된 구체 타입을 기본 포인터로 관리
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WorkerTaskNode : public SLIST_ENTRY
{
public:
	WorkerTaskNode() = default;
	virtual ~WorkerTaskNode() = default;

public:
	virtual void Execute() = 0;
	virtual void ExecuteOnThread() = 0;
	virtual void Dispose() = 0;
	virtual void Run() = 0;

public:
	WorkerTaskNode* GetNext() const noexcept { return static_cast<WorkerTaskNode*>(Next); }
};
