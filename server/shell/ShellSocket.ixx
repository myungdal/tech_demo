// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ShellSocket.ixx - Shell 소켓 모듈 인터페이스
module;

#pragma warning(disable: 5028)

// GMF - ServerEngine PCH 필요 (Lock, ThreadId, CompletionHandler 등)
#include "Shell/PCH/pch.h"

#include "Shell/Socket/SocketShellToMain.h"
#include "Shell/PacketHandler/ShellPacketHandler/ShellPacketHandler.h"

export module ShellSocket;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
export import ServerEngineCore;
export import ServerEngineNetwork;
export import ServerEngineUtil;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
export
{
	using ::SocketShellToMain;
	using ::gSocketShellToMain;
	using ::ShellPacketHandler;
	using ::gShellPacketHandler;
}
