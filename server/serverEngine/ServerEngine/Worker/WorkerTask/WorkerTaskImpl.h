// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WorkerTaskImpl - 작업 노드 템플릿 구현
//
// 설계 목표:
// - 임의의 멤버 함수와 인자를 캡처하여 지연 실행
// - 타입 안전성 보장 (컴파일 타임 검증)
// - Worker 수명 관리 (weak_ptr 사용)
//
// 템플릿 파라미터:
// - _BaseClass: 기본 클래스 (WorkerTaskNode)
// - _Worker: Worker 파생 타입
// - _Owner: 멤버 함수 소유 클래스
// - _Function: 멤버 함수 포인터 타입
// - _Args...: 함수 인자 타입들
//
// 캡처 전략:
// - Worker: weak_ptr로 참조 (수명 안전성)
// - Owner: raw pointer (Worker가 Owner 수명 관리)
// - Function: 멤버 함수 포인터
// - Args: std::tuple로 값 캡처 (std::decay_t 적용)
//
// [WHY] 왜 std::function을 안 쓰는가?
// - 메모리 풀링과 함께 사용하기 어려움
// - 타입 소거 비용 회피
// - 고정 크기 캡처로 풀 관리 용이
//
// [PERF] 성능 특성:
// - Perfect forwarding으로 불필요한 복사 제거
// - index_sequence로 튜플 언팩 최적화
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Worker;

template <typename _BaseClass, typename _Worker, typename _Owner, typename _Function, typename... _Args>
class WorkerTaskImpl : public _BaseClass
{
public:
	using Function = std::decay_t<_Function>;
	using ArgTuple = std::tuple<std::decay_t<_Args>...>;

protected:
	std::weak_ptr<_Worker> mWorkerWeakPtr;	
	_Owner* mOwner = nullptr;
	Function mFunction = nullptr;
	ArgTuple mArgTuple;

public:
	WorkerTaskImpl() = default;
	explicit WorkerTaskImpl(std::shared_ptr<_Worker> workerPtr, _Owner* owner, _Function&& function, _Args&&... args)
		:
		mWorkerWeakPtr(workerPtr),
		mOwner(owner),
		mFunction(std::forward<_Function>(function)),
		mArgTuple(std::forward<_Args>(args)...)
	{
	}

public:
	void Initialize(std::shared_ptr<_Worker> workerPtr, _Owner* owner, _Function&& function, _Args&&... args)
	{
		mWorkerWeakPtr = workerPtr;
		mOwner = owner;
		mFunction = std::forward<_Function>(function);
		mArgTuple = std::forward_as_tuple(args...);
		//mArguments = std::make_tuple(std::forward<_Args>(args)...);
	}

	virtual void Release()
	{
		mWorkerWeakPtr.reset();
		mOwner = nullptr;
		mFunction = nullptr;
		mArgTuple = {};
	}

public:
	void Execute() override
	{
		if (auto workerPtr = mWorkerWeakPtr.lock())
		{
			CallFunction(std::index_sequence_for<_Args...>());
		}
	}

	void ExecuteOnThread() override
	{
		CallFunction(std::index_sequence_for<_Args...>());
	}

protected:
	template <std::size_t... _Index>
	void CallFunction(std::index_sequence<_Index...>)
	{
		(mOwner->*mFunction)(std::forward<_Args>(std::get<_Index>(mArgTuple))...);
	}
};
