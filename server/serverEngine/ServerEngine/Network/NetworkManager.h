// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// NetworkManager.h - 네트워크 관리자
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NetworkManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 네트워크 관리자 (시간 동기화 등)
class NetworkManager
{
private:
	// Global/Local 시간 차이 (시간 동기화용)
	std::atomic<ClockMs> mGlobalLocalTimeDelta = INVALID_CLOCK_MS;

public:
	NetworkManager();
	virtual ~NetworkManager();

	// Global 시간 설정 여부 확인
	bool HasGlobalNow() const;
	
	// Global 시간 조회
	ClockPoint GetGlobalNow() const;

	// Global 시간 설정 (동기화)
	void SetupGlobalNow(ClockMs globalNow);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 전역 싱글톤
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::shared_ptr<NetworkManager> gNetworkManager;
