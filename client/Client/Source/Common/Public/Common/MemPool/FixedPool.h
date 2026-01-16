// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/UsageMeter/UsageMeter.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 고정 크기 메모리 풀 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 정해진 개수 안에서 고성능, 스레드 안전한 메모리 풀.
// - 책임: 객체 할당/해제 관리 (풀 초과 시 New/Delete 사용)
// - 스레드 모델: 스레드 안전 (lock-free)
template <typename _Type, size_t _PoolSizeFactor>
class FixedPool final
{
private:
	static constexpr size_t POOL_SIZE = (1 << _PoolSizeFactor);      // 풀 크기 (2^_PoolSizeFactor)
	static constexpr size_t POOL_SIZE_MASK = (POOL_SIZE - 1);        // 모듈로 연산용 마스크

	std::atomic<_Type*> mPool[POOL_SIZE] = {};  // 메모리 풀 배열
	std::atomic<size_t> mPopPos{ 0 };           // Pop 위치 (원자적 카운터)
	std::atomic<size_t> mPushPos{ 0 };          // Push 위치 (원자적 카운터)

public:
	FixedPool()
	{
		UsageMeter<FixedPool, _Type>::OnInit(POOL_SIZE);

		for (size_t i = 0; i < POOL_SIZE; ++i)
		{
			_Type* ptr = _MALLOC<_Type>(1);
			// memory_order(relaxed): 초기화 단계에서 단순 포인터 저장(동기화 불필요).
			mPool[i].store(ptr, std::memory_order_relaxed);
		}
	}

	~FixedPool()
	{
		for (size_t i = 0; i < POOL_SIZE; ++i)
		{
			// memory_order(relaxed): 소멸 시점에는 외부 동시 사용이 없다는 가정 하에 단순 회수.
			_Type* ptr = mPool[i].load(std::memory_order_relaxed);
			if (ptr)
				_FREE(ptr);
		}
	}

	// 객체를 풀에서 할당한다. 풀이 비어있으면 새로 할당.
	// - return: 할당된 객체 포인터
	_Type* Pop()
	{
		// [PERF] memory_order(relaxed): 인덱스 분산(슬롯 선택)용 카운터는 원자성만 있으면 됨
		size_t popPos = mPopPos.fetch_add(1, std::memory_order_relaxed);
		popPos &= POOL_SIZE_MASK;
		// [PERF] memory_order(acquire): Push에서 release로 publish한 포인터를 "획득"하는 핸드오프 지점
		// (다음 스레드가 이 포인터를 받은 뒤, publish 이전의 쓰기를 관측하도록)
		_Type* before = mPool[popPos].exchange(nullptr, std::memory_order_acquire);

		if (nullptr == before)
			before = _MALLOC<_Type>(1);

		::new (before)_Type(); // 생성자 호출

		UsageMeter<FixedPool, _Type>::OnIncrease();
		return before;
	}

	// 객체를 풀에 반환한다. 풀이 가득 차면 해제.
	// - ptr: 반환할 객체 포인터
	void Push(_Type* ptr)
	{
		// [PERF] memory_order(relaxed): 인덱스 분산(슬롯 선택)용 카운터는 원자성만 있으면 됨
		size_t pushPos = mPushPos.fetch_add(1, std::memory_order_relaxed);
		pushPos &= POOL_SIZE_MASK;

		// 소멸자 호출
		ptr->~_Type();

		_Type* expected = nullptr;
		// [PERF] memory_order:
		// - 성공(release): 슬롯에 포인터를 publish(이 포인터를 Pop-acquire가 안전하게 획득)
		// - 실패(relaxed): 실패 시에는 _FREE로 폐기(재시도/다른 공유데이터 동기화가 없음)
		if (false == mPool[pushPos].compare_exchange_strong(expected, ptr, std::memory_order_release, std::memory_order_relaxed))
			_FREE(ptr);

		UsageMeter<FixedPool, _Type>::OnDecrease();
	}
};
