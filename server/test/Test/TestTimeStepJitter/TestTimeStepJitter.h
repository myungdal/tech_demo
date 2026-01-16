// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// TestTimeStepJitter.h - TimeStep Jitter 시각화 테스트
#pragma once

#include "Test/App/TestAppBase.h"
#include "TimeStepJitter.h"
#include <random>
#include <thread>
#include <atomic>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TestTimeStepJitter: Windows GDI 기반 지터 시뮬레이션 시각화
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TestTimeStepJitter final : public TestAppBase
{
private:
	static constexpr int64_t TIME_STEP_INTERVAL_MS = 32;	// 32ms 간격 (약 31.25 FPS)
	static constexpr int CANVAS_SIZE = 512;					// 캔버스 크기

	// 시뮬레이션 상태
	TimeStepJitterGenerator mGenerator;
	TimeStepJitterPlayer mPlayer;
	std::mt19937 mRandomEngine;

	int64_t mTotalStepCount0 = 0;	// Generator에서 생성된 총 스텝
	int64_t mTotalStepCount1 = 0;	// Player에서 실행된 총 스텝
	int64_t mJitterBuffer = 0;		// 지터 시뮬레이션용 버퍼

	// 윈도우 핸들
	HWND mHwnd = nullptr;
	std::atomic<bool> mRunning{ false };
	std::thread mWindowThread;
	std::thread mJitterThread;

public:
	explicit TestTimeStepJitter(AppArg appArg);
	~TestTimeStepJitter() override;

public:
	void OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher) override;
	void OnStarted() override;
	void OnUpdate() override;

private:
	// 윈도우 프로시저
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	// 렌더링
	void Render(HDC hdc);
	void DrawCircle(HDC hdc, int x, int y, int radius);
	void DrawText(HDC hdc, int x, int y, const std::wstring& text);

	// 스레드 함수
	void WindowThreadFunc();
	void JitterThreadFunc();
	void SimulationStep();
};

inline std::shared_ptr<TestTimeStepJitter> gTestTimeStepJitter = nullptr;
