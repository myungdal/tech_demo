// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// WriteLock.h - RAII 스타일 Write Lock Guard
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Write Lock Guard
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Lock;

// RAII Write Lock Guard (생성자에서 획득, 소멸자에서 해제)
class WriteLock
{
private:
	// 보호 대상 Lock 객체 포인터
	Lock* mLock = nullptr;

public:
	WriteLock() = default;
	
	// Lock을 획득한다.
	explicit WriteLock(Lock& lock);
	
	// 소멸 시 자동으로 락을 해제한다.
	~WriteLock();

	// 복사 금지
	WriteLock(const WriteLock&) = delete;
	WriteLock& operator=(const WriteLock&) = delete;

	// 이동 허용
	WriteLock(WriteLock&& other) noexcept;
	WriteLock& operator=(WriteLock&& other) noexcept;
};
