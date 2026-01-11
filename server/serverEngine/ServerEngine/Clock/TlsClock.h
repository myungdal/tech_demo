// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// TlsClock.h - Thread Local 시간 캐싱 클래스
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread Local Clock
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 스레드별 시간 캐싱 클래스 (고속 시간 조회)
// [WHY] 매번 시스템 콜 대신 스레드 루프에서 주기적으로 갱신된 시간 사용
// [주의] 시스템 시간 변경 시 시간이 역전될 수 있음
class TlsClock
{
private:
	// Local 시간 포인터 (TlsClockInternalMembers 참조)
	time_t* mLocalNowTt = nullptr;
	tm* mLocalNowTm = nullptr;
	TIMESTAMP_STRUCT* mLocalNowTs = nullptr;

	// Global 시간 포인터 (TlsClockInternalMembers 참조)
	time_t* mGlobalNowTt = nullptr;
	tm* mGlobalNowTm = nullptr;
	TIMESTAMP_STRUCT* mGlobalNowTs = nullptr;

public:
	TlsClock();
	
	// 시간 갱신 (스레드 루프에서 주기적으로 호출)
	void Update();

public:
	// Local 시간 조회 (프로세스 시작 후 경과 시간 기준)
	const ClockPoint& GetLocalNow() const;
	const ClockMs& GetLocalNowMs() const;
	const time_t& GetLocalNowTt();
	const tm& GetLocalNowTm();
	const TIMESTAMP_STRUCT& GetLocalNowTs();

public:
	// Global 시간 조회 (실제 시스템 시간 기준)
	const ClockPoint& GetGlobalNow() const;
	const ClockMs& GetGlobalNowMs() const;
	const time_t& GetGlobalNowTt();
	const tm& GetGlobalNowTm();
	const TIMESTAMP_STRUCT& GetGlobalNowTs();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal Members
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TlsClock 내부 데이터 저장소 (TLS 변수로 사용)
struct TlsClockInternalMembers
{
	friend class TlsClock;

private:
	// Local 시간 (프로세스 시작 후 경과 시간)
	ClockPoint mLocalNow = CLOCK_NOW;
	ClockMs mLocalNowMs = 0ms;
	time_t mLocalNowTt = 0;
	std::tm mLocalNowTm{};
	TIMESTAMP_STRUCT mLocalNowTs{};

	// Global 시간 (실제 시스템 시간)
	ClockPoint mGlobalNow = CLOCK_NOW;
	ClockMs mGlobalNowMs = 0ms;
	time_t mGlobalNowTt = 0;
	std::tm mGlobalNowTm{};
	TIMESTAMP_STRUCT mGlobalNowTs{};
};
