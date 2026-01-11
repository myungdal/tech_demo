// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Test/App/TestAppBase.h"
#include "ServerEngine/Lock/Lock.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ReadLock/WriteLock의 이동(transfer) semantics 테스트
// RAII 락 가드가 복사 금지/이동 허용 정책을 올바르게 지키는지 검증한다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TestLock final : public TestAppBase
{
public:
	explicit TestLock(AppArg appArg) : TestAppBase(appArg) {}

public:
	void OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher) override;
	void OnStarted() override;
	void OnUpdate() override;

private:
	// 기본 동작 테스트
	void TestCase_BasicReadLock();
	void TestCase_BasicWriteLock();
	
	// 이동 생성자 테스트
	void TestCase_ReadLockCopyTransfer();
	void TestCase_WriteLockCopyTransfer();
	
	// 함수 반환(mv) 테스트
	void TestCase_ReadLockReturnTransfer();
	void TestCase_WriteLockReturnTransfer();
	void TestCase_ExplicitCopyOnReturn();
	void TestCase_MoveSemantics();
	
	// 이중 해제/미해제 감지 테스트
	void TestCase_DoubleReleaseCheck();
	void TestCase_NoReleaseCheck();
	
	// tLockedCount 일관성 테스트
	void TestCase_LockedCountConsistency();
	
	// 멀티스레드 이동 테스트
	void TestCase_MultithreadLockTransfer();
	
	// structured binding 패턴 테스트
	void TestCase_StructuredBindingPattern();

private:
	// 락을 반환하는 헬퍼 함수(함수 경계 이동 시나리오)
	ReadLock AcquireReadLock(Lock& lock);
	WriteLock AcquireWriteLock(Lock& lock);
	
	// structured binding 시나리오
	std::tuple<int, ReadLock> FindDataWithReadLock(Lock& lock, int value);
	std::tuple<int*, WriteLock> FindDataWithWriteLock(Lock& lock, int& value);
};

inline std::shared_ptr<TestLock> gTestLock = nullptr;

