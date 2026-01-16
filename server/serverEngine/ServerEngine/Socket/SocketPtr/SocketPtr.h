// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


template<typename _SocketType>
class SocketPtr final
{
	template<typename _SocketType> friend class SocketWeakPtr;

private:
	_SocketType* mPtr = nullptr;
	const wchar_t* mReason = L"";	

public:
	SocketPtr() = default;
	SocketPtr(_SocketType* ptr, const wchar_t* reason)
	{
		mPtr = ptr;
		mReason = reason;

		mPtr->AddRef(mReason);
	}

	SocketPtr(const SocketPtr& ptr)
	{
		if (!ptr.mPtr)
			return;

		mPtr = ptr.mPtr;
		mReason = ptr.mReason;

		mPtr->AddRef(mReason);
	}

	~SocketPtr()
	{
		if (nullptr != mPtr)
		{
			mPtr->ReleaseRef(mReason);
			mPtr = nullptr;
		}
	}

	SocketPtr& Set(_SocketType* ptr, const wchar_t* reason)
	{
		mPtr = ptr;
		mReason = reason;

		mPtr->AddRef(mReason);

		return *this;
	}

	SocketPtr& operator=(const SocketPtr& ptr)
	{
		if (ptr.mPtr)
		{
			mPtr = ptr.mPtr;
			mReason = ptr.mReason;
			mPtr->AddRef(mReason);
		}
		else if (mPtr)
		{
			mPtr->ReleaseRef(mReason);
			mPtr = nullptr;
			mReason = L"";
		}
		else
		{
			mReason = L"";
		}

		return *this;
	}

public:
	bool IsNull() const { return (nullptr == mPtr) ? true : false; }
	bool IsNotNull() const { return (nullptr != mPtr) ? true : false; }
	const wchar_t* GetReason() const { return mReason; }

	operator _SocketType* () const { return mPtr; }

	_SocketType* operator*() const noexcept { return mPtr; }
	_SocketType* operator->() const noexcept { return mPtr; }
};

