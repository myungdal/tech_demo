// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Test/App/TestAppBase.h"
#include "ServerEngine/Socket/Socket.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트용 소켓 클래스 - 상태 전이를 검증하기 위한 목적
// SocketBase에 friend class TestSocket; 선언 필요 (TEST_BUILD 매크로 사용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TestSocket final : public Socket<TestSocket, 16>
{
public:
	// SocketBase의 protected 멤버에 접근하기 위한 래퍼
	SocketState GetSocketState() const { return mSocketState.load(); }
	void SetSocketStateForTest(SocketState state) { mSocketState.store(state); }
	int32_t GetRefCntForTest() const { return GetRefCnt(); }
	bool HasErrorForTest() const { return mHasError; }
	void SetHasErrorForTest(bool hasError) { mHasError = hasError; }
	
	// ChangeSocketState를 외부에서 호출 가능하게 (friend 선언으로 private 접근)
	bool TryChangeSocketState(SocketState state) { return ChangeSocketState(state); }

	// IOCP 설정 없이 테스트하기 위해
	void SetIocpForTest(std::shared_ptr<Iocp> iocp) { SetIocp(iocp); }

public:
	void OnConnected(bool succeed) override { MAYBE_UNUSED auto _ = succeed; }
	void OnDisconnected() override {}
	Result OnRecv(uint8_t* buf, size_t len, OUT size_t& recved) override 
	{ 
		MAYBE_UNUSED auto _ = buf;
		MAYBE_UNUSED auto __ = len;
		recved = 0;
		return Result::SUCCEEDED; 
	}
	bool HasEncryptionKey() const override { return false; }
	void OnTryEncrypt(uint8_t* buf, int32_t len) override 
	{
		MAYBE_UNUSED auto _ = buf;
		MAYBE_UNUSED auto __ = len;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트 앱
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TestSocketStateMachine final : public TestAppBase
{
public:
	explicit TestSocketStateMachine(AppArg appArg) : TestAppBase(appArg) {}

public:
	void OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher) override;
	void OnStarted() override;
	void OnUpdate() override;

private:
	// 기본 테스트 케이스들
	void TestCase_StateTransitionTable();
	void TestCase_ReconnectBlocked();
	void TestCase_MultipleReconnect();
	void TestCase_ErrorRecovery();
	void TestCase_RefCountBalance();
	void TestCase_ConnectFailureRecovery();
	void TestCase_ClosingFromVariousStates();
	void TestCase_RapidReconnectStress();
	void TestCase_RecoveryPathFromAllStates();
	void TestCase_ConcurrentStateTransition();

	// 추가 테스트 케이스들
	void TestCase_MultithreadedStateTransition();
	void TestCase_SocketPoolMemoryCheck();
	void TestCase_TimeoutScenario();
	void TestCase_MassConnectionTest();
	void TestCase_AbnormalTermination();
	void TestCase_SendRecvDuringDisconnect();
	void TestCase_AcceptFailureRecovery();
	void TestCase_AcceptFailureDetailedScenarios();
	void TestCase_ReconnectDelayBehavior();
	void TestCase_EncryptionHandshakeState();
	void TestCase_BacklogManagement();

	void TestCase_DisconnectedToClosedTransition();

	// 유틸리티
	static const wchar_t* GetSocketStateString(SocketBase::SocketState state);
	void LogResult(const wchar_t* testName, bool passed, const wchar_t* detail = L"");
};

inline std::shared_ptr<TestSocketStateMachine> gTestSocketStateMachine = nullptr;

