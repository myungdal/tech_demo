// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/NetworkPacket/PacketHeader.h"

#include "ServerEngine/Socket/SocketPtr/SocketWeakPtr.h"


class SocketDbFromFront;
class SocketBase;

class DbUserSocketState
{
private:	
	mutable SocketWeakPtr<SocketDbFromFront> mSocketFront;

	PacketHeader mCurrPacketHeader;

	AuthTicket mAuthTicket = INVALID_AUTH_TICKET;

public:
	DbUserSocketState();

public:
	void SetSocket(SocketDbFromFront* socket);
	SocketPtr<SocketDbFromFront> GetSocket() const;

public:
	void SetCurrPacketHeader(const PacketHeader& info) { mCurrPacketHeader = info; }
	PacketHeader& GetCurrPacketHeader() { return mCurrPacketHeader; }

public:
	void SetDbShardIdx(DbShardIdx shardIdx);
	bool ValidateDbSeq(const PacketHeader& packetHeader) const;	
	DbSeq AddDbSeq();

	bool ValidateState(const PacketHeader& packetHeader) const;

public:
	void SetAuthTicket(AuthTicket authTicket) { mAuthTicket = authTicket; }
	AuthTicket GetAuthTicket() const noexcept { return mAuthTicket; }
};
