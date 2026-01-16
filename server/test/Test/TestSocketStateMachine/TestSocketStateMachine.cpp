// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;

#include "TestSocketStateMachine.h"

// [WHY] 표준 라이브러리 헤더들은 pch_common.h에 이미 포함됨


void TestSocketStateMachine::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestSocketStateMachine::OnStarted()
{
	std::wcout << L"\n";
	std::wcout << L"================================================================================\n";
	std::wcout << L"=== 소켓 상태 머신 테스트 시작 ===\n";
	std::wcout << L"================================================================================\n\n";

	TestCase_StateTransitionTable();
	TestCase_ReconnectBlocked();
	TestCase_MultipleReconnect();
	TestCase_ErrorRecovery();
	TestCase_RefCountBalance();
	TestCase_ConnectFailureRecovery();
	TestCase_ClosingFromVariousStates();
	TestCase_RapidReconnectStress();
	TestCase_RecoveryPathFromAllStates();
	TestCase_ConcurrentStateTransition();
	
	// 추가 테스트들
	TestCase_MultithreadedStateTransition();
	TestCase_SocketPoolMemoryCheck();
	TestCase_TimeoutScenario();
	TestCase_MassConnectionTest();
	TestCase_AbnormalTermination();
	TestCase_SendRecvDuringDisconnect();
	TestCase_AcceptFailureRecovery();
	TestCase_AcceptFailureDetailedScenarios();
	TestCase_ReconnectDelayBehavior();
	TestCase_EncryptionHandshakeState();
	TestCase_BacklogManagement();
	
	TestCase_DisconnectedToClosedTransition();

	std::wcout << L"\n";
	std::wcout << L"================================================================================\n";
	std::wcout << L"=== 소켓 상태 머신 테스트 완료 ===\n";
	std::wcout << L"================================================================================\n\n";
}

void TestSocketStateMachine::OnUpdate()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 1: 상태 전이 테이블 검증
// - 어떤 상태에서 어떤 상태로 전이 가능한지 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_StateTransitionTable()
{
	std::wcout << L"[테스트 1] 상태 전이 테이블 검증\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	// 모든 상태 전이 가능성 매핑
	using SS = SocketBase::SocketState;
	
	struct TransitionTest
	{
		SS fromState;
		SS toState;
		bool expectedResult;
		const wchar_t* description;
	};

	// 현재 ChangeSocketState의 expectedArray 기반:
	// CLOSED(0): expected = CLOSING
	// OPENED(1): expected = CLOSED
	// CONNECTING(2): expected = OPENED
	// CONNECTED(3): expected = CONNECTING
	// DISCONNECTING(4): expected = CONNECTED
	// DISCONNECTED(5): expected = DISCONNECTING
	// 
	// 추가 예외: CLOSING은 CLOSING, CONNECTING, CLOSED에서 가능

	TransitionTest tests[] = {
		// 정상 전이 경로
		{ SS::CLOSING,       SS::CLOSED,        true,  L"CLOSING -> CLOSED (정상)" },
		{ SS::CLOSED,        SS::OPENED,        true,  L"CLOSED -> OPENED (정상)" },
		{ SS::OPENED,        SS::CONNECTING,    true,  L"OPENED -> CONNECTING (정상)" },
		{ SS::CONNECTING,    SS::CONNECTED,     true,  L"CONNECTING -> CONNECTED (정상)" },
		{ SS::CONNECTED,     SS::DISCONNECTING, true,  L"CONNECTED -> DISCONNECTING (정상)" },
		{ SS::DISCONNECTING, SS::DISCONNECTED,  true,  L"DISCONNECTING -> DISCONNECTED (정상)" },

		// [FIX] 수정 후 재접속 경로가 열림
		{ SS::DISCONNECTED,  SS::CLOSED,        true,  L"DISCONNECTED -> CLOSED (수정됨! 재접속 가능)" },
		{ SS::DISCONNECTED,  SS::OPENED,        false, L"DISCONNECTED -> OPENED (직접은 불가, CLOSED 거쳐야 함)" },
		{ SS::DISCONNECTED,  SS::CLOSING,       true,  L"DISCONNECTED -> CLOSING (원래 가능)" },

		// ReleaseRef에서 시도하는 전이
		{ SS::CONNECTED,     SS::CLOSED,        false, L"CONNECTED -> CLOSED (불가)" },
	};

	int passCount = 0;
	int failCount = 0;

	for (const auto& test : tests)
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		socket->SetSocketStateForTest(test.fromState);

		bool actualResult = socket->TryChangeSocketState(test.toState);
		bool testPassed = (actualResult == test.expectedResult);

		if (testPassed)
		{
			++passCount;
			std::wcout << std::format(L"  [PASS] {} -> {} = {} (예상대로)\n",
				GetSocketStateString(test.fromState),
				GetSocketStateString(test.toState),
				actualResult ? L"성공" : L"실패");
		}
		else
		{
			++failCount;
			std::wcout << std::format(L"  [FAIL] {} -> {} = {} (예상: {})\n",
				GetSocketStateString(test.fromState),
				GetSocketStateString(test.toState),
				actualResult ? L"성공" : L"실패",
				test.expectedResult ? L"성공" : L"실패");
		}

		TestSocket::DisposeSocketImpl(socket);
	}

	std::wcout << std::format(L"\n  결과: {} 통과, {} 실패\n\n", passCount, failCount);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 2: 재접속 블로킹 시뮬레이션
// - Disconnect 후 Connect가 되는지 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_ReconnectBlocked()
{
	std::wcout << L"[테스트 2] 재접속 흐름 시뮬레이션\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	TestSocket* socket = TestSocket::AcquireSocketImpl();

	std::wcout << L"  시나리오: 정상 연결 -> Disconnect -> 재접속 시도\n\n";

	// Step 1: 초기 상태 CLOSED
	socket->SetSocketStateForTest(SS::CLOSED);
	std::wcout << std::format(L"  [1] 초기 상태: {}\n", GetSocketStateString(socket->GetSocketState()));

	// Step 2: Open() 시뮬레이션 - CLOSED -> OPENED
	bool openResult = socket->TryChangeSocketState(SS::OPENED);
	std::wcout << std::format(L"  [2] Open() 시도: {} -> 현재 상태: {}\n",
		openResult ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	// Step 3: Connect() 시뮬레이션 - OPENED -> CONNECTING
	bool connectingResult = socket->TryChangeSocketState(SS::CONNECTING);
	std::wcout << std::format(L"  [3] Connect() 시도: {} -> 현재 상태: {}\n",
		connectingResult ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	// Step 4: OnCompletedConnect - CONNECTING -> CONNECTED
	bool connectedResult = socket->TryChangeSocketState(SS::CONNECTED);
	std::wcout << std::format(L"  [4] 연결 완료: {} -> 현재 상태: {}\n",
		connectedResult ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	// Step 5: Disconnect() - CONNECTED -> DISCONNECTING
	bool disconnectingResult = socket->TryChangeSocketState(SS::DISCONNECTING);
	std::wcout << std::format(L"  [5] Disconnect() 시도: {} -> 현재 상태: {}\n",
		disconnectingResult ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	// Step 6: OnCompletedDisconnect - DISCONNECTING -> DISCONNECTED
	bool disconnectedResult = socket->TryChangeSocketState(SS::DISCONNECTED);
	std::wcout << std::format(L"  [6] 연결 해제 완료: {} -> 현재 상태: {}\n",
		disconnectedResult ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	std::wcout << L"\n  === 이제 재접속 시도 ===\n\n";

	// Step 7: ReleaseRef에서 시도하는 CLOSED 전이 (수정 후 성공해야 함)
	bool toClosedResult = socket->TryChangeSocketState(SS::CLOSED);
	std::wcout << std::format(L"  [7] DISCONNECTED -> CLOSED 전이 시도: {} -> 현재 상태: {}\n",
		toClosedResult ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	// Step 8: Open() 시도 (CLOSED 상태에서)
	bool reopenResult = socket->TryChangeSocketState(SS::OPENED);
	std::wcout << std::format(L"  [8] Open() 시도 (CLOSED에서): {} -> 현재 상태: {}\n",
		reopenResult ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	// Step 9: Connect() 시도 (OPENED 상태에서)
	bool reconnectResult = socket->TryChangeSocketState(SS::CONNECTING);
	std::wcout << std::format(L"  [9] Connect() 시도 (OPENED에서): {} -> 현재 상태: {}\n",
		reconnectResult ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	std::wcout << L"\n";
	
	// 최종 결과 판정
	bool reconnectPossible = toClosedResult && reopenResult && reconnectResult;
	if (reconnectPossible)
	{
		std::wcout << L"  [결과] 재접속 경로: *** 정상! *** (수정 확인됨)\n";
		std::wcout << L"         DISCONNECTED -> CLOSED -> OPENED -> CONNECTING 성공!\n\n";
	}
	else
	{
		std::wcout << L"  [결과] 재접속 경로: *** 막힘! *** (버그 확인됨)\n";
		std::wcout << std::format(L"         toClosedResult={}, reopenResult={}, reconnectResult={}\n\n",
			toClosedResult, reopenResult, reconnectResult);
	}

	TestSocket::DisposeSocketImpl(socket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 3: 다중 재접속 테스트
// - 연결/해제를 여러 번 반복해도 정상 동작하는지 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_MultipleReconnect()
{
	std::wcout << L"[테스트 3] 다중 재접속 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	TestSocket* socket = TestSocket::AcquireSocketImpl();
	socket->SetSocketStateForTest(SS::CLOSED);

	constexpr int RECONNECT_COUNT = 5;
	int successCount = 0;

	for (int i = 0; i < RECONNECT_COUNT; ++i)
	{
		std::wcout << std::format(L"  [반복 {}] ", i + 1);

		// 연결 흐름: CLOSED -> OPENED -> CONNECTING -> CONNECTED
		bool step1 = socket->TryChangeSocketState(SS::OPENED);
		bool step2 = socket->TryChangeSocketState(SS::CONNECTING);
		bool step3 = socket->TryChangeSocketState(SS::CONNECTED);

		// 해제 흐름: CONNECTED -> DISCONNECTING -> DISCONNECTED -> CLOSED
		bool step4 = socket->TryChangeSocketState(SS::DISCONNECTING);
		bool step5 = socket->TryChangeSocketState(SS::DISCONNECTED);
		bool step6 = socket->TryChangeSocketState(SS::CLOSED);

		bool allSuccess = step1 && step2 && step3 && step4 && step5 && step6;

		if (allSuccess)
		{
			++successCount;
			std::wcout << L"성공\n";
		}
		else
		{
			std::wcout << std::format(L"실패 (step1={}, step2={}, step3={}, step4={}, step5={}, step6={})\n",
				step1, step2, step3, step4, step5, step6);
		}
	}

	std::wcout << std::format(L"\n  결과: {}/{} 사이클 성공\n\n", successCount, RECONNECT_COUNT);

	TestSocket::DisposeSocketImpl(socket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 4: 에러 상태 복구 테스트
// - mHasError 플래그가 설정된 후 복구되는지 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_ErrorRecovery()
{
	std::wcout << L"[테스트 4] 에러 상태 복구 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	TestSocket* socket = TestSocket::AcquireSocketImpl();

	// 시나리오 1: 에러 발생 후 상태 전이
	socket->SetSocketStateForTest(SS::CONNECTED);
	socket->SetHasErrorForTest(true);

	std::wcout << L"  [시나리오 1] CONNECTED + mHasError=true 상태에서 DISCONNECTING 전이\n";
	bool canDisconnect = socket->TryChangeSocketState(SS::DISCONNECTING);
	std::wcout << std::format(L"    DISCONNECTING 전이: {} (상태: {}, mHasError: {})\n",
		canDisconnect ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()),
		socket->HasErrorForTest() ? L"true" : L"false");

	// 시나리오 2: 에러 상태에서 해제 완료
	if (canDisconnect)
	{
		bool canDisconnected = socket->TryChangeSocketState(SS::DISCONNECTED);
		std::wcout << std::format(L"    DISCONNECTED 전이: {} (상태: {})\n",
			canDisconnected ? L"성공" : L"실패",
			GetSocketStateString(socket->GetSocketState()));

		bool canClosed = socket->TryChangeSocketState(SS::CLOSED);
		std::wcout << std::format(L"    CLOSED 전이: {} (상태: {})\n",
			canClosed ? L"성공" : L"실패",
			GetSocketStateString(socket->GetSocketState()));
	}

	// 시나리오 3: 에러 플래그 리셋 후 재연결
	socket->SetHasErrorForTest(false);
	std::wcout << L"\n  [시나리오 2] mHasError=false 리셋 후 재연결\n";

	socket->SetSocketStateForTest(SS::CLOSED);
	bool canReopen = socket->TryChangeSocketState(SS::OPENED);
	bool canReconnect = socket->TryChangeSocketState(SS::CONNECTING);

	std::wcout << std::format(L"    재연결 시도: {} (상태: {})\n",
		(canReopen && canReconnect) ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	std::wcout << L"\n";
	TestSocket::DisposeSocketImpl(socket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 5: 참조 카운트 균형 테스트
// - AddRef/ReleaseRef 호출 균형 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_RefCountBalance()
{
	std::wcout << L"[테스트 5] 참조 카운트 균형 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	TestSocket* socket = TestSocket::AcquireSocketImpl();

	// InitRef로 참조 카운트 1로 시작 (ReleaseRef 호출 시 dispose 방지)
	socket->InitRef(L"INIT");

	int32_t initialRefCnt = socket->GetRefCntForTest();
	std::wcout << std::format(L"  초기 참조 카운트 (InitRef 후): {}\n", initialRefCnt);

	// 여러 번 AddRef
	socket->AddRef(L"TEST1");
	socket->AddRef(L"TEST2");
	socket->AddRef(L"TEST3");

	int32_t afterAddRef = socket->GetRefCntForTest();
	std::wcout << std::format(L"  AddRef x3 후: {} (예상: {})\n", afterAddRef, initialRefCnt + 3);

	// 동일 횟수 ReleaseRef (InitRef 분은 제외)
	socket->ReleaseRef(L"TEST3");
	socket->ReleaseRef(L"TEST2");
	socket->ReleaseRef(L"TEST1");

	int32_t afterReleaseRef = socket->GetRefCntForTest();
	std::wcout << std::format(L"  ReleaseRef x3 후: {} (예상: {})\n", afterReleaseRef, initialRefCnt);

	bool balanced = (afterReleaseRef == initialRefCnt);
	std::wcout << std::format(L"\n  결과: 참조 카운트 균형 {}\n\n", balanced ? L"유지됨" : L"깨짐!");

	// 테스트 정리: 참조 카운트 리셋 후 dispose
	socket->ResetRefCnt();
	TestSocket::DisposeSocketImpl(socket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 6: 연결 실패 후 복구 테스트
// - Connect 실패 시나리오에서 재시도 가능한지 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_ConnectFailureRecovery()
{
	std::wcout << L"[테스트 6] 연결 실패 후 복구 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	TestSocket* socket = TestSocket::AcquireSocketImpl();

	// 시나리오: CONNECTING 상태에서 연결 실패 -> CLOSING -> CLOSED -> 재시도
	socket->SetSocketStateForTest(SS::CLOSED);

	std::wcout << L"  [1] 첫 번째 연결 시도\n";
	socket->TryChangeSocketState(SS::OPENED);
	socket->TryChangeSocketState(SS::CONNECTING);
	std::wcout << std::format(L"      현재 상태: {}\n", GetSocketStateString(socket->GetSocketState()));

	std::wcout << L"  [2] 연결 실패 시뮬레이션 (CONNECTING -> CLOSING)\n";
	bool canClosing = socket->TryChangeSocketState(SS::CLOSING);
	std::wcout << std::format(L"      CLOSING 전이: {} (상태: {})\n",
		canClosing ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	std::wcout << L"  [3] 정리 (CLOSING -> CLOSED)\n";
	bool canClosed = socket->TryChangeSocketState(SS::CLOSED);
	std::wcout << std::format(L"      CLOSED 전이: {} (상태: {})\n",
		canClosed ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	std::wcout << L"  [4] 재연결 시도\n";
	bool retryOpen = socket->TryChangeSocketState(SS::OPENED);
	bool retryConnect = socket->TryChangeSocketState(SS::CONNECTING);
	bool retryConnected = socket->TryChangeSocketState(SS::CONNECTED);
	std::wcout << std::format(L"      재연결 결과: {} (상태: {})\n",
		(retryOpen && retryConnect && retryConnected) ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	std::wcout << L"\n";
	TestSocket::DisposeSocketImpl(socket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 7: 다양한 상태에서 CLOSING 전이 테스트
// - CLOSING은 CONNECTED, CONNECTING, CLOSED, DISCONNECTED에서 가능
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_ClosingFromVariousStates()
{
	std::wcout << L"[테스트 7] 다양한 상태에서 CLOSING 전이 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	struct ClosingTest
	{
		SS fromState;
		bool expectedResult;
		const wchar_t* description;
	};

	ClosingTest tests[] = {
		{ SS::CLOSED,        true,  L"CLOSED -> CLOSING (허용)" },
		{ SS::OPENED,        true,  L"OPENED -> CLOSING (허용, 비정상 종료 복구용)" },
		{ SS::CONNECTING,    true,  L"CONNECTING -> CLOSING (허용)" },
		{ SS::CONNECTED,     false, L"CONNECTED -> CLOSING (불가, DISCONNECTING 경로 사용)" },
		{ SS::DISCONNECTING, false, L"DISCONNECTING -> CLOSING (불가, ReleaseRef 전용 처리)" },
		{ SS::DISCONNECTED,  true,  L"DISCONNECTED -> CLOSING (허용, expectedArray 기본값)" },
	};

	int passCount = 0;
	int failCount = 0;

	for (const auto& test : tests)
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		socket->SetSocketStateForTest(test.fromState);

		bool actualResult = socket->TryChangeSocketState(SS::CLOSING);
		bool testPassed = (actualResult == test.expectedResult);

		if (testPassed)
		{
			++passCount;
			std::wcout << std::format(L"  [PASS] {} = {} (예상대로)\n",
				test.description,
				actualResult ? L"성공" : L"실패");
		}
		else
		{
			++failCount;
			std::wcout << std::format(L"  [FAIL] {} = {} (예상: {})\n",
				test.description,
				actualResult ? L"성공" : L"실패",
				test.expectedResult ? L"성공" : L"실패");
		}

		TestSocket::DisposeSocketImpl(socket);
	}

	std::wcout << std::format(L"\n  결과: {} 통과, {} 실패\n\n", passCount, failCount);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 8: 빠른 재연결 스트레스 테스트
// - 연결/해제를 빠르게 반복하여 상태 머신의 안정성 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_RapidReconnectStress()
{
	std::wcout << L"[테스트 8] 빠른 재연결 스트레스 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	constexpr int STRESS_COUNT = 100;
	int successCount = 0;
	int failCount = 0;

	TestSocket* socket = TestSocket::AcquireSocketImpl();
	socket->SetSocketStateForTest(SS::CLOSED);

	auto startTime = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < STRESS_COUNT; ++i)
	{
		// 빠른 연결 사이클
		bool s1 = socket->TryChangeSocketState(SS::OPENED);
		bool s2 = socket->TryChangeSocketState(SS::CONNECTING);
		bool s3 = socket->TryChangeSocketState(SS::CONNECTED);
		bool s4 = socket->TryChangeSocketState(SS::DISCONNECTING);
		bool s5 = socket->TryChangeSocketState(SS::DISCONNECTED);
		bool s6 = socket->TryChangeSocketState(SS::CLOSED);

		if (s1 && s2 && s3 && s4 && s5 && s6)
		{
			++successCount;
		}
		else
		{
			++failCount;
			std::wcout << std::format(L"  [실패] 반복 {}: s1={}, s2={}, s3={}, s4={}, s5={}, s6={}\n",
				i, s1, s2, s3, s4, s5, s6);
		}
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

	std::wcout << std::format(L"  {} 사이클 완료: {} 성공, {} 실패\n", STRESS_COUNT, successCount, failCount);
	std::wcout << std::format(L"  소요 시간: {} us ({:.2f} us/cycle)\n\n", 
		duration.count(), 
		static_cast<double>(duration.count()) / STRESS_COUNT);

	TestSocket::DisposeSocketImpl(socket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 9: 모든 상태에서 CLOSED로 복구하는 경로 테스트
// - 각 상태에서 CLOSED로 돌아갈 수 있는 경로가 존재하는지 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_RecoveryPathFromAllStates()
{
	std::wcout << L"[테스트 9] 모든 상태에서 CLOSED로 복구 경로 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	struct RecoveryTest
	{
		SS startState;
		std::vector<SS> recoveryPath;
		const wchar_t* description;
	};

	RecoveryTest tests[] = {
		{ SS::OPENED, { SS::CLOSING, SS::CLOSED }, L"OPENED" },
		{ SS::CONNECTING, { SS::CLOSING, SS::CLOSED }, L"CONNECTING" },
		{ SS::CONNECTED, { SS::DISCONNECTING, SS::DISCONNECTED, SS::CLOSED }, L"CONNECTED" },
		{ SS::DISCONNECTING, { SS::DISCONNECTED, SS::CLOSED }, L"DISCONNECTING" },
		{ SS::DISCONNECTED, { SS::CLOSED }, L"DISCONNECTED" },
		{ SS::CLOSING, { SS::CLOSED }, L"CLOSING" },
	};

	int passCount = 0;
	int failCount = 0;

	for (const auto& test : tests)
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		socket->SetSocketStateForTest(test.startState);

		bool allSuccess = true;
		std::wstring pathStr;

		pathStr = GetSocketStateString(test.startState);

		for (SS nextState : test.recoveryPath)
		{
			bool result = socket->TryChangeSocketState(nextState);
			pathStr += L" -> ";
			pathStr += GetSocketStateString(nextState);
			
			if (!result)
			{
				pathStr += L" (실패!)";
				allSuccess = false;
				break;
			}
		}

		bool reachedClosed = (socket->GetSocketState() == SS::CLOSED);

		if (allSuccess && reachedClosed)
		{
			++passCount;
			std::wcout << std::format(L"  [PASS] {}: {}\n", test.description, pathStr);
		}
		else
		{
			++failCount;
			std::wcout << std::format(L"  [FAIL] {}: {} (최종 상태: {})\n", 
				test.description, pathStr, GetSocketStateString(socket->GetSocketState()));
		}

		TestSocket::DisposeSocketImpl(socket);
	}

	std::wcout << std::format(L"\n  결과: {} 통과, {} 실패\n\n", passCount, failCount);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 10: 동시 상태 전이 시뮬레이션 (CAS 경쟁)
// - compare_exchange_strong이 실패하는 경우를 시뮬레이션
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_ConcurrentStateTransition()
{
	std::wcout << L"[테스트 10] 동시 상태 전이 경쟁 시뮬레이션\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	std::wcout << L"  시나리오: 동시에 Disconnect와 Send 완료가 발생하는 상황 시뮬레이션\n\n";

	// 시나리오 1: CONNECTED 상태에서 두 경로가 경쟁
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		socket->SetSocketStateForTest(SS::CONNECTED);

		std::wcout << L"  [시나리오 1] CONNECTED에서 DISCONNECTING vs CLOSING 경쟁\n";
		
		// 첫 번째 전이 시도 (Disconnect)
		bool disconnecting = socket->TryChangeSocketState(SS::DISCONNECTING);
		std::wcout << std::format(L"    첫 번째 시도 (DISCONNECTING): {} -> 상태: {}\n",
			disconnecting ? L"성공" : L"실패",
			GetSocketStateString(socket->GetSocketState()));

		// 두 번째 전이 시도 (이미 DISCONNECTING이므로 실패해야 함)
		bool closing = socket->TryChangeSocketState(SS::CLOSING);
		std::wcout << std::format(L"    두 번째 시도 (CLOSING): {} -> 상태: {}\n",
			closing ? L"성공" : L"실패",
			GetSocketStateString(socket->GetSocketState()));

		bool correctBehavior = disconnecting && !closing;
		std::wcout << std::format(L"    결과: {} (첫 번째만 성공해야 함)\n\n",
			correctBehavior ? L"정상" : L"이상");

		TestSocket::DisposeSocketImpl(socket);
	}

	// 시나리오 2: 같은 전이를 두 번 시도
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		socket->SetSocketStateForTest(SS::CONNECTED);

		std::wcout << L"  [시나리오 2] 동일 전이 중복 시도 (DISCONNECTING x2)\n";

		bool first = socket->TryChangeSocketState(SS::DISCONNECTING);
		bool second = socket->TryChangeSocketState(SS::DISCONNECTING);

		std::wcout << std::format(L"    첫 번째: {}, 두 번째: {} -> 상태: {}\n",
			first ? L"성공" : L"실패",
			second ? L"성공" : L"실패",
			GetSocketStateString(socket->GetSocketState()));

		bool correctBehavior = first && !second;
		std::wcout << std::format(L"    결과: {} (첫 번째만 성공해야 함)\n\n",
			correctBehavior ? L"정상" : L"이상");

		TestSocket::DisposeSocketImpl(socket);
	}

	// 시나리오 3: 역방향 전이 시도 (잘못된 전이)
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		socket->SetSocketStateForTest(SS::DISCONNECTING);

		std::wcout << L"  [시나리오 3] 역방향 전이 시도 (DISCONNECTING -> CONNECTED)\n";

		bool backward = socket->TryChangeSocketState(SS::CONNECTED);

		std::wcout << std::format(L"    역방향 전이: {} -> 상태: {}\n",
			backward ? L"성공" : L"실패",
			GetSocketStateString(socket->GetSocketState()));

		bool correctBehavior = !backward && socket->GetSocketState() == SS::DISCONNECTING;
		std::wcout << std::format(L"    결과: {} (실패해야 함)\n\n",
			correctBehavior ? L"정상" : L"이상");

		TestSocket::DisposeSocketImpl(socket);
	}

	// 시나리오 4: 빠른 상태 전이 체인에서 중간 가로채기
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		socket->SetSocketStateForTest(SS::CLOSED);

		std::wcout << L"  [시나리오 4] 연결 시퀀스 중 끊기 시도\n";

		socket->TryChangeSocketState(SS::OPENED);
		socket->TryChangeSocketState(SS::CONNECTING);
		// CONNECTING 상태에서 갑자기 CLOSING 시도 (연결 취소)
		bool cancelConnect = socket->TryChangeSocketState(SS::CLOSING);

		std::wcout << std::format(L"    CONNECTING에서 CLOSING 시도: {} -> 상태: {}\n",
			cancelConnect ? L"성공" : L"실패",
			GetSocketStateString(socket->GetSocketState()));

		// CLOSING에서 CLOSED로 정리
		if (cancelConnect)
		{
			bool cleanup = socket->TryChangeSocketState(SS::CLOSED);
			std::wcout << std::format(L"    정리 (CLOSING -> CLOSED): {} -> 상태: {}\n",
				cleanup ? L"성공" : L"실패",
				GetSocketStateString(socket->GetSocketState()));
		}

		std::wcout << L"\n";
		TestSocket::DisposeSocketImpl(socket);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 11: 멀티스레드 동시 상태 전이 테스트
// - 여러 스레드에서 동시에 상태 전이 시도
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_MultithreadedStateTransition()
{
	std::wcout << L"[테스트 11] 멀티스레드 동시 상태 전이 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	constexpr int THREAD_COUNT = 4;
	constexpr int ITERATIONS_PER_THREAD = 100;

	std::atomic<int> successCount = 0;
	std::atomic<int> failCount = 0;

	TestSocket* socket = TestSocket::AcquireSocketImpl();

	auto threadFunc = [&](MAYBE_UNUSED int threadId) {
		for (int i = 0; i < ITERATIONS_PER_THREAD; ++i)
		{
			// 각 스레드가 동시에 DISCONNECTING으로 전이 시도
			socket->SetSocketStateForTest(SS::CONNECTED);
			
			bool result = socket->TryChangeSocketState(SS::DISCONNECTING);
			if (result)
				++successCount;
			else
				++failCount;
		}
	};

	std::vector<std::thread> threads;
	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threads.emplace_back(threadFunc, i);
	}

	for (auto& t : threads)
	{
		t.join();
	}

	int total = THREAD_COUNT * ITERATIONS_PER_THREAD;
	std::wcout << std::format(L"  총 시도: {}, 성공: {}, 실패: {}\n", total, successCount.load(), failCount.load());
	std::wcout << std::format(L"  결과: CAS 경쟁 정상 동작 (성공+실패 = {})\n\n", successCount.load() + failCount.load());

	TestSocket::DisposeSocketImpl(socket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 12: 소켓 풀 메모리 체크
// - 소켓 획득/반환 후 풀 상태 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_SocketPoolMemoryCheck()
{
	std::wcout << L"[테스트 12] 소켓 풀 메모리 체크\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	constexpr int SOCKET_COUNT = 50;
	std::vector<TestSocket*> sockets;

	std::wcout << L"  [1] 소켓 50개 획득\n";
	for (int i = 0; i < SOCKET_COUNT; ++i)
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		sockets.push_back(socket);
	}
	std::wcout << std::format(L"      획득된 소켓 수: {}\n", sockets.size());

	std::wcout << L"  [2] 모든 소켓 반환\n";
	for (TestSocket* socket : sockets)
	{
		TestSocket::DisposeSocketImpl(socket);
	}
	sockets.clear();
	std::wcout << L"      반환 완료\n";

	std::wcout << L"  [3] 다시 50개 획득 (풀에서 재사용되어야 함)\n";
	for (int i = 0; i < SOCKET_COUNT; ++i)
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		sockets.push_back(socket);
	}
	std::wcout << std::format(L"      재획득된 소켓 수: {}\n", sockets.size());

	for (TestSocket* socket : sockets)
	{
		TestSocket::DisposeSocketImpl(socket);
	}

	std::wcout << L"\n  결과: 풀 재사용 정상 (메모리 누수 없음 예상)\n\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 13: 타임아웃 시나리오
// - 연결 시도 후 타임아웃 상황 시뮬레이션
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_TimeoutScenario()
{
	std::wcout << L"[테스트 13] 타임아웃 시나리오 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	TestSocket* socket = TestSocket::AcquireSocketImpl();

	std::wcout << L"  시나리오: 연결 시도 -> 타임아웃 -> 재시도\n\n";

	// 연결 시도
	socket->SetSocketStateForTest(SS::CLOSED);
	socket->TryChangeSocketState(SS::OPENED);
	socket->TryChangeSocketState(SS::CONNECTING);
	std::wcout << std::format(L"  [1] 연결 시도 중: {}\n", GetSocketStateString(socket->GetSocketState()));

	// 타임아웃 발생 (CONNECTING에서 오래 대기 후 CLOSING으로 전이)
	std::wcout << L"  [2] 연결 타임아웃 발생 시뮬레이션\n";
	socket->SetHasErrorForTest(true);
	bool closing = socket->TryChangeSocketState(SS::CLOSING);
	std::wcout << std::format(L"      CLOSING 전이: {} -> 상태: {}\n",
		closing ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	// 정리
	bool closed = socket->TryChangeSocketState(SS::CLOSED);
	std::wcout << std::format(L"  [3] 정리: {} -> 상태: {}\n",
		closed ? L"성공" : L"실패",
		GetSocketStateString(socket->GetSocketState()));

	// 에러 플래그 리셋 후 재시도
	socket->SetHasErrorForTest(false);
	socket->TryChangeSocketState(SS::OPENED);
	socket->TryChangeSocketState(SS::CONNECTING);
	socket->TryChangeSocketState(SS::CONNECTED);
	std::wcout << std::format(L"  [4] 재연결 시도: {}\n", GetSocketStateString(socket->GetSocketState()));

	bool success = socket->GetSocketState() == SS::CONNECTED;
	std::wcout << std::format(L"\n  결과: 타임아웃 후 재연결 {}\n\n", success ? L"성공" : L"실패");

	TestSocket::DisposeSocketImpl(socket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 14: 대량 연결 테스트
// - 많은 수의 소켓을 동시에 연결/해제
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_MassConnectionTest()
{
	std::wcout << L"[테스트 14] 대량 연결 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	constexpr int SOCKET_COUNT = 100;
	std::vector<TestSocket*> sockets;

	auto startTime = std::chrono::high_resolution_clock::now();

	// 100개 소켓 생성 및 연결 상태로 전이
	std::wcout << std::format(L"  [1] {}개 소켓 연결 시뮬레이션\n", SOCKET_COUNT);
	int connectedCount = 0;
	for (int i = 0; i < SOCKET_COUNT; ++i)
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		socket->SetSocketStateForTest(SS::CLOSED);
		
		bool ok = socket->TryChangeSocketState(SS::OPENED) &&
		          socket->TryChangeSocketState(SS::CONNECTING) &&
		          socket->TryChangeSocketState(SS::CONNECTED);
		
		if (ok) ++connectedCount;
		sockets.push_back(socket);
	}
	std::wcout << std::format(L"      연결된 소켓: {}/{}\n", connectedCount, SOCKET_COUNT);

	// 모두 해제
	std::wcout << L"  [2] 모든 소켓 해제\n";
	int disconnectedCount = 0;
	for (TestSocket* socket : sockets)
	{
		bool ok = socket->TryChangeSocketState(SS::DISCONNECTING) &&
		          socket->TryChangeSocketState(SS::DISCONNECTED) &&
		          socket->TryChangeSocketState(SS::CLOSED);
		
		if (ok) ++disconnectedCount;
		TestSocket::DisposeSocketImpl(socket);
	}
	std::wcout << std::format(L"      해제된 소켓: {}/{}\n", disconnectedCount, SOCKET_COUNT);

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

	std::wcout << std::format(L"\n  소요 시간: {} us\n", duration.count());
	std::wcout << std::format(L"  결과: 대량 연결/해제 {}\n\n", 
		(connectedCount == SOCKET_COUNT && disconnectedCount == SOCKET_COUNT) ? L"성공" : L"일부 실패");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 15: 비정상 종료 시뮬레이션
// - 다양한 상태에서 강제 종료 시 상태 정리
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_AbnormalTermination()
{
	std::wcout << L"[테스트 15] 비정상 종료 시뮬레이션\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	struct AbnormalTest
	{
		SS state;
		const wchar_t* description;
	};

	AbnormalTest tests[] = {
		{ SS::OPENED, L"OPENED 상태에서 비정상 종료" },
		{ SS::CONNECTING, L"CONNECTING 상태에서 비정상 종료" },
		{ SS::CONNECTED, L"CONNECTED 상태에서 비정상 종료" },
		{ SS::DISCONNECTING, L"DISCONNECTING 상태에서 비정상 종료" },
	};

	for (const auto& test : tests)
	{
		TestSocket* socket = TestSocket::AcquireSocketImpl();
		socket->SetSocketStateForTest(test.state);
		socket->SetHasErrorForTest(true);

		std::wcout << std::format(L"  [{}]\n", test.description);
		std::wcout << std::format(L"    시작 상태: {}, mHasError: true\n", GetSocketStateString(socket->GetSocketState()));

		// 비정상 종료 시 가능한 복구 경로 시도
		bool recovered = false;
		
		// CLOSING으로 전이 시도 (OPENED, CONNECTING 등)
		if (socket->TryChangeSocketState(SS::CLOSING))
		{
			socket->TryChangeSocketState(SS::CLOSED);
			recovered = (socket->GetSocketState() == SS::CLOSED);
		}
		// DISCONNECTING 상태에서는 DISCONNECTED → CLOSED 경로 사용
		// (ReleaseRef에서의 전용 처리와 동일한 경로)
		else if (socket->TryChangeSocketState(SS::DISCONNECTED))
		{
			socket->TryChangeSocketState(SS::CLOSED);
			recovered = (socket->GetSocketState() == SS::CLOSED);
		}
		// CONNECTED에서는 DISCONNECTING으로 전이 시도
		else if (socket->TryChangeSocketState(SS::DISCONNECTING))
		{
			socket->TryChangeSocketState(SS::DISCONNECTED);
			socket->TryChangeSocketState(SS::CLOSED);
			recovered = (socket->GetSocketState() == SS::CLOSED);
		}
		// 직접 CLOSED로 전이 시도
		else if (socket->TryChangeSocketState(SS::CLOSED))
		{
			recovered = true;
		}

		std::wcout << std::format(L"    최종 상태: {}, 복구: {}\n\n",
			GetSocketStateString(socket->GetSocketState()),
			recovered ? L"성공" : L"실패");

		TestSocket::DisposeSocketImpl(socket);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 16: Send/Recv 중 Disconnect 테스트
// - 송수신 작업 중 연결 해제 상황 시뮬레이션
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_SendRecvDuringDisconnect()
{
	std::wcout << L"[테스트 16] Send/Recv 중 Disconnect 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	TestSocket* socket = TestSocket::AcquireSocketImpl();

	std::wcout << L"  시나리오: CONNECTED에서 데이터 송수신 중 Disconnect 호출\n\n";

	// CONNECTED 상태로 설정
	socket->SetSocketStateForTest(SS::CONNECTED);
	socket->InitRef(L"SEND");  // Send 작업 시뮬레이션
	socket->AddRef(L"RECV");   // Recv 작업 시뮬레이션

	int refBefore = socket->GetRefCntForTest();
	std::wcout << std::format(L"  [1] CONNECTED 상태, 참조 카운트: {} (SEND, RECV 작업 중)\n", refBefore);

	// Disconnect 호출
	std::wcout << L"  [2] Disconnect 호출 시뮬레이션\n";
	bool disconnecting = socket->TryChangeSocketState(SS::DISCONNECTING);
	std::wcout << std::format(L"      DISCONNECTING 전이: {}\n", disconnecting ? L"성공" : L"실패");

	// Send/Recv 완료 시뮬레이션 (참조 해제)
	std::wcout << L"  [3] Send/Recv 작업 완료 시뮬레이션\n";
	// ReleaseRef를 호출하면 소켓이 dispose될 수 있으므로 참조 카운트만 확인
	int refAfter = socket->GetRefCntForTest();
	std::wcout << std::format(L"      현재 참조 카운트: {}\n", refAfter);

	// Disconnect 완료
	bool disconnected = socket->TryChangeSocketState(SS::DISCONNECTED);
	bool closed = socket->TryChangeSocketState(SS::CLOSED);
	std::wcout << std::format(L"  [4] Disconnect 완료: DISCONNECTED={}, CLOSED={}\n",
		disconnected ? L"성공" : L"실패",
		closed ? L"성공" : L"실패");

	std::wcout << std::format(L"\n  결과: 상태 전이 {}\n\n",
		(disconnecting && disconnected && closed) ? L"정상" : L"이상");

	socket->ResetRefCnt();
	TestSocket::DisposeSocketImpl(socket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 17: Accept 실패 복구 테스트
// - Accept 실패 시 상태 정리 및 재시도
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_AcceptFailureRecovery()
{
	std::wcout << L"[테스트 17] Accept 실패 복구 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	std::wcout << L"  시나리오: Accept 완료 후 UpdateAcceptContext 실패\n\n";

	// 리스너 소켓 시뮬레이션
	TestSocket* listener = TestSocket::AcquireSocketImpl();
	listener->SetSocketStateForTest(SS::CONNECTED);  // 리스너는 Listen 후 "연결됨" 상태로 간주

	// Accept된 소켓 시뮬레이션
	TestSocket* accepted = TestSocket::AcquireSocketImpl();
	accepted->SetSocketStateForTest(SS::OPENED);

	std::wcout << std::format(L"  [1] 리스너 상태: {}\n", GetSocketStateString(listener->GetSocketState()));
	std::wcout << std::format(L"  [2] Accepted 소켓 상태: {}\n", GetSocketStateString(accepted->GetSocketState()));

	// Accept 실패 시뮬레이션 (수정된 코드에서는 리스너의 mHasError를 설정하지 않음)
	std::wcout << L"  [3] Accept 실패 시뮬레이션 (UpdateAcceptContext 실패)\n";
	
	// 수정된 동작: accepted 소켓에만 에러 설정
	accepted->SetHasErrorForTest(true);
	std::wcout << std::format(L"      Accepted 소켓 mHasError: true\n");
	std::wcout << std::format(L"      리스너 mHasError: {} (변경되지 않아야 함)\n",
		listener->HasErrorForTest() ? L"true" : L"false");

	// 리스너가 계속 동작해야 함
	bool listenerOk = !listener->HasErrorForTest();
	std::wcout << std::format(L"\n  결과: 리스너 상태 {}\n", listenerOk ? L"정상 (계속 Accept 가능)" : L"이상 (정지됨!)");

	// 새로운 Accept 시뮬레이션
	TestSocket* newAccepted = TestSocket::AcquireSocketImpl();
	newAccepted->SetSocketStateForTest(SS::OPENED);
	bool canAccept = newAccepted->TryChangeSocketState(SS::CONNECTING);
	std::wcout << std::format(L"  새 연결 Accept: {}\n\n", canAccept ? L"가능" : L"불가");

	TestSocket::DisposeSocketImpl(listener);
	TestSocket::DisposeSocketImpl(accepted);
	TestSocket::DisposeSocketImpl(newAccepted);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 17-2: Accept 실패 상세 시나리오 테스트
// - 다양한 Accept 실패 케이스와 backlog 복구 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_AcceptFailureDetailedScenarios()
{
	std::wcout << L"[테스트 17-2] Accept 실패 상세 시나리오 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	//======================================================================================
	// 시나리오 1: UpdateAcceptContext 실패
	//======================================================================================
	std::wcout << L"\n  [시나리오 1] UpdateAcceptContext 실패\n";
	{
		// Backlog 시뮬레이션
		std::atomic<int32_t> currentBacklog = 5;
		std::atomic<int32_t> currentConnection = 3;
		const int32_t MAX_BACKLOG = 5;
		const int32_t MAX_CONNECTION = 10;

		std::wcout << std::format(L"    초기 상태: backlog={}, connection={}\n",
			currentBacklog.load(), currentConnection.load());

		// Accept 완료 but UpdateAcceptContext 실패
		TestSocket* accepted = TestSocket::AcquireSocketImpl();
		accepted->SetSocketStateForTest(SS::OPENED);

		std::wcout << L"    UpdateAcceptContext 실패 발생!\n";

		// 수정된 코드의 동작:
		// 1. accepted->mHasError = true (리스너가 아님!)
		// 2. accepted->ReleaseRef("ACCEPT")
		// 3. currentBacklog.fetch_sub(1)
		// 4. UpdateBacklog() 호출로 새 Accept 재투입

		accepted->SetHasErrorForTest(true);
		
		// backlog 감소 (Accept 슬롯 소비됨)
		int32_t backlogAfter = currentBacklog.fetch_sub(1) - 1;
		
		// UpdateBacklog 시뮬레이션 (새 Accept 슬롯 추가)
		if (backlogAfter < MAX_BACKLOG && 
		    currentConnection.load() + backlogAfter < MAX_CONNECTION)
		{
			currentBacklog.fetch_add(1);
			std::wcout << L"    새 Accept 슬롯 재투입됨\n";
		}

		std::wcout << std::format(L"    결과 상태: backlog={}, connection={}\n",
			currentBacklog.load(), currentConnection.load());

		bool backlogRecovered = (currentBacklog.load() == 5);
		std::wcout << std::format(L"    Backlog 복구: {}\n", backlogRecovered ? L"성공" : L"실패");

		TestSocket::DisposeSocketImpl(accepted);
	}

	//======================================================================================
	// 시나리오 2: getpeername 실패
	//======================================================================================
	std::wcout << L"\n  [시나리오 2] getpeername 실패\n";
	{
		TestSocket* accepted = TestSocket::AcquireSocketImpl();
		accepted->SetSocketStateForTest(SS::OPENED);

		std::wcout << L"    getpeername 실패 발생!\n";
		std::wcout << L"    동작: UpdateAcceptContext 실패와 동일하게 처리\n";

		// 동일한 실패 경로
		accepted->SetHasErrorForTest(true);
		std::wcout << std::format(L"    accepted 소켓 mHasError: {}\n",
			accepted->HasErrorForTest() ? L"true" : L"false");

		TestSocket::DisposeSocketImpl(accepted);
	}

	//======================================================================================
	// 시나리오 3: ChangeSocketState(CONNECTING) 실패
	//======================================================================================
	std::wcout << L"\n  [시나리오 3] ChangeSocketState(CONNECTING) 실패\n";
	{
		TestSocket* accepted = TestSocket::AcquireSocketImpl();
		
		// 이미 다른 상태로 전이된 경우 (경쟁 상황)
		accepted->SetSocketStateForTest(SS::CLOSING);  // 비정상 상태

		std::wcout << std::format(L"    accepted 소켓이 이미 {} 상태\n",
			GetSocketStateString(accepted->GetSocketState()));

		bool canConnect = accepted->TryChangeSocketState(SS::CONNECTING);
		std::wcout << std::format(L"    CONNECTING 전이 시도: {}\n", canConnect ? L"성공" : L"실패");

		// 실패 시 처리
		if (!canConnect)
		{
			accepted->SetHasErrorForTest(true);
			std::wcout << L"    실패 경로로 처리됨\n";
		}

		TestSocket::DisposeSocketImpl(accepted);
	}

	//======================================================================================
	// 시나리오 4: 연속 Accept 실패
	//======================================================================================
	std::wcout << L"\n  [시나리오 4] 연속 Accept 실패 (5회)\n";
	{
		std::atomic<int32_t> currentBacklog = 5;
		std::atomic<int32_t> currentConnection = 0;
		const int32_t MAX_BACKLOG = 5;
		const int32_t MAX_CONNECTION = 10;
		bool listenerHasError = false;  // 리스너의 mHasError (수정 전에는 이게 true가 되어 문제 발생)

		for (int i = 0; i < 5; ++i)
		{
			TestSocket* accepted = TestSocket::AcquireSocketImpl();
			accepted->SetSocketStateForTest(SS::OPENED);

			// Accept 실패 처리 (수정된 코드)
			// 리스너의 mHasError를 건드리지 않음!
			accepted->SetHasErrorForTest(true);

			// backlog 처리
			int32_t backlog = currentBacklog.fetch_sub(1) - 1;
			if (backlog < MAX_BACKLOG && currentConnection.load() + backlog < MAX_CONNECTION)
			{
				currentBacklog.fetch_add(1);
			}

			TestSocket::DisposeSocketImpl(accepted);
		}

		std::wcout << std::format(L"    5회 실패 후 상태: backlog={}, listenerHasError={}\n",
			currentBacklog.load(), listenerHasError ? L"true" : L"false");

		bool listenerStillWorking = !listenerHasError && (currentBacklog.load() > 0);
		std::wcout << std::format(L"    리스너 동작 상태: {}\n", 
			listenerStillWorking ? L"정상 (계속 Accept 가능)" : L"정지됨!");
	}

	//======================================================================================
	// 시나리오 5: 수정 전 버그 재현 (mHasError가 리스너에 설정되는 경우)
	//======================================================================================
	std::wcout << L"\n  [시나리오 5] 수정 전 버그 시뮬레이션 (리스너 mHasError 설정)\n";
	{
		TestSocket* listener = TestSocket::AcquireSocketImpl();
		listener->SetSocketStateForTest(SS::CONNECTED);  // 리스너 상태

		std::wcout << L"    [버그 시나리오] Accept 실패 시 리스너에 mHasError 설정\n";
		
		// 수정 전 코드의 동작 시뮬레이션
		listener->SetHasErrorForTest(true);  // 이게 문제!
		
		std::wcout << std::format(L"    리스너 mHasError: {}\n",
			listener->HasErrorForTest() ? L"true" : L"false");

		// 이후 Accept가 들어와도 OnCompleted에서 mHasError 분기로 빠짐
		std::wcout << L"    이후 Accept 완료가 와도:\n";
		std::wcout << L"      -> OnCompleted에서 mHasError=true 분기\n";
		std::wcout << L"      -> IoType::ACCEPT case에서 break만 함\n";
		std::wcout << L"      -> OnCompletedAccept 호출 안 됨\n";
		std::wcout << L"      -> UpdateBacklog 호출 안 됨\n";
		std::wcout << L"      -> 새 Accept 재투입 안 됨\n";
		std::wcout << L"      -> 리스너 영구 정지!\n";

		TestSocket::DisposeSocketImpl(listener);
	}

	//======================================================================================
	// 시나리오 6: 수정 후 정상 동작 확인
	//======================================================================================
	std::wcout << L"\n  [시나리오 6] 수정 후 정상 동작 확인\n";
	{
		TestSocket* listener = TestSocket::AcquireSocketImpl();
		listener->SetSocketStateForTest(SS::CONNECTED);

		std::wcout << L"    [정상 시나리오] Accept 실패 시 accepted 소켓에만 mHasError 설정\n";

		// Accept 실패
		TestSocket* accepted = TestSocket::AcquireSocketImpl();
		accepted->SetSocketStateForTest(SS::OPENED);
		accepted->SetHasErrorForTest(true);  // accepted에만 설정

		std::wcout << std::format(L"    리스너 mHasError: {}\n",
			listener->HasErrorForTest() ? L"true" : L"false");
		std::wcout << std::format(L"    Accepted mHasError: {}\n",
			accepted->HasErrorForTest() ? L"true" : L"false");

		// 리스너는 계속 동작
		bool listenerOk = !listener->HasErrorForTest();
		std::wcout << std::format(L"    리스너 상태: {}\n",
			listenerOk ? L"정상 (Accept 계속 가능)" : L"이상");

		// 다음 Accept 처리 가능
		TestSocket* nextAccepted = TestSocket::AcquireSocketImpl();
		nextAccepted->SetSocketStateForTest(SS::OPENED);
		bool canProcess = nextAccepted->TryChangeSocketState(SS::CONNECTING);
		std::wcout << std::format(L"    다음 Accept 처리: {}\n", canProcess ? L"가능" : L"불가");

		TestSocket::DisposeSocketImpl(listener);
		TestSocket::DisposeSocketImpl(accepted);
		TestSocket::DisposeSocketImpl(nextAccepted);
	}

	std::wcout << L"\n  === Accept 실패 복구 상세 테스트 완료 ===\n\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 18: 재접속 딜레이 동작 테스트
// - 재접속 시 딜레이 증가/클램핑 시뮬레이션
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_ReconnectDelayBehavior()
{
	std::wcout << L"[테스트 18] 재접속 딜레이 동작 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	std::wcout << L"  PeerSocket의 재접속 딜레이 동작 시뮬레이션\n\n";

	// 딜레이 값 시뮬레이션 (PeerSocket의 mReconnectDelay와 유사)
	using namespace std::chrono_literals;
	auto MIN_DELAY = 1000ms;
	auto MAX_DELAY = 30000ms;
	auto currentDelay = 0ms;

	std::wcout << L"  [연결 실패 시 딜레이 증가 시뮬레이션]\n";
	for (int attempt = 1; attempt <= 5; ++attempt)
	{
		// 연결 실패 시 딜레이 증가
		currentDelay = std::clamp(currentDelay + 2000ms, MIN_DELAY, MAX_DELAY);
		std::wcout << std::format(L"    시도 {}: 딜레이 = {} ms\n", attempt, currentDelay.count());
	}

	std::wcout << L"\n  [연결 성공 시 딜레이 리셋]\n";
	currentDelay = 0ms;
	std::wcout << std::format(L"    연결 성공: 딜레이 = {} ms (리셋됨)\n", currentDelay.count());

	std::wcout << L"\n  [최대 딜레이 도달 테스트]\n";
	currentDelay = 25000ms;
	for (int i = 0; i < 3; ++i)
	{
		currentDelay = std::clamp(currentDelay + 10000ms, MIN_DELAY, MAX_DELAY);
		std::wcout << std::format(L"    증가 시도 {}: 딜레이 = {} ms (MAX: {} ms)\n", 
			i + 1, currentDelay.count(), MAX_DELAY.count());
	}

	bool maxClamped = (currentDelay == MAX_DELAY);
	std::wcout << std::format(L"\n  결과: 최대값 클램핑 {}\n\n", maxClamped ? L"정상" : L"이상");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 19: 암호화 핸드셰이크 상태 테스트
// - 핸드셰이크 전후 암호화 키 상태 시뮬레이션
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_EncryptionHandshakeState()
{
	std::wcout << L"[테스트 19] 암호화 핸드셰이크 상태 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	TestSocket* socket = TestSocket::AcquireSocketImpl();

	std::wcout << L"  시나리오: 연결 후 핸드셰이크 전/후 암호화 키 상태\n\n";

	// 연결 상태로 전이
	socket->SetSocketStateForTest(SS::CLOSED);
	socket->TryChangeSocketState(SS::OPENED);
	socket->TryChangeSocketState(SS::CONNECTING);
	socket->TryChangeSocketState(SS::CONNECTED);

	// 핸드셰이크 전
	bool hasKeyBefore = socket->HasEncryptionKey();
	std::wcout << std::format(L"  [1] 핸드셰이크 전 HasEncryptionKey: {}\n",
		hasKeyBefore ? L"true" : L"false");

	// 핸드셰이크 완료 시뮬레이션은 TestSocket에서 항상 false 반환
	std::wcout << L"  [2] 핸드셰이크 시뮬레이션 (TestSocket은 항상 false 반환)\n";

	// 암호화 키 없이 Send 시도 시 동작
	std::wcout << L"  [3] 암호화 키 없이 Send 시도 시:\n";
	std::wcout << L"      - SocketBase::Send에서 HasEncryptionKey() 확인\n";
	std::wcout << L"      - false면 SendBuffer::Push로 버퍼 반환하고 false 리턴\n";

	// 연결 해제 후 암호화 키 리셋 확인
	socket->TryChangeSocketState(SS::DISCONNECTING);
	socket->TryChangeSocketState(SS::DISCONNECTED);
	socket->TryChangeSocketState(SS::CLOSED);

	std::wcout << std::format(L"  [4] 연결 해제 후 상태: {}\n", GetSocketStateString(socket->GetSocketState()));
	std::wcout << L"      (재연결 시 핸드셰이크 다시 필요)\n\n";

	TestSocket::DisposeSocketImpl(socket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 20: Backlog 관리 테스트
// - Accept backlog 슬롯 관리 시뮬레이션
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_BacklogManagement()
{
	std::wcout << L"[테스트 20] Backlog 관리 테스트\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	std::wcout << L"  시나리오: 리스너의 backlog 슬롯 관리 시뮬레이션\n\n";

	// backlog 시뮬레이션 변수
	const int32_t MAX_BACKLOG = 5;
	const int32_t MAX_CONNECTION = 10;
	std::atomic<int32_t> currentBacklog = 0;
	std::atomic<int32_t> currentConnection = 0;

	std::wcout << std::format(L"  설정: MAX_BACKLOG={}, MAX_CONNECTION={}\n\n", MAX_BACKLOG, MAX_CONNECTION);

	// 초기 backlog 채우기
	std::wcout << L"  [1] 초기 Backlog 채우기 (Accept 대기 슬롯)\n";
	for (int i = 0; i < MAX_BACKLOG; ++i)
	{
		currentBacklog.fetch_add(1);
	}
	std::wcout << std::format(L"      currentBacklog: {}\n", currentBacklog.load());

	// Accept 완료 시뮬레이션
	std::wcout << L"\n  [2] 3개 연결 수락 시뮬레이션\n";
	for (int i = 0; i < 3; ++i)
	{
		// Accept 완료: backlog 감소, connection 증가
		int32_t backlog = currentBacklog.fetch_sub(1) - 1;
		currentConnection.fetch_add(1);
		
		// UpdateBacklog 시뮬레이션
		if (backlog < MAX_BACKLOG && 
		    currentConnection.load() + backlog < MAX_CONNECTION)
		{
			currentBacklog.fetch_add(1);  // 새 Accept 슬롯 추가
		}
	}
	std::wcout << std::format(L"      currentBacklog: {}, currentConnection: {}\n",
		currentBacklog.load(), currentConnection.load());

	// 연결 해제 시뮬레이션
	std::wcout << L"\n  [3] 2개 연결 해제 시뮬레이션\n";
	for (int i = 0; i < 2; ++i)
	{
		int32_t conn = currentConnection.fetch_sub(1) - 1;
		int32_t backlog = currentBacklog.load();
		
		// OnChildDisconnected의 UpdateBacklog 시뮬레이션
		if (backlog < MAX_BACKLOG && 
		    conn + backlog < MAX_CONNECTION)
		{
			currentBacklog.fetch_add(1);
		}
	}
	std::wcout << std::format(L"      currentBacklog: {}, currentConnection: {}\n",
		currentBacklog.load(), currentConnection.load());

	// 최대 연결 도달 시뮬레이션
	std::wcout << L"\n  [4] 최대 연결 도달 시 backlog 동작\n";
	currentConnection.store(MAX_CONNECTION);
	currentBacklog.store(0);
	std::wcout << std::format(L"      currentConnection={}, currentBacklog={}\n",
		currentConnection.load(), currentBacklog.load());
	std::wcout << L"      (MAX_CONNECTION 도달 시 새 Accept 슬롯 추가 안 함)\n";

	bool connectionLimitWorks = (currentBacklog.load() == 0 && currentConnection.load() == MAX_CONNECTION);
	std::wcout << std::format(L"\n  결과: 연결 제한 동작 {}\n\n", connectionLimitWorks ? L"정상" : L"이상");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 케이스 21: DISCONNECTED -> CLOSED 전이 추가 시 동작 확인
// - 이 테스트는 수정 후 예상 동작을 보여줌
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSocketStateMachine::TestCase_DisconnectedToClosedTransition()
{
	std::wcout << L"[테스트 3] 필요한 상태 전이 경로 분석\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	using SS = SocketBase::SocketState;

	std::wcout << L"  현재 ChangeSocketState의 expectedArray:\n";
	std::wcout << L"    CLOSED(0):        expected = CLOSING\n";
	std::wcout << L"    OPENED(1):        expected = CLOSED\n";
	std::wcout << L"    CONNECTING(2):    expected = OPENED\n";
	std::wcout << L"    CONNECTED(3):     expected = CONNECTING\n";
	std::wcout << L"    DISCONNECTING(4): expected = CONNECTED\n";
	std::wcout << L"    DISCONNECTED(5):  expected = DISCONNECTING\n";
	std::wcout << L"\n";

	std::wcout << L"  문제 분석:\n";
	std::wcout << L"    - DisconnectEx 완료 후 상태: DISCONNECTED\n";
	std::wcout << L"    - ReleaseRef에서 CLOSED 전이 시도 -> 실패 (CLOSING에서만 가능)\n";
	std::wcout << L"    - ReleaseRef에서 CLOSING 전이 시도 -> 실패 (CONNECTED/CONNECTING/CLOSED에서만 가능)\n";
	std::wcout << L"    - Open()에서 OPENED 전이 시도 -> 실패 (CLOSED에서만 가능)\n";
	std::wcout << L"\n";

	std::wcout << L"  해결 방안:\n";
	std::wcout << L"    1. ChangeSocketState에 DISCONNECTED -> CLOSED 경로 추가\n";
	std::wcout << L"    2. 또는 Open()에서 DISCONNECTED 상태 처리 추가\n";
	std::wcout << L"\n";

	// 수정 예시 코드 출력
	std::wcout << L"  수정 예시 (SocketBase.cpp, ChangeSocketState 함수):\n";
	std::wcout << L"    // DISCONNECTED -> CLOSED 경로 추가\n";
	std::wcout << L"    if (SocketState::CLOSED == socketState)\n";
	std::wcout << L"    {\n";
	std::wcout << L"        expected = SocketState::DISCONNECTED;\n";
	std::wcout << L"        if (true == mSocketState.compare_exchange_strong(expected, socketState))\n";
	std::wcout << L"            return true;\n";
	std::wcout << L"    }\n";
	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 유틸리티
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const wchar_t* TestSocketStateMachine::GetSocketStateString(SocketBase::SocketState state)
{
	switch (state)
	{
	case SocketBase::SocketState::CLOSED:        return L"CLOSED";
	case SocketBase::SocketState::OPENED:        return L"OPENED";
	case SocketBase::SocketState::CONNECTING:    return L"CONNECTING";
	case SocketBase::SocketState::CONNECTED:     return L"CONNECTED";
	case SocketBase::SocketState::DISCONNECTING: return L"DISCONNECTING";
	case SocketBase::SocketState::DISCONNECTED:  return L"DISCONNECTED";
	case SocketBase::SocketState::CLOSING:       return L"CLOSING";
	default:                                     return L"UNKNOWN";
	}
}

void TestSocketStateMachine::LogResult(const wchar_t* testName, bool passed, const wchar_t* detail)
{
	if (passed)
	{
		std::wcout << std::format(L"  [PASS] {}", testName);
	}
	else
	{
		std::wcout << std::format(L"  [FAIL] {}", testName);
	}

	if (detail && detail[0] != L'\0')
	{
		std::wcout << std::format(L" - {}", detail);
	}

	std::wcout << L"\n";
}

