// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// NetworkTypes.h - 네트워크 공통 타입/전역
// (모듈에서는 ServerEngineNetwork에서 export됨)

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Network Types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// IOCP 완료 이벤트 타입
enum class IoType : uint8_t
{
	NONE = 0,
	ACCEPT,
	CONNECT,
	DISCONNECT,
	SEND,
	PRE_RECV,
	RECV,
	MAX
};

// IOCP Overlapped 구조체 (풀에서 관리)
struct Overlapped : OVERLAPPED
{
	IoType mIoType = IoType::NONE;		// IO 작업 타입
	WSABUF mWsaBuf{ 0, nullptr };		// 전송/수신 버퍼

	// Overlapped 객체 획득(풀에서 pop)
	static Overlapped* Acquire(IoType ioType, CHAR* buf, ULONG len);

	// Overlapped 객체 반납(풀에 push)
	void Dispose();
};

// IOCP 완료 핸들러 인터페이스
class CompletionHandler
{
public:
	virtual ~CompletionHandler() = default;

	// IO 완료 시 호출되는 콜백
	virtual void OnCompleted(Overlapped& overlapped, DWORD bytes, bool error) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Network Globals
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Winsock ConnectEx 함수 포인터
inline LPFN_CONNECTEX ConnectEx = nullptr;

// Winsock DisconnectEx 함수 포인터
inline LPFN_DISCONNECTEX DisconnectEx = nullptr;

