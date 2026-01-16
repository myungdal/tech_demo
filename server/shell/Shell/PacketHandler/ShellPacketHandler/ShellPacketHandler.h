// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

class SocketShellToMain;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Shell 서버 패킷 핸들러
// Main 서버로부터 수신된 패킷을 처리한다.
class ShellPacketHandler final
{
public:
	// 패킷 디스패치 (현재 처리하는 패킷 없음)
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketShellToMain& socket) { return HandleResult::NOT_EXISTS; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::shared_ptr<ShellPacketHandler> gShellPacketHandler = nullptr;
