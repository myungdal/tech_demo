// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


// forward declaration (SocketPtr.h include → .cpp로 이동)
template<typename T> class SocketPtr;
class SocketGameFromFront;
class SocketGameToDb;

class GamePacketHandlerStaticData final
{
public:
	static constexpr ClockMs RETRY_DELAY = 1000ms;

public:
	void SendReqStaticDataChecksum_timer(SocketPtr<SocketGameToDb>& socket);
	void SendReqStaticData(SocketGameToDb& socket);

public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketGameToDb& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(class DG_ACK_STATIC_DATA_CHECKSUM& rp, MAYBE_UNUSED SocketGameToDb& socket);
	HandleResult OnPacket(class DG_ACK_STATIC_DATA& rp, MAYBE_UNUSED SocketGameToDb& socket);
};

inline std::shared_ptr<GamePacketHandlerStaticData> gGamePacketHandlerStaticData = nullptr;
