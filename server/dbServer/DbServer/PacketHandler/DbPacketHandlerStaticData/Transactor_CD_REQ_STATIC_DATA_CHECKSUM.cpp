// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;

#include "Transactor_CD_REQ_STATIC_DATA_CHECKSUM.h"


Transactor_CD_REQ_STATIC_DATA_CHECKSUM::Transactor_CD_REQ_STATIC_DATA_CHECKSUM(
	DbUser& dbUser, DbUserContext& userContext,
	SocketDbFromFront& frontSocket,
	CD_REQ_STATIC_DATA_CHECKSUM& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_CD_REQ_STATIC_DATA_CHECKSUM::OnUpdate()
{
	if (!gStaticDataCatalog->IsSynced())
		return Result::RETRY_LATER;
	return Result::SUCCEEDED;	
}
void Transactor_CD_REQ_STATIC_DATA_CHECKSUM::OnFinish()
{
	DbSocketUtil::SendToClient<DC_ACK_STATIC_DATA_CHECKSUM::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.GetHeader().SetDbSeq(AddDbSeq());
	wp.SetValues(
		gStaticDataCatalog->GetMyChecksumForClient(),
		gStaticDataCatalog->GetMySizeList().data(), static_cast<PacketSize>(gStaticDataCatalog->GetMySizeList().size()),
		gStaticDataCatalog->GetMyChecksumList().data(), static_cast<PacketSize>(gStaticDataCatalog->GetMyChecksumList().size())
	);
}

void Transactor_CD_REQ_STATIC_DATA_CHECKSUM::OnError()
{
	DbSocketUtil::SendToClient<DC_ACK_STATIC_DATA_CHECKSUM::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.SetValues(
		{},
		nullptr, 0,
		nullptr, 0
	);
}

void Transactor_CD_REQ_STATIC_DATA_CHECKSUM::OnLog()
{

}
