// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;

#include "TestLock.h"


void TestLock::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestLock::OnStarted()
{
	std::wcout << L"================================================================================\n";
	std::wcout << L"=== ReadLock/WriteLock ����/�̰� �׽�Ʈ ���� ===\n";
	std::wcout << L"================================================================================\n";

	TestCase_BasicReadLock();
	TestCase_BasicWriteLock();
	TestCase_ReadLockCopyTransfer();
	TestCase_WriteLockCopyTransfer();
	TestCase_ReadLockReturnTransfer();
	TestCase_WriteLockReturnTransfer();
	TestCase_ExplicitCopyOnReturn();
	TestCase_MoveSemantics();
	TestCase_DoubleReleaseCheck();
	TestCase_NoReleaseCheck();
	TestCase_LockedCountConsistency();
	TestCase_StructuredBindingPattern();
	// TestCase_MultithreadLockTransfer();  // 필요 시 활성화

	std::wcout << L"================================================================================\n";
	std::wcout << L"=== ReadLock/WriteLock �׽�Ʈ �Ϸ� ===\n";
	std::wcout << L"================================================================================\n";
}

void TestLock::OnUpdate()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 1: 기본 ReadLock 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestLock::TestCase_BasicReadLock()
{
	std::wcout << L"[�׽�Ʈ 1] �⺻ ReadLock ����\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	Lock lock;
	int32_t initialLockedCount = tLockedCount;

	std::wcout << std::format(L"  �ʱ� tLockedCount: {}\n", initialLockedCount);
	std::wcout << std::format(L"  IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");

	{
		ReadLock readLock(lock);
		std::wcout << std::format(L"  ReadLock ȹ�� �� tLockedCount: {}\n", tLockedCount);
		std::wcout << std::format(L"  IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");

		if (tLockedCount != initialLockedCount + 1)
		{
			std::wcout << L"  [FAIL] tLockedCount�� �������� ����\n";
		}
		else if (!lock.IsReadLocked())
		{
			std::wcout << L"  [FAIL] ���� ȹ����� ����\n";
		}
		else
		{
			std::wcout << L"  [PASS] ReadLock ȹ�� ����\n";
		}
	}

	std::wcout << std::format(L"  ReadLock ���� �� tLockedCount: {}\n", tLockedCount);
	std::wcout << std::format(L"  IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");

	if (tLockedCount != initialLockedCount)
	{
		std::wcout << L"  [FAIL] tLockedCount�� �������� ����\n";
	}
	else if (lock.IsReadLocked())
	{
		std::wcout << L"  [FAIL] ���� �������� ����\n";
	}
	else
	{
		std::wcout << L"  [PASS] ReadLock ���� ����\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 2: 기본 WriteLock 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestLock::TestCase_BasicWriteLock()
{
	std::wcout << L"[�׽�Ʈ 2] �⺻ WriteLock ����\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	Lock lock;
	int32_t initialLockedCount = tLockedCount;

	std::wcout << std::format(L"  �ʱ� tLockedCount: {}\n", initialLockedCount);
	std::wcout << std::format(L"  IsWriteLocked: {}\n", lock.IsWriteLocked() ? L"true" : L"false");

	{
		WriteLock writeLock(lock);
		std::wcout << std::format(L"  WriteLock ȹ�� �� tLockedCount: {}\n", tLockedCount);
		std::wcout << std::format(L"  IsWriteLocked: {}\n", lock.IsWriteLocked() ? L"true" : L"false");

		if (tLockedCount != initialLockedCount + 1)
		{
			std::wcout << L"  [FAIL] tLockedCount�� �������� ����\n";
		}
		else if (!lock.IsWriteLocked())
		{
			std::wcout << L"  [FAIL] ���� ȹ����� ����\n";
		}
		else
		{
			std::wcout << L"  [PASS] WriteLock ȹ�� ����\n";
		}
	}

	std::wcout << std::format(L"  WriteLock ���� �� tLockedCount: {}\n", tLockedCount);
	std::wcout << std::format(L"  IsWriteLocked: {}\n", lock.IsWriteLocked() ? L"true" : L"false");

	if (tLockedCount != initialLockedCount)
	{
		std::wcout << L"  [FAIL] tLockedCount�� �������� ����\n";
	}
	else if (lock.IsWriteLocked())
	{
		std::wcout << L"  [FAIL] ���� �������� ����\n";
	}
	else
	{
		std::wcout << L"  [PASS] WriteLock ���� ����\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 3: ReadLock 이동 생성자(transfer) 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestLock::TestCase_ReadLockCopyTransfer()
{
	std::wcout << L"[�׽�Ʈ 3] ReadLock �̵� �����ڸ� ���� �̰�\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	Lock lock;
	int32_t initialLockedCount = tLockedCount;

	std::wcout << std::format(L"  �ʱ� tLockedCount: {}\n", initialLockedCount);

	{
		ReadLock original(lock);
		std::wcout << std::format(L"  original ���� �� tLockedCount: {}\n", tLockedCount);
		std::wcout << std::format(L"  IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");

		{
			ReadLock copied(std::move(original));  // 이동 생성자 호출(transfer)
			std::wcout << std::format(L"  copied ���� �� tLockedCount: {}\n", tLockedCount);
			std::wcout << std::format(L"  IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");

			// 이동 이후에도 EnterReadLock 추가 호출이 없어야 한다. (tLockedCount는 +1 유지)
			if (tLockedCount != initialLockedCount + 1)
			{
				std::wcout << std::format(L"  [WARN] �̰� �� tLockedCount ����ġ: {} (����: {})\n", 
					tLockedCount, initialLockedCount + 1);
			}
			else
			{
				std::wcout << L"  [PASS] �̰� �� tLockedCount ����\n";
			}
		}

		std::wcout << std::format(L"  copied �ı� �� tLockedCount: {}\n", tLockedCount);
		std::wcout << std::format(L"  IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");

		// copied 소멸 시 LeaveReadLock 호출됨
		// original은 mLock==nullptr 이므로 소멸 시 아무 동작도 하지 않아야 한다.
	}

	std::wcout << std::format(L"  original �ı� �� tLockedCount: {}\n", tLockedCount);
	std::wcout << std::format(L"  IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");

	if (tLockedCount != initialLockedCount)
	{
		std::wcout << std::format(L"  [FAIL] ���� tLockedCount ����ġ: {} (����: {})\n", 
			tLockedCount, initialLockedCount);
	}
	else if (lock.IsReadLocked())
	{
		std::wcout << L"  [FAIL] ���� �������� ����\n";
	}
	else
	{
		std::wcout << L"  [PASS] �̰� �� ���� ����\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 4: WriteLock 이동 생성자(transfer) 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestLock::TestCase_WriteLockCopyTransfer()
{
	std::wcout << L"[�׽�Ʈ 4] WriteLock �̵� �����ڸ� ���� �̰�\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	Lock lock;
	int32_t initialLockedCount = tLockedCount;

	std::wcout << std::format(L"  �ʱ� tLockedCount: {}\n", initialLockedCount);

	{
		WriteLock original(lock);
		std::wcout << std::format(L"  original ���� �� tLockedCount: {}\n", tLockedCount);

		{
			WriteLock copied(std::move(original));  // �̵� ������ ȣ�� (�̰�)
			std::wcout << std::format(L"  copied ���� �� tLockedCount: {}\n", tLockedCount);

			if (tLockedCount != initialLockedCount + 1)
			{
				std::wcout << std::format(L"  [WARN] �̰� �� tLockedCount ����ġ: {} (����: {})\n", 
					tLockedCount, initialLockedCount + 1);
			}
			else
			{
				std::wcout << L"  [PASS] �̰� �� tLockedCount ����\n";
			}
		}

		std::wcout << std::format(L"  copied �ı� �� tLockedCount: {}\n", tLockedCount);
	}

	std::wcout << std::format(L"  original �ı� �� tLockedCount: {}\n", tLockedCount);

	if (tLockedCount != initialLockedCount)
	{
		std::wcout << std::format(L"  [FAIL] ���� tLockedCount ����ġ: {} (����: {})\n", 
			tLockedCount, initialLockedCount);
	}
	else if (lock.IsWriteLocked())
	{
		std::wcout << L"  [FAIL] ���� �������� ����\n";
	}
	else
	{
		std::wcout << L"  [PASS] �̰� �� ���� ����\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 5: 함수 반환을 통한 ReadLock 이동(transfer) 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 반환 값을 통해 이동 semantics를 검증하기 위한 헬퍼
ReadLock TestLock::AcquireReadLock(Lock& lock)
{
	std::wcout << std::format(L"    [�Լ� ����] tLockedCount: {}\n", tLockedCount);
	
	ReadLock localLock(lock);  // EnterReadLock ȣ��, tLockedCount +1
	std::wcout << std::format(L"    [Lock ���� ��] tLockedCount: {}, IsReadLocked: {}\n", 
		tLockedCount, lock.IsReadLocked() ? L"true" : L"false");
	
	std::wcout << L"    [return ����] �̵� ������ �Ǵ� RVO �߻� ����\n";
	return std::move(localLock);  // �̵� ������: localLock.mLock -> ��ȯ��.mLock, localLock.mLock = nullptr
	// 함수 종료 시 localLock 소멸자 호출(mLock==nullptr 이므로 LeaveReadLock이 호출되면 안 됨)
}

void TestLock::TestCase_ReadLockReturnTransfer()
{
	std::wcout << L"[�׽�Ʈ 5] �Լ� ��ȯ�� ���� ReadLock �̰� (�� ����)\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	Lock lock;
	int32_t initialLockedCount = tLockedCount;

	std::wcout << std::format(L"  [ȣ�� ��] tLockedCount: {}\n", initialLockedCount);

	{
		std::wcout << L"  --- AcquireReadLock ȣ�� ---\n";
		ReadLock readLock = AcquireReadLock(lock);
		std::wcout << L"  --- AcquireReadLock ��ȯ �Ϸ� ---\n";
		
		std::wcout << std::format(L"  [��ȯ ��] tLockedCount: {}\n", tLockedCount);
		std::wcout << std::format(L"  [��ȯ ��] IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");

		// 재검증: tLockedCount가 정확히 +1 인가?
		int32_t expectedCount = initialLockedCount + 1;
		if (tLockedCount != expectedCount)
		{
			std::wcout << std::format(L"  [FAIL] tLockedCount ����ġ! ����: {}, ����: {}\n", 
				tLockedCount, expectedCount);
			std::wcout << L"         �� ���� �� EnterReadLock�� �߰� ȣ��Ǿ��ų�, ������ �����\n";
		}
		else if (!lock.IsReadLocked())
		{
			std::wcout << L"  [FAIL] tLockedCount�� ������ ���� �� ��������!\n";
			std::wcout << L"         �� tLockedCount�� ���� �� ���°� ����ġ\n";
		}
		else
		{
			std::wcout << L"  [PASS] ��ȯ �̰� ���� - tLockedCount�� �� ���� ��ġ\n";
		}
	}

	std::wcout << std::format(L"  [������ ���� ��] tLockedCount: {}\n", tLockedCount);
	std::wcout << std::format(L"  [������ ���� ��] IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");

	// 정리 검증
	if (tLockedCount != initialLockedCount)
	{
		std::wcout << std::format(L"  [FAIL] ���� tLockedCount ����ġ: {} (����: {})\n", 
			tLockedCount, initialLockedCount);
		if (tLockedCount > initialLockedCount)
			std::wcout << L"         �� �� ������ ����� (LeaveReadLock ��ȣ��)\n";
		else
			std::wcout << L"         �� �� ������ ������ (LeaveReadLock �ߺ� ȣ��)\n";
	}
	else if (lock.IsReadLocked())
	{
		std::wcout << L"  [FAIL] tLockedCount�� ���������� ���� ������ ��������!\n";
	}
	else
	{
		std::wcout << L"  [PASS] ���� ���� ���� - tLockedCount ����, �� ������\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 6: 함수 반환을 통한 WriteLock 이동(transfer) 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WriteLock TestLock::AcquireWriteLock(Lock& lock)
{
	std::wcout << std::format(L"    [�Լ� ����] tLockedCount: {}\n", tLockedCount);
	
	WriteLock localLock(lock);  // EnterWriteLock ȣ��, tLockedCount +1
	std::wcout << std::format(L"    [Lock ���� ��] tLockedCount: {}, IsWriteLocked: {}\n", 
		tLockedCount, lock.IsWriteLocked() ? L"true" : L"false");
	
	std::wcout << L"    [return ����] �̵� ������ �Ǵ� RVO �߻� ����\n";
	return std::move(localLock);
}

void TestLock::TestCase_WriteLockReturnTransfer()
{
	std::wcout << L"[�׽�Ʈ 6] �Լ� ��ȯ�� ���� WriteLock �̰� (�� ����)\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	Lock lock;
	int32_t initialLockedCount = tLockedCount;

	std::wcout << std::format(L"  [ȣ�� ��] tLockedCount: {}\n", initialLockedCount);

	{
		std::wcout << L"  --- AcquireWriteLock ȣ�� ---\n";
		WriteLock writeLock = AcquireWriteLock(lock);
		std::wcout << L"  --- AcquireWriteLock ��ȯ �Ϸ� ---\n";
		
		std::wcout << std::format(L"  [��ȯ ��] tLockedCount: {}\n", tLockedCount);
		std::wcout << std::format(L"  [��ȯ ��] IsWriteLocked: {}\n", lock.IsWriteLocked() ? L"true" : L"false");

		int32_t expectedCount = initialLockedCount + 1;
		if (tLockedCount != expectedCount)
		{
			std::wcout << std::format(L"  [FAIL] tLockedCount ����ġ! ����: {}, ����: {}\n", 
				tLockedCount, expectedCount);
		}
		else if (!lock.IsWriteLocked())
		{
			std::wcout << L"  [FAIL] tLockedCount�� ������ ���� �� ��������!\n";
		}
		else
		{
			std::wcout << L"  [PASS] ��ȯ �̰� ����\n";
		}
	}

	std::wcout << std::format(L"  [������ ���� ��] tLockedCount: {}\n", tLockedCount);

	if (tLockedCount != initialLockedCount)
	{
		std::wcout << std::format(L"  [FAIL] ���� tLockedCount ����ġ: {} (����: {})\n", 
			tLockedCount, initialLockedCount);
	}
	else if (lock.IsWriteLocked())
	{
		std::wcout << L"  [FAIL] tLockedCount�� ���������� ���� ������ ��������!\n";
	}
	else
	{
		std::wcout << L"  [PASS] ���� ���� ����\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 6-2: RVO 우회(조건부 반환) 테스트
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestLock::TestCase_ExplicitCopyOnReturn()
{
	std::wcout << L"[�׽�Ʈ 6-2] RVO ��ȸ - ����� ���� �׽�Ʈ\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	Lock lock;
	int32_t initialLockedCount = tLockedCount;

	std::wcout << std::format(L"  [ȣ�� ��] tLockedCount: {}\n", initialLockedCount);

	// 조건 분기 반환으로 RVO가 항상 적용되지 않는 케이스를 만들고, 이동이 안전한지 확인한다.
	auto acquireWithCondition = [&lock](bool condition) -> ReadLock {
		std::wcout << std::format(L"    [���� ����] tLockedCount: {}\n", tLockedCount);
		
		ReadLock lock1(lock);
		std::wcout << std::format(L"    [lock1 ����] tLockedCount: {}\n", tLockedCount);
		
		if (condition)
		{
			std::wcout << L"    [���Ǻ� ��ȯ - lock1]\n";
			return std::move(lock1);  // �̵� ������ ȣ��
		}
		
		ReadLock lock2(lock);  // �� ��δ� ���� �� ��
		std::wcout << L"    [���Ǻ� ��ȯ - lock2]\n";
		return std::move(lock2);
	};

	{
		std::wcout << L"  --- ���Ǻ� ��ȯ ���� ȣ�� ---\n";
		ReadLock result = acquireWithCondition(true);
		std::wcout << L"  --- ���� ��ȯ �Ϸ� ---\n";
		
		std::wcout << std::format(L"  [��ȯ ��] tLockedCount: {}\n", tLockedCount);
		std::wcout << std::format(L"  [��ȯ ��] IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");

		int32_t expectedCount = initialLockedCount + 1;
		if (tLockedCount != expectedCount)
		{
			std::wcout << std::format(L"  [FAIL] tLockedCount ����ġ! ����: {}, ����: {}\n", 
				tLockedCount, expectedCount);
		}
		else
		{
			std::wcout << L"  [PASS] ���Ǻ� ��ȯ �̰� ����\n";
		}
	}

	std::wcout << std::format(L"  [������ ���� ��] tLockedCount: {}\n", tLockedCount);

	if (tLockedCount != initialLockedCount)
	{
		std::wcout << std::format(L"  [FAIL] ���� tLockedCount ����ġ: {} (����: {})\n", 
			tLockedCount, initialLockedCount);
	}
	else
	{
		std::wcout << L"  [PASS] ���� ���� ����\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 6-3: std::move 사용 시 이동 semantics 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestLock::TestCase_MoveSemantics()
{
	std::wcout << L"[�׽�Ʈ 6-3] std::move ��� �� ���� �׽�Ʈ\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	Lock lock;
	int32_t initialLockedCount = tLockedCount;

	std::wcout << std::format(L"  [�ʱ�] tLockedCount: {}\n", initialLockedCount);

	{
		ReadLock original(lock);
		std::wcout << std::format(L"  [original ����] tLockedCount: {}, IsReadLocked: {}\n", 
			tLockedCount, lock.IsReadLocked() ? L"true" : L"false");

		// std::move를 사용하면 이동 생성자가 호출되어 ownership이 moved로 이전된다.
		ReadLock moved(std::move(original));
		std::wcout << std::format(L"  [std::move ��] tLockedCount: {}, IsReadLocked: {}\n", 
			tLockedCount, lock.IsReadLocked() ? L"true" : L"false");

		// 기대 동작:
		// - original.mLock == nullptr
		// - moved가 유효한 락을 보유

		if (tLockedCount != initialLockedCount + 1)
		{
			std::wcout << std::format(L"  [WARN] tLockedCount ����ġ: {} (����: {})\n", 
				tLockedCount, initialLockedCount + 1);
		}
		else
		{
			std::wcout << L"  [PASS] std::move �� tLockedCount ����\n";
		}
	}

	std::wcout << std::format(L"  [������ ����] tLockedCount: {}, IsReadLocked: {}\n", 
		tLockedCount, lock.IsReadLocked() ? L"true" : L"false");

	if (tLockedCount != initialLockedCount)
	{
		std::wcout << std::format(L"  [FAIL] ���� tLockedCount ����ġ: {} (����: {})\n", 
			tLockedCount, initialLockedCount);
	}
	else
	{
		std::wcout << L"  [PASS] ���� ���� ����\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 7: 이중 해제 방지(설계) 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestLock::TestCase_DoubleReleaseCheck()
{
	std::wcout << L"[�׽�Ʈ 7] ���� ���� �ó����� �м�\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	std::wcout << L"  ���� ���� �м�:\n";
	std::wcout << L"    - ���� ������: original.mLock = nullptr�� ����\n";
	std::wcout << L"    - �Ҹ���: mLock != nullptr�� ���� LeaveXxxLock() ȣ��\n";
	std::wcout << L"    - ���� ���� ������ �߻����� ����\n";
	std::wcout << L"  [PASS] ���� ���� ���� ��Ŀ���� Ȯ��\n";

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 8: 미해제(누수) 방지(설계) 확인
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestLock::TestCase_NoReleaseCheck()
{
	std::wcout << L"[�׽�Ʈ 8] ������ �ó����� �м�\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	std::wcout << L"  ���� ���� �м�:\n";
	std::wcout << L"    - ���� �� original.mLock = nullptr\n";
	std::wcout << L"    - copied�� mLock�� ������\n";
	std::wcout << L"    - copied �Ҹ� �� LeaveXxxLock() ȣ��\n";
	std::wcout << L"    - original �Ҹ� �� mLock == nullptr�̹Ƿ� �ƹ��͵� �� ��\n";
	std::wcout << L"    - ���� ��Ȯ�� �� ���� ������\n";
	std::wcout << L"  [PASS] ���� ���� ���� Ȯ��\n";

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 9: tLockedCount 일관성 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestLock::TestCase_LockedCountConsistency()
{
	std::wcout << L"[�׽�Ʈ 9] tLockedCount �ϰ��� �׽�Ʈ\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	Lock lock;
	int32_t initialLockedCount = tLockedCount;

	std::wcout << std::format(L"  �ʱ� tLockedCount: {}\n", initialLockedCount);

	// 연속 이동 시나리오
	{
		ReadLock lock1(lock);
		std::wcout << std::format(L"  lock1 ���� ��: {}\n", tLockedCount);

		{
			ReadLock lock2(std::move(lock1));  // �̰�
			std::wcout << std::format(L"  lock2�� �̰� ��: {}\n", tLockedCount);

			{
				ReadLock lock3(std::move(lock2));  // �ٽ� �̰�
				std::wcout << std::format(L"  lock3�� �̰� ��: {}\n", tLockedCount);
			}  // lock3 �ı� - LeaveReadLock ȣ��
			std::wcout << std::format(L"  lock3 �ı� ��: {}\n", tLockedCount);
		}  // lock2 �ı� - mLock == nullptr�̹Ƿ� �ƹ��͵� �� ��
		std::wcout << std::format(L"  lock2 �ı� ��: {}\n", tLockedCount);
	}  // lock1 �ı� - mLock == nullptr�̹Ƿ� �ƹ��͵� �� ��
	std::wcout << std::format(L"  lock1 �ı� ��: {}\n", tLockedCount);

	if (tLockedCount != initialLockedCount)
	{
		std::wcout << std::format(L"  [FAIL] tLockedCount ����ġ: {} (����: {})\n", 
			tLockedCount, initialLockedCount);
	}
	else
	{
		std::wcout << L"  [PASS] tLockedCount �ϰ��� ����\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 10: structured binding 패턴 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::tuple<int, ReadLock> TestLock::FindDataWithReadLock(Lock& lock, int value)
{
	ReadLock readLock(lock);
	return std::make_tuple(value * 2, std::move(readLock));
}

std::tuple<int*, WriteLock> TestLock::FindDataWithWriteLock(Lock& lock, int& value)
{
	WriteLock writeLock(lock);
	return std::make_tuple(&value, std::move(writeLock));
}

void TestLock::TestCase_StructuredBindingPattern()
{
	std::wcout << L"[�׽�Ʈ 10] ������ ���ε� ���� �׽�Ʈ\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	Lock lock;
	int32_t initialLockedCount = tLockedCount;
	int testValue = 42;

	std::wcout << std::format(L"  �ʱ� tLockedCount: {}\n", initialLockedCount);

	// ReadLock: structured binding으로 이동
	{
		auto [result, readLock] = FindDataWithReadLock(lock, testValue);
		std::wcout << std::format(L"  ������ ���ε� �� tLockedCount: {}\n", tLockedCount);
		std::wcout << std::format(L"  IsReadLocked: {}\n", lock.IsReadLocked() ? L"true" : L"false");
		std::wcout << std::format(L"  result: {} (����: {})\n", result, testValue * 2);

		if (!lock.IsReadLocked())
		{
			std::wcout << L"  [FAIL] ReadLock�� �������� ����\n";
		}
		else
		{
			std::wcout << L"  [PASS] ReadLock ������ ���ε� �̰� ����\n";
		}
	}

	std::wcout << std::format(L"  ReadLock ������ ���� �� tLockedCount: {}\n", tLockedCount);

	// WriteLock: structured binding으로 이동
	{
		auto [ptr, writeLock] = FindDataWithWriteLock(lock, testValue);
		std::wcout << std::format(L"  WriteLock ������ ���ε� �� tLockedCount: {}\n", tLockedCount);
		std::wcout << std::format(L"  IsWriteLocked: {}\n", lock.IsWriteLocked() ? L"true" : L"false");

		if (!lock.IsWriteLocked())
		{
			std::wcout << L"  [FAIL] WriteLock�� �������� ����\n";
		}
		else
		{
			*ptr = 100;  // �� ��ȣ �Ͽ� �� ����
			std::wcout << std::format(L"  �� ���� ��: {}\n", testValue);
			std::wcout << L"  [PASS] WriteLock ������ ���ε� �̰� ����\n";
		}
	}

	std::wcout << std::format(L"  WriteLock ������ ���� �� tLockedCount: {}\n", tLockedCount);

	if (tLockedCount != initialLockedCount)
	{
		std::wcout << std::format(L"  [FAIL] ���� tLockedCount ����ġ: {} (����: {})\n", 
			tLockedCount, initialLockedCount);
	}
	else
	{
		std::wcout << L"  [PASS] ��� �� ���� ����\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 11: 멀티스레드 이동(필요 시 활성화)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestLock::TestCase_MultithreadLockTransfer()
{
	std::wcout << L"[�׽�Ʈ 11] ��Ƽ������ �� �̰� �м�\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	std::wcout << L"  ���ǻ���:\n";
	std::wcout << L"    - tLockedCount�� thread_local ����\n";
	std::wcout << L"    - ���� �ٸ� ������� �̰��ϸ� tLockedCount�� ����\n";
	std::wcout << L"    - ������ A���� +1, ������ B���� -1 �� ���� �ұ���\n";
	std::wcout << L"    - �� ��ü�� ������ �� �̰��ϸ� �� ��!\n";
	std::wcout << L"  [INFO] ���� ������ �������� �̰� ����\n";

	std::wcout << L"\n";
}

