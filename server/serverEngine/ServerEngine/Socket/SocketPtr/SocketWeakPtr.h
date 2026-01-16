// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketPtr/SocketRefCounter.h"
#include "ServerEngine/Socket/SocketPtr/SocketPtr.h"


template<typename _SocketType>
class SocketWeakPtr
{
protected:
	using _SharedPtr = SocketPtr<_SocketType>;

private:
	std::atomic<_SocketType*> mPtr;
	SocketRefCounterPtr mSocketRefCounterPtr;
	const wchar_t* mReason;

public:
	SocketWeakPtr() : mPtr(nullptr), mSocketRefCounterPtr(nullptr), mReason(L"") {}

	explicit SocketWeakPtr(const _SharedPtr& sharedPtr)
	{
		if (sharedPtr)
		{
			_SocketType* rawPtr = *sharedPtr;
			mPtr.store(rawPtr);
			mSocketRefCounterPtr = rawPtr->mSocketRefCounterPtr;
			mReason = sharedPtr.mReason;
		}
		else
		{
			mPtr.store(nullptr);
			mSocketRefCounterPtr = nullptr;
			mReason = L"";
		}
	}

	explicit SocketWeakPtr(const SocketWeakPtr& weakPtr)
	{
		_SocketType* rawPtr = weakPtr.mPtr.load();
		mPtr.store(rawPtr);
		mSocketRefCounterPtr = weakPtr.mSocketRefCounterPtr;
		mReason = weakPtr.mReason;
	}

	explicit SocketWeakPtr(const _SocketType* ptr, const wchar_t* reason)
	{
		if (ptr)
		{
			mPtr.store(ptr);
			mSocketRefCounterPtr = ptr->mSocketRefCounterPtr;
			mReason = reason;
		}
		else
		{
			mPtr.store(nullptr);
			mSocketRefCounterPtr = nullptr;
			mReason = L"";
		}
	}

	SocketWeakPtr(_SocketType* ptr)
	{
		if (ptr)
		{
			mPtr.store(ptr);
			mSocketRefCounterPtr = mPtr.load()->mSocketRefCounterPtr;
			mReason = L"";
		}
		else
		{
			mPtr.store(nullptr);
			mSocketRefCounterPtr = nullptr;
			mReason = L"";
		}
	}

	SocketWeakPtr& operator=(const _SharedPtr& sharedPtr)
	{
		if (sharedPtr)
		{
			_SocketType* rawPtr = *sharedPtr;
			mPtr.store(rawPtr);
			mSocketRefCounterPtr = rawPtr->mSocketRefCounterPtr;
			mReason = sharedPtr.mReason;
		}
		else
		{
			mPtr.store(nullptr);
			mSocketRefCounterPtr = nullptr;
			mReason = L"";
		}

		return *this;
	}

	SocketWeakPtr& operator=(const SocketWeakPtr& weakPtr)
	{
		// 자신을 대입한 경우.
		if (this == &weakPtr)
			return *this;

		_SocketType* rawPtr = weakPtr.mPtr.load();
		mPtr.store(rawPtr);
		mSocketRefCounterPtr = weakPtr.mSocketRefCounterPtr;
		mReason = weakPtr.mReason;

		return *this;
	}

	SocketWeakPtr& Set(_SocketType* ptr, const wchar_t* reason)
	{
		if (ptr)
		{
			mPtr.store(ptr);
			mSocketRefCounterPtr = ptr->mSocketRefCounterPtr;
			mReason = reason;
		}
		else
		{
			mPtr.store(nullptr);
			mSocketRefCounterPtr = nullptr;
			mReason = L"";
		}

		return *this;
	}

	SocketWeakPtr& operator=(_SocketType* ptr)
	{
		if (mPtr.load() == ptr)
			return *this;

		if (ptr)
		{
			mPtr.store(ptr);
			mSocketRefCounterPtr = ptr->mSocketRefCounterPtr;
			mReason = L"";
		}
		else
		{
			mPtr.store(nullptr);
			mSocketRefCounterPtr = nullptr;
			mReason = L"";
		}

		return *this;
	}

	SocketPtr<_SocketType> Lock()
	{
		_SocketType* rawPtr = mPtr.load();
		if (nullptr == rawPtr)
			return {};

		if (!mSocketRefCounterPtr || !mSocketRefCounterPtr->IncreaseRefCnt())
		{
			mPtr.store(nullptr);
			return {};
		}

		SocketPtr<_SocketType> ptr(rawPtr, mReason);
		mSocketRefCounterPtr->DecreaseRefCnt();
		return ptr;
	}
};
