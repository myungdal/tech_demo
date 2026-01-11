// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/PacketFuncs/PacketLogger.h"

#include "Packet/NetworkPacket/NetworkPacket.h"


PacketLogger::PacketLogType PacketLogger::GetPacketLogType(PacketType packetType)
{
	switch (packetType)
	{	
	case PacketTypes::FD_REQ_CD_BYPASS:
	case PacketTypes::DF_ACK_DC_BYPASS:
	case PacketTypes::FG_CG_BYPASS:
	case PacketTypes::GF_GC_BYPASS:
	case PacketTypes::FB_REQ_CB_BYPASS:
	case PacketTypes::BF_ACK_BC_BYPASS:
	case PacketTypes::FM_REQ_CM_BYPASS:
	case PacketTypes::MF_ACK_MC_BYPASS:
	case PacketTypes::FC_ACK_PACKET_LIST:
		return PacketLogger::PacketLogType::LOGGING_PACKET_TYPE_ONLY;
	case PacketTypes::GF_GC_BYPASS_GAME:
	case PacketTypes::GC_GAME_UPDATE:
		return PacketLogger::PacketLogType::LOGGING_NONE;
	default:
		break;
	}
	return PacketLogger::PacketLogType::LOGGING_PACKET_TYPE_AND_ARGS;
}
