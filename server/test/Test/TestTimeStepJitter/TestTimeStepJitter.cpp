// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// TestTimeStepJitter.cpp - TimeStep Jitter 시각화 테스트 구현
import Test;

#include "TestTimeStepJitter.h"
#include <format>

// 전역 인스턴스 포인터 (윈도우 프로시저에서 사용)
static TestTimeStepJitter* gInstancePtr = nullptr;

TestTimeStepJitter::TestTimeStepJitter(AppArg appArg)
	: TestAppBase(appArg)
	, mGenerator(TIME_STEP_INTERVAL_MS)
	, mPlayer(TIME_STEP_INTERVAL_MS)
	, mRandomEngine(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()))
{
	gInstancePtr = this;
}

TestTimeStepJitter::~TestTimeStepJitter()
{
	mRunning = false;

	if (mJitterThread.joinable())
		mJitterThread.join();

	if (mHwnd)
		PostMessage(mHwnd, WM_CLOSE, 0, 0);

	if (mWindowThread.joinable())
		mWindowThread.join();

	gInstancePtr = nullptr;
}

void TestTimeStepJitter::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
	dispatcher.Register(
		L"start", L"시뮬레이션 시작",
		[this](MAYBE_UNUSED ArgList& args) -> std::wstring
		{
			if (!mRunning)
			{
				mRunning = true;
				mWindowThread = std::thread(&TestTimeStepJitter::WindowThreadFunc, this);
				mJitterThread = std::thread(&TestTimeStepJitter::JitterThreadFunc, this);
				return L"시뮬레이션 시작됨\n";
			}
			return L"이미 실행 중\n";
		}
	);

	dispatcher.Register(
		L"stop", L"시뮬레이션 중지",
		[this](MAYBE_UNUSED ArgList& args) -> std::wstring
		{
			if (mRunning)
			{
				mRunning = false;
				return L"시뮬레이션 중지됨\n";
			}
			return L"실행 중이 아님\n";
		}
	);

	dispatcher.Register(
		L"status", L"현재 상태 출력",
		[this](MAYBE_UNUSED ArgList& args) -> std::wstring
		{
			return std::format(
				L"=== TimeStep Jitter 상태 ===\n"
				L"  생성된 스텝: {}\n"
				L"  실행된 스텝: {}\n"
				L"  지터 버퍼: {}\n"
				L"  큐 스텝: {}\n"
				L"  속도: {:.3f}\n"
				L"  목표 큐 시간: {:.1f}ms\n",
				mTotalStepCount0,
				mTotalStepCount1,
				mJitterBuffer,
				mPlayer.GetQueuedStepCount(),
				mPlayer.GetSpeed(),
				mPlayer.GetTargetQueuedTime()
			);
		}
	);
}

void TestTimeStepJitter::OnStarted()
{
	std::wcout << L"=== TimeStep Jitter 시각화 테스트 ===" << std::endl;
	std::wcout << L"명령어: stop, status, help" << std::endl;
	std::wcout << L"시뮬레이션 자동 시작..." << std::endl;

	// 자동 시작
	mRunning = true;
	mWindowThread = std::thread(&TestTimeStepJitter::WindowThreadFunc, this);
	mJitterThread = std::thread(&TestTimeStepJitter::JitterThreadFunc, this);

	std::wcout << L"시각화 윈도우가 열렸습니다." << std::endl;
}

void TestTimeStepJitter::OnUpdate()
{
	// 콘솔 업데이트 (필요시)
}

LRESULT CALLBACK TestTimeStepJitter::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			// 더블 버퍼링
			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP memBitmap = CreateCompatibleBitmap(hdc, CANVAS_SIZE, CANVAS_SIZE);
			HBITMAP oldBitmap = static_cast<HBITMAP>(SelectObject(memDC, memBitmap));

			// 배경 지우기
			RECT rect = { 0, 0, CANVAS_SIZE, CANVAS_SIZE };
			HBRUSH bgBrush = CreateSolidBrush(RGB(20, 20, 30));
			FillRect(memDC, &rect, bgBrush);
			DeleteObject(bgBrush);

			// 렌더링
			if (gInstancePtr)
				gInstancePtr->Render(memDC);

			// 화면에 복사
			BitBlt(hdc, 0, 0, CANVAS_SIZE, CANVAS_SIZE, memDC, 0, 0, SRCCOPY);

			// 정리
			SelectObject(memDC, oldBitmap);
			DeleteObject(memBitmap);
			DeleteDC(memDC);

			EndPaint(hwnd, &ps);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_TIMER:
		InvalidateRect(hwnd, nullptr, FALSE);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void TestTimeStepJitter::Render(HDC hdc)
{
	// 시뮬레이션 스텝 실행
	SimulationStep();

	// 텍스트 색상 설정
	SetTextColor(hdc, RGB(200, 200, 200));
	SetBkMode(hdc, TRANSPARENT);

	// 상태 정보 표시
	DrawText(hdc, 10, 10, std::format(L"jitterBuffer: {}", mJitterBuffer));
	DrawText(hdc, 10, 30, std::format(L"queuedStepCount: {}", mPlayer.GetQueuedStepCount()));
	DrawText(hdc, 10, 50, std::format(L"speed: {:.4f}", mPlayer.GetSpeed()));
	DrawText(hdc, 10, 70, std::format(L"dest: {:.1f}", mPlayer.GetTargetQueuedTime()));
	DrawText(hdc, 10, 90, std::format(L"gen: {} / play: {}", mTotalStepCount0, mTotalStepCount1));

	// 원형 궤도 중심
	constexpr int centerX = 200;
	constexpr int centerY = 200;
	constexpr int radius = 100;

	// 궤도 그리기 (회색 원)
	HPEN orbitPen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
	HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, orbitPen));
	HBRUSH nullBrush = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
	HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(hdc, nullBrush));
	Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
	SelectObject(hdc, oldPen);
	DeleteObject(orbitPen);

	// 점 0: Generator 생성 위치 (흰색, 작은 원)
	double r0 = (mTotalStepCount0 % 300) / 300.0 * 2.0 * 3.14159265358979;
	int x0 = centerX + static_cast<int>(std::cos(r0) * radius);
	int y0 = centerY + static_cast<int>(std::sin(r0) * radius);

	HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	SelectObject(hdc, whitePen);
	DrawCircle(hdc, x0, y0, 3);
	DeleteObject(whitePen);

	// 점 1: Player 실행 위치 (녹색, 큰 원)
	double r1 = (mTotalStepCount1 % 300) / 300.0 * 2.0 * 3.14159265358979;
	int x1 = centerX + static_cast<int>(std::cos(r1) * radius);
	int y1 = centerY + static_cast<int>(std::sin(r1) * radius);

	HPEN greenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 100));
	SelectObject(hdc, greenPen);
	DrawCircle(hdc, x1, y1, 5);
	DeleteObject(greenPen);

	// 점 2: 지터 보정된 위치 (노란색, 작은 원)
	double r2 = ((mTotalStepCount0 - mJitterBuffer) % 300) / 300.0 * 2.0 * 3.14159265358979;
	int x2 = centerX + static_cast<int>(std::cos(r2) * radius);
	int y2 = centerY + static_cast<int>(std::sin(r2) * radius);

	HPEN yellowPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	SelectObject(hdc, yellowPen);
	DrawCircle(hdc, x2, y2, 2);
	DeleteObject(yellowPen);

	// 축 그리기
	constexpr int graphY = 400;

	HPEN axisPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	SelectObject(hdc, axisPen);

	// X축
	MoveToEx(hdc, 100, graphY, nullptr);
	LineTo(hdc, 300, graphY);

	// Y축
	MoveToEx(hdc, 200, graphY - 60, nullptr);
	LineTo(hdc, 200, graphY + 60);

	DeleteObject(axisPen);

	// DoubleSigmoid 그래프 그리기
	HPEN graphPen = CreatePen(PS_SOLID, 2, RGB(255, 100, 100));
	SelectObject(hdc, graphPen);

	bool first = true;
	for (int px = -100; px <= 100; ++px)
	{
		double t = px / 100.0;
		double y = TimeStepJitterTuner::DoubleSigmoid(t);
		int graphX = 200 + px;
		int graphPY = graphY - static_cast<int>(y * 50);

		if (first)
		{
			MoveToEx(hdc, graphX, graphPY, nullptr);
			first = false;
		}
		else
		{
			LineTo(hdc, graphX, graphPY);
		}
	}

	DeleteObject(graphPen);
	SelectObject(hdc, oldBrush);
}

void TestTimeStepJitter::DrawCircle(HDC hdc, int x, int y, int r)
{
	Ellipse(hdc, x - r, y - r, x + r, y + r);
}

void TestTimeStepJitter::DrawText(HDC hdc, int x, int y, const std::wstring& text)
{
	TextOutW(hdc, x, y, text.c_str(), static_cast<int>(text.length()));
}

void TestTimeStepJitter::WindowThreadFunc()
{
	// 윈도우 클래스 등록
	WNDCLASSEXW wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = L"TimeStepJitterWindow";

	RegisterClassExW(&wc);

	// 윈도우 생성
	RECT rect = { 0, 0, CANVAS_SIZE, CANVAS_SIZE };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	mHwnd = CreateWindowExW(
		0,
		L"TimeStepJitterWindow",
		L"TimeStep Jitter Visualization",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr,
		GetModuleHandle(nullptr),
		nullptr
	);

	if (!mHwnd)
		return;

	ShowWindow(mHwnd, SW_SHOW);
	UpdateWindow(mHwnd);

	// 타이머 설정 (약 60 FPS)
	SetTimer(mHwnd, 1, 16, nullptr);

	// 메시지 루프
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (!mRunning)
			break;
	}

	KillTimer(mHwnd, 1);
	DestroyWindow(mHwnd);
	mHwnd = nullptr;
}

void TestTimeStepJitter::JitterThreadFunc()
{
	// 네트워크 지터 시뮬레이션
	// 랜덤한 간격으로 스텝을 Player에 Push
	while (mRunning)
	{
		// 지터 버퍼에서 랜덤하게 일부만 적용
		int64_t maxApply = std::max(mJitterBuffer, static_cast<int64_t>(0));
		std::uniform_int_distribution<int64_t> dist(0, maxApply);
		int64_t applyStepCount = dist(mRandomEngine);

		mJitterBuffer -= applyStepCount;
		mPlayer.PushStep(applyStepCount);

		// 랜덤 지연 (50~150ms, 가끔 400ms 또는 800ms 추가)
		std::uniform_real_distribution<double> randDist(0.0, 1.0);
		int delay = 50 + static_cast<int>(randDist(mRandomEngine) * 100);

		if (randDist(mRandomEngine) * 30 < 1)
			delay += 400;

		if (randDist(mRandomEngine) * 70 < 1)
			delay += 800;

		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}
}

void TestTimeStepJitter::SimulationStep()
{
	// Generator에서 스텝 생성
	int64_t stepCount0 = mGenerator.Generate();
	mTotalStepCount0 += stepCount0;
	mJitterBuffer += stepCount0;

	// Player에서 스텝 실행
	int64_t stepCount1 = mPlayer.PlayStep();
	mTotalStepCount1 += stepCount1;
}
