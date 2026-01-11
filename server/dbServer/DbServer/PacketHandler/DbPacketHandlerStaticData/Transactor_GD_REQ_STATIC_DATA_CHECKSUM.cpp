// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;

#include "DbServer/PacketHandler/DbPacketHandlerStaticData/Transactor_GD_REQ_STATIC_DATA_CHECKSUM.h"


Transactor_GD_REQ_STATIC_DATA_CHECKSUM::Transactor_GD_REQ_STATIC_DATA_CHECKSUM(
	SocketDbFromGame& gameSocket,
	GD_REQ_STATIC_DATA_CHECKSUM& rp
)
	:
	mGameSocket(gameSocket),
	mRp(rp)
{
}
Result Transactor_GD_REQ_STATIC_DATA_CHECKSUM::OnUpdate()
{
	if (!gStaticDataCatalog->IsSynced())
		return Result::RETRY_LATER;
	return Result::SUCCEEDED;	
}
void Transactor_GD_REQ_STATIC_DATA_CHECKSUM::OnFinish()
{
	SocketUtil::Send<DG_ACK_STATIC_DATA_CHECKSUM::Writer> wp(mGameSocket, ACK, mRp, GetResult());
	wp.SetValues(
		gStaticDataCatalog->GetMyChecksumForGameServer(),
		gStaticDataCatalog->GetMySizeList().data(), static_cast<PacketSize>(gStaticDataCatalog->GetMySizeList().size()),
		gStaticDataCatalog->GetMyChecksumList().data(), static_cast<PacketSize>(gStaticDataCatalog->GetMyChecksumList().size())
	);
}

void Transactor_GD_REQ_STATIC_DATA_CHECKSUM::OnError()
{
	SocketUtil::Send<DG_ACK_STATIC_DATA_CHECKSUM::Writer> wp(mGameSocket, ACK, mRp, GetResult());
	wp.SetValues(
		{},
		nullptr, 0,
		nullptr, 0
	);
}

void Transactor_GD_REQ_STATIC_DATA_CHECKSUM::OnLog()
{

}
