// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Lock.h"

import ServerEngineCore;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Lock
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Lock::Lock()
{
}

Lock::~Lock()
{
}

void Lock::EnterWriteLock()
{
	uint32_t spinCnt = 0;

	constexpr uint32_t writeLockMask = (WRITE_LOCK_FLAG | WRITE_LOCK_THREAD_ID_MASK);
	const uint32_t threadWriteLockFlag = (WRITE_LOCK_FLAG | (tThreadId << 16));

	while (true)
	{
		// 읽기와 쓰기 락 모두 없으면 허용
		uint32_t expected = 0;
		const uint32_t desired = threadWriteLockFlag;

		// memory_order:
		// - 성공(acquire): 쓰기 락 획득(임계구역 진입) 시, 이후 임계구역에서 보호된 데이터 관측 보장.
		// - 실패(relaxed): 실패 시 루프/재시도만 하므로 추가 동기화 불필요.
		const bool result = mLockFlag.compare_exchange_weak(expected, desired, std::memory_order_acquire, std::memory_order_relaxed);
		
		if (true == result)
		{
			break;
		}

		// CAS 실패 시 expected에는 관측된 현재 값이 들어온다(추가 atomic load 1회 절약).
		const uint32_t lockFlag = expected;

		// 동일 Thread 에서 쓰기 락을 걸었었다면 허용
		if ((lockFlag & writeLockMask) == threadWriteLockFlag)
			break;

		if (MAX_SPIN_CNT < spinCnt)
		{
			_YIELD_SPIN;
		}
		else
		{
			++spinCnt;
		}
	}

	// memory_order(relaxed): 재진입 카운트는 소유 스레드에서만 의미가 있고,
	// 임계구역 동기화의 핵심은 mLockFlag(acquire/release)로 충분.
	mWriteLockCount.fetch_add(1, std::memory_order_relaxed);

	// 이 Thread 가 점유한 Lock 갯수를 증가시킨다
	++tLockedCount;
}

void Lock::LeaveWriteLock()
{
	// memory_order(acquire): 해제 전 검증(현재 플래그/카운트)을 최신으로 관측.
	_ASSERT_CRASH(0 == (mLockFlag.load(std::memory_order_acquire) & READ_LOCK_COUNT_MASK));
	_ASSERT_CRASH(0 < mWriteLockCount.load(std::memory_order_acquire));

	// memory_order(relaxed): 마지막 재진입 해제 판단용 카운터.
	// 실제 임계구역 publish는 아래 mLockFlag.store(..., release)가 담당.
	if (0 == (mWriteLockCount.fetch_sub(1, std::memory_order_relaxed) - 1))
	{
		// memory_order(release): 임계구역 내 쓰기들이 락 해제 이후로 재배치되지 않도록 보장.
		mLockFlag.store(0, std::memory_order_release);
	}

	// 이 Thread 가 점유한 Lock 갯수를 감소시킨다
	--tLockedCount;
}

void Lock::EnterReadLock()
{
	uint32_t spinCnt = 0;
	constexpr uint32_t writeLockMask = (WRITE_LOCK_FLAG | WRITE_LOCK_THREAD_ID_MASK);
	const uint32_t threadWriteLockFlag = (WRITE_LOCK_FLAG | (tThreadId << 16));

	do
	{
		// memory_order(relaxed): 스핀 중 상태 관측은 원자성만 필요.
		// 실제 임계구역 진입(보호 데이터 관측)은 아래 CAS 성공(acquire)에서 보장.
		const uint32_t lockFlag = mLockFlag.load(std::memory_order_relaxed);

		if ((lockFlag & WRITE_LOCK_FLAG) == 0)
		{
			// 쓰기 락이 없으면 읽기 락 시도.

			uint32_t expected = (lockFlag & READ_LOCK_COUNT_MASK);
			// memory_order:
			// - 성공(acquire): 읽기 락 획득(임계구역 진입) 시, 이후 보호된 데이터 관측 보장.
			// - 실패(relaxed): 재시도만 하므로 추가 동기화 불필요.
			const bool result = mLockFlag.compare_exchange_weak(expected, (expected + 1), std::memory_order_acquire, std::memory_order_relaxed);

			if (true == result)
			{
				break;
			}
		}
		else if ((lockFlag & writeLockMask) == threadWriteLockFlag)
		{
			// 동일 Thread 에서 쓰기 락을 걸었었다면 허용.
			// memory_order(relaxed): write-lock 보유 중 read-lock 카운트 증가(재진입용).
			// 이미 write-lock이 임계구역을 보장하므로 추가 동기화 불필요.
			mLockFlag.fetch_add(1, std::memory_order_relaxed);

			break;
		}

		if (MAX_SPIN_CNT < spinCnt)
		{
			_YIELD_SPIN;
		}
		else
		{
			++spinCnt;
		}

	} while (true);

	// 이 Thread 가 점유한 Lock 갯수를 증가시킨다
	++tLockedCount;
}

void Lock::LeaveReadLock()
{
	// memory_order(acquire): 현재 리드카운트가 유효한지 최신 관측.
	_ASSERT_CRASH(0 < (mLockFlag.load(std::memory_order_acquire) & READ_LOCK_COUNT_MASK));

	// memory_order(release): 읽기 임계구역 종료를 publish.
	// (읽기 임계구역 내에서의 작업이 unlock 이후로 재배치되지 않도록 보수적으로 release)
	mLockFlag.fetch_sub(1, std::memory_order_release);

	// 이 Thread 가 점유한 Lock 갯수를 감소시킨다
	--tLockedCount;
}
