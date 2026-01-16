// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ReadLock.h"

import ServerEngineCore;


ReadLock::ReadLock(Lock& lock) : mLock(&lock)
{
	if (nullptr != mLock)
		mLock->EnterReadLock();
}
ReadLock::ReadLock(ReadLock&& other) noexcept : mLock(other.mLock)
{
	other.mLock = nullptr;
}
ReadLock& ReadLock::operator=(ReadLock&& other) noexcept
{
	if (this != &other)
	{
		// 기존 락 해제
		if (nullptr != mLock)
			mLock->LeaveReadLock();

		// 대상 락 인수
		mLock = other.mLock;
		other.mLock = nullptr;
	}
	return *this;
}

ReadLock::~ReadLock()
{
	if (nullptr != mLock)
	{
		mLock->LeaveReadLock();
		mLock = nullptr;
	}	
}
