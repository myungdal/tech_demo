// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "NetworkManager.h"

#include "Common/MemPool/FixedPool.h"

import ServerEngineNetwork;
import ServerEngineUtil;


namespace
{
	using OverlappedPool = FixedPool<Overlapped, 12>;
	inline OverlappedPool& GetOverlappedPool()
	{
		static OverlappedPool pool{};
		return pool;
	}
}

Overlapped* Overlapped::Acquire(IoType ioType, CHAR* buf, ULONG len)
{
	Overlapped* overlapped = GetOverlappedPool().Pop();
	memset(overlapped, 0, sizeof(Overlapped));
	overlapped->mIoType = ioType;
	overlapped->mWsaBuf.buf = buf;
	overlapped->mWsaBuf.len = len;
	return overlapped;
}

void Overlapped::Dispose()
{
	GetOverlappedPool().Push(this);
}

NetworkManager::NetworkManager()
{
	static std::atomic<bool> isDone = false;
	static std::atomic<bool> isDoing = false;

	while (true)
	{
		// memory_order(acquire): isDone==true를 본 뒤에는 초기화 쓰기(WSAStartup/함수포인터 조회 등)가
		// 현재 스레드에서도 관측되도록(완료 플래그와 초기화 사이의 happens-before 보장).
		if (true == isDone.load(std::memory_order_acquire))
			break;

		// memory_order(acq_rel): 단 한 스레드만 초기화 블록에 진입시키는 "1회 초기화 게이트".
		// - acquire: 다른 스레드의 완료(release)와 짝지어 이후 코드가 앞당겨 실행되지 않도록
		// - release: isDoing=true 공개 자체는 atomic이지만, 여기서는 보수적으로 RMW에 release까지 포함
		if (false == isDoing.exchange(true, std::memory_order_acq_rel))
		{
			WSADATA wsaData;
			const int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

			if (startupResult != 0)
			{
				_ASSERT_CRASH(false);

				return;
			}

			{
				const SOCKET tempSocket = NetworkUtil::CreateSocket();

				GUID guid = WSAID_CONNECTEX;
				DWORD bytes = 0;

				const int result = WSAIoctl(
					tempSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &ConnectEx, sizeof(ConnectEx), &bytes, nullptr, nullptr
				);

				closesocket(tempSocket);

				if (SOCKET_ERROR == result)
				{
					_ASSERT_CRASH(false);

					return;
				}
			}

			{
				const SOCKET tempSocket = NetworkUtil::CreateSocket();
				GUID guid = WSAID_DISCONNECTEX;
				DWORD bytes = 0;

				const int result = WSAIoctl(
					tempSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &DisconnectEx, sizeof(DisconnectEx), &bytes, nullptr, nullptr
				);

				closesocket(tempSocket);

				if (SOCKET_ERROR == result)
				{
					_ASSERT_CRASH(false);

					return;
				}
			}

			// memory_order(release): 초기화가 끝난 뒤 완료 플래그를 publish.
			// 다른 스레드가 acquire-load로 isDone을 관측하면 초기화 결과를 안전하게 사용 가능.
			isDone.store(true, std::memory_order_release);

			break;
		}

		std::this_thread::sleep_for(1ms);
	}
}

NetworkManager::~NetworkManager()
{
	WSACleanup();
}

bool NetworkManager::HasGlobalNow() const
{
	return (INVALID_CLOCK_MS == mGlobalLocalTimeDelta.load()) ? false : true;
}

ClockPoint NetworkManager::GetGlobalNow() const
{
	return tClock.GetLocalNow() + mGlobalLocalTimeDelta.load();
}

void NetworkManager::SetupGlobalNow(ClockMs globalNow)
{
	const ClockMs& now = tClock.GetLocalNowMs();
	mGlobalLocalTimeDelta = (globalNow - now); // 마이너스도 허용

	if (0ms == mGlobalLocalTimeDelta.load())
		mGlobalLocalTimeDelta = 1ms;
}
