// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketGameToMain;

class GamePacketHandlerSystem final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketGameToMain& socket) { return HandleResult::NOT_EXISTS; }
};

inline std::shared_ptr<GamePacketHandlerSystem> gGamePacketHandlerSystem = nullptr;
