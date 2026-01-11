// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Iocp.h"


Iocp::Iocp()
{
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
}
Iocp::~Iocp()
{
	CloseHandle(mIocpHandle);
}
void Iocp::BindCompletionHandle(SOCKET socket, CompletionHandler* completionHandler) const
{
	MAYBE_UNUSED HANDLE handle = CreateIoCompletionPort(
		reinterpret_cast<HANDLE>(socket), 
		mIocpHandle, 
		reinterpret_cast<ULONG_PTR>(completionHandler), 
		0
	);

	if (!handle)
	{
		// 이미 등록된 소켓이라서 나타나는 에러일 것.
		const int errorCode = WSAGetLastError();
		_DEBUG_LOG(RED, L"BindCompletionHandle, {}", errorCode);
	}
}

void Iocp::DispatchCompletion(DWORD timeout) const
{
	DWORD bytes = 0;
	PULONG_PTR* key = nullptr;
	Overlapped* overlapped = nullptr;

	const bool result = GetQueuedCompletionStatus(mIocpHandle, &bytes, (PULONG_PTR)&key, (LPOVERLAPPED*)&overlapped, timeout);

	if (true == result)
	{
		CompletionHandler* handler = reinterpret_cast<CompletionHandler*>(key);
		handler->OnCompleted(*overlapped, bytes, false);

		overlapped->Dispose();

		return;
	}

	if (nullptr == overlapped)
		return;

	const int errorCode = WSAGetLastError();
	
	// [FIX] 모든 에러에서 OnCompleted를 호출하여 참조 카운트가 해제되도록 보장
	// 그렇지 않으면 AddRef된 SEND/RECV가 영구히 해제되지 않아 리소스 누수 발생
	CompletionHandler* handler = reinterpret_cast<CompletionHandler*>(key);
	
	switch (errorCode)
	{
	case ERROR_SEM_TIMEOUT:
	case ERROR_NETNAME_DELETED: // 언리얼 에디터 중지할 때에는 아무 반응이 없고, 종료할 때 나타남.
	case ERROR_CONNECTION_REFUSED:
	case ERROR_INVALID_NETNAME:
	case ERROR_OPERATION_ABORTED: // WSA_OPERATION_ABORTED
	case ERROR_CONNECTION_ABORTED:
	case ERROR_NETWORK_UNREACHABLE:
	case WSAECONNRESET:    // [FIX] 일반적인 연결 리셋 (10054)
	case WSAESHUTDOWN:     // [FIX] 소켓 종료됨 (10058)
	case WSAECONNABORTED:  // [FIX] 연결 중단 (10053)
	{
		handler->OnCompleted(*overlapped, bytes, true);
		break;
	}
	default:
	{
		_DEBUG_LOG(RED, L"Iocp::DispatchCompletion unknown errorCode: {}", errorCode);
		// [FIX] 알 수 없는 에러도 OnCompleted를 호출하여 참조 해제 보장
		handler->OnCompleted(*overlapped, bytes, true);
		break;
	}
	}

	overlapped->Dispose();
}
