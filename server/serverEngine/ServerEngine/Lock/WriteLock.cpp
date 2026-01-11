// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "WriteLock.h"

import ServerEngineCore;


WriteLock::WriteLock(Lock& lock) : mLock(&lock)
{
	if (nullptr != mLock)
		mLock->EnterWriteLock();
}
WriteLock::WriteLock(WriteLock&& other) noexcept : mLock(other.mLock)
{
	other.mLock = nullptr;
}
WriteLock& WriteLock::operator=(WriteLock&& other) noexcept
{
	if (this != &other)
	{
		// 기존 락 해제
		if (nullptr != mLock)
			mLock->LeaveWriteLock();

		// 대상 락 인수
		mLock = other.mLock;
		other.mLock = nullptr;
	}
	return *this;
}

WriteLock::~WriteLock()
{
	if (nullptr != mLock)
	{
		mLock->LeaveWriteLock();
		mLock = nullptr;
	}	
}
