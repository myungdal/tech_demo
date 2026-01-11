// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// Lock.h - Reader-Writer Lock 구현
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reader-Writer Lock
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 읽기/쓰기 락 (다중 읽기, 단일 쓰기 지원)
class Lock
{
private:
	// 락 플래그 비트 마스크
	static constexpr uint32_t WRITE_LOCK_FLAG = (static_cast<uint32_t>(1) << 31);
	static constexpr uint32_t WRITE_LOCK_THREAD_ID_MASK = (0xFFFF0000 & ~WRITE_LOCK_FLAG);
	static constexpr uint32_t READ_LOCK_COUNT_MASK = 0x0000FFFF;
	
	// 스핀락 최대 시도 횟수
	static constexpr uint32_t MAX_SPIN_CNT = 10000;

private:
	// 락 상태 플래그 (읽기 카운트 + 쓰기 플래그)
	std::atomic<uint32_t> mLockFlag = 0;
	
	// 쓰기 락 재진입 카운트
	std::atomic<int32_t> mWriteLockCount = 0;

public:
	Lock();
	~Lock();

	// 쓰기 락 획득
	void EnterWriteLock();
	
	// 쓰기 락 해제
	void LeaveWriteLock();

	// 읽기 락 획득
	void EnterReadLock();
	
	// 읽기 락 해제
	void LeaveReadLock();

	// 읽기 락 상태 확인 (디버그용)
	bool IsReadLocked() const { return (0 != (mLockFlag.load(std::memory_order_acquire) & READ_LOCK_COUNT_MASK)); }
	
	// 쓰기 락 상태 확인 (디버그용)
	bool IsWriteLocked() const { return (0 != (mLockFlag.load(std::memory_order_acquire) & WRITE_LOCK_FLAG)); }
};

#include "ServerEngine/Lock/ReadLock.h"
#include "ServerEngine/Lock/WriteLock.h"
