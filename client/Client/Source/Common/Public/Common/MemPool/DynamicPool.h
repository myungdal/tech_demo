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
// 동적 확장 메모리 풀 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 초과될 경우 1.5배로 확장되는 메모리 풀.
// - 책임: 객체 할당/해제 관리 (동적 확장)
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
template <typename _Type, size_t _DefaultCapacity = 1024, size_t _MaxCapacity = SIZE_MAX>
class DynamicPool final
{
private:
	using PtrList = std::vector<_Type*>;
	PtrList mPtrList;      // 메모리 풀 포인터 리스트
	size_t mPopPos = 0;    // Pop 위치
	size_t mPushPos = 0;   // Push 위치

public:
	DynamicPool()
	{
		UsageMeter<DynamicPool, _Type>::OnInit(_DefaultCapacity);

		ReservePool();
	}

	virtual ~DynamicPool()
	{
		for (_Type* objectPtr : mPtrList)
			if (objectPtr)
				_FREE(objectPtr);
	}

	// 풀 크기를 확장한다 (1.5배 증가).
	void ReservePool()
	{
		const size_t oldSize = mPtrList.size();
		size_t newSize = (_DefaultCapacity > oldSize) ? _DefaultCapacity : (oldSize + (oldSize >> 1) + 1);

		mPopPos = oldSize;
		mPushPos = 0;

		if (_MaxCapacity < newSize)
			newSize = _MaxCapacity;

		mPtrList.reserve(newSize);
		for (size_t i = oldSize; i < newSize; ++i)
			mPtrList.emplace_back(_MALLOC<_Type>(1));

		UsageMeter<DynamicPool, _Type>::OnChangeLimitation(newSize);
	}

	// 객체를 풀에서 할당한다. 풀이 비어있으면 확장.
	// - return: 할당된 객체 포인터. 실패 시 nullptr
	_Type* Pop()
	{
		if (true == IsEmpty())
			ReservePool();

		if (true == IsEmpty())
			return nullptr;

		const size_t pos = (mPopPos++) % mPtrList.size();

		_Type* objectPtr = mPtrList[pos];
		_ASSERT_CRASH(nullptr != objectPtr);

		// 생성자 호출
		new (objectPtr)_Type();

		mPtrList[pos] = nullptr;

		UsageMeter<DynamicPool, _Type>::OnIncrease();

		return objectPtr;
	}

	// 객체를 풀에 반환한다.
	// - objectPtr: 반환할 객체 포인터
	void Push(_Type* objectPtr)
	{
		const size_t pos = (mPushPos++) % mPtrList.size();
		_ASSERT_CRASH(nullptr == mPtrList[pos]);

		mPtrList[pos] = objectPtr;

		// 소멸자 호출
		objectPtr->~_Type();

		UsageMeter<DynamicPool, _Type>::OnDecrease();
	}

	// 풀이 비어있는지 확인한다.
	// - return: 비어있으면 true
	bool IsEmpty() const
	{
		if (true == mPtrList.empty())
			return true;

		const size_t pos = mPopPos % mPtrList.size();
		return (nullptr == mPtrList[pos]);
	}

	// 풀 크기를 반환한다.
	// - return: 풀 크기
	size_t GetPoolSize() const
	{
		return mPtrList.size();
	}
};
