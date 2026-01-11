// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "HAL/CriticalSection.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 초과될 경우 1.5배로 확장되는 메모리 풀
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Type, size_t _DefaultCapacity = 1024, size_t _MaxCapacity = SIZE_MAX>
class FClientDynamicPool final
{
private:
	using PtrList = std::vector<_Type*>;
	PtrList mPtrList;
	size_t mPopPos = 0;
	size_t mPushPos = 0;

	mutable FCriticalSection mLock;

public:
	FClientDynamicPool()
	{
		FScopeLock lock(&mLock);
		
		ReservePool();
	}

	~FClientDynamicPool()
	{
		FScopeLock lock(&mLock);
	
		for (_Type* objectPtr : mPtrList)
			if (objectPtr)
				_FREE(objectPtr);
	}

	void ReservePool()
	{
		FScopeLock lock(&mLock);
		
		const size_t oldSize = mPtrList.size();
		size_t newSize = (_DefaultCapacity > oldSize) ? _DefaultCapacity : (oldSize + (oldSize >> 1) + 1);

		mPopPos = oldSize;
		mPushPos = 0;

		if (_MaxCapacity < newSize)
			newSize = _MaxCapacity;

		mPtrList.reserve(newSize);
		for (size_t i = oldSize; i < newSize; ++i)
			mPtrList.emplace_back(_MALLOC<_Type>(1));
	}

	_Type* Pop()
	{
		FScopeLock lock(&mLock);
		
		if (true == IsEmpty())
			ReservePool();

		if (true == IsEmpty())
			return nullptr;

		const size_t pos = (mPopPos++) % mPtrList.size();

		_Type* ptr = mPtrList[pos];
		_ASSERT_CRASH(nullptr != ptr);

		// 생성자 호출
		new (ptr)_Type();

		mPtrList[pos] = nullptr;

		return ptr;
	}

	void Push(_Type* objectPtr)
	{
		FScopeLock lock(&mLock);
		
		const size_t pos = (mPushPos++) % mPtrList.size();
		_ASSERT_CRASH(nullptr == mPtrList[pos]);

		mPtrList[pos] = objectPtr;

		// 소멸자 호출
		objectPtr->~_Type();
	}

	bool IsEmpty() const
	{
		FScopeLock lock(&mLock);
		
		if (true == mPtrList.empty())
			return true;

		const size_t pos = mPopPos % mPtrList.size();
		return (nullptr == mPtrList[pos]);
	}

	size_t GetPoolSize() const
	{
		FScopeLock lock(&mLock);
	
		return mPtrList.size();
	}
};
