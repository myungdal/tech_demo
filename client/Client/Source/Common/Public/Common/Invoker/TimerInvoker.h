// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Invoker.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 타이머 기반 함수 호출자 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 지연 시간 후 함수를 호출하는 클래스.
// - 책임: 시간 기반 지연 호출 관리
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
template <typename... _Args>
class TimerInvoker
{
public:
	using ArgTuple = std::tuple<std::decay_t<_Args>...>;

private:
	Invoker<_Args...> mInvoker;                          // 실제 호출자
	ArgTuple mArgTuple;                                  // 저장된 인자 튜플
	ClockPoint mRevervedTime = INVALID_CLOCK_POINT;     // 예약된 호출 시간

public:
	// 지연 시간 후 함수 호출을 예약한다.
	// - now: 현재 시간
	// - delay: 지연 시간
	// - args: 전달할 인자들
	void Invoke(ClockPoint now, ClockMs delay, _Args&&... args)
	{
		if (INVALID_CLOCK_POINT != mRevervedTime)
			return;

		mRevervedTime = now + delay;
		mArgTuple = std::forward_as_tuple(args...);
	}

	// 예약된 호출을 취소한다.
	void CancelInvoke()
	{
		mRevervedTime = INVALID_CLOCK_POINT;
	}

	// 호출할 함수를 추가한다.
	// - owner: 함수를 소유하는 객체 포인터
	// - function: 호출할 함수
	// - return: Invoker 참조
	template <typename _Owner, typename _Function>
	Invoker<_Args...>& Add(_Owner* owner, _Function&& function)
	{
		return mInvoker.Add(owner, std::forward<_Function>(function));
	}

public:
	// 시간을 업데이트하고 예약된 호출을 실행한다.
	// - now: 현재 시간
	void Update(ClockPoint now)
	{
		if (INVALID_CLOCK_POINT == mRevervedTime)
			return;

		if (mRevervedTime > now)
			return;
	
		mRevervedTime = INVALID_CLOCK_POINT;
		CallFunction(std::index_sequence_for<_Args...>());
	}

private:
	// 저장된 인자로 함수를 호출한다.
	template <std::size_t... _Index>
	void CallFunction(std::index_sequence<_Index...>)
	{
		mInvoker.Invoke(std::forward<_Args>(std::get<_Index>(mArgTuple))...);
	}

public:
	// 등록된 모든 함수를 제거한다.
	void Clear()
	{
		mInvoker.Clear();
	}
};
