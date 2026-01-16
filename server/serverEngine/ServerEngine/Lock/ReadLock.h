// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ReadLock.h - RAII 스타일 Read Lock Guard
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read Lock Guard
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Lock;

// RAII Read Lock Guard (생성자에서 획득, 소멸자에서 해제)
class ReadLock
{
private:
	// 보호 대상 Lock 객체 포인터
	Lock* mLock = nullptr;

public:
	ReadLock() = default;
	
	// Lock을 획득한다.
	explicit ReadLock(Lock& lock);
	
	// 소멸 시 자동으로 락을 해제한다.
	~ReadLock();

	// 복사 금지
	ReadLock(const ReadLock&) = delete;
	ReadLock& operator=(const ReadLock&) = delete;

	// 이동 허용
	ReadLock(ReadLock&& other) noexcept;
	ReadLock& operator=(ReadLock&& other) noexcept;
};
