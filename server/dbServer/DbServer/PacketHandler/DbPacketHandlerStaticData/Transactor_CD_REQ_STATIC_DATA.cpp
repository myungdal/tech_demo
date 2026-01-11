// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;

#include "Transactor_CD_REQ_STATIC_DATA.h"


Transactor_CD_REQ_STATIC_DATA::Transactor_CD_REQ_STATIC_DATA(
	DbUser& dbUser, DbUserContext& userContext,
	SocketDbFromFront& frontSocket,
	CD_REQ_STATIC_DATA& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_CD_REQ_STATIC_DATA::OnUpdate()
{
	if (!gStaticDataCatalog->IsSynced())
		return Result::RETRY_LATER;
	mStaticDataIndex = mRp.Get_index();
	if (gStaticDataCatalog->GetMySizeList().size() <= _IDX(mStaticDataIndex))
		return Result::WRONG_APP_VERSION;
	return Result::SUCCEEDED;
}
void Transactor_CD_REQ_STATIC_DATA::OnFinish()
{
	StaticDataSize offset = 0;
	StaticDataSize size = gStaticDataCatalog->GetMySizeList().at(mStaticDataIndex);
	uint8_t* bin = gStaticDataCatalog->BinaryList().at(mStaticDataIndex);	
	while (0 < size)
	{
		const uint8_t isCompleted = (MAX_PACKET_SAFE_DATA_SIZE >= size) ? 1 : 0;
		const StaticDataSize thisDataSize = (1 == isCompleted) ? size : MAX_PACKET_SAFE_DATA_SIZE;

		// 정적데이터 성공 응답.
		{
			DbSocketUtil::SendToClient<DC_ACK_STATIC_DATA::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
			wp.GetHeader().SetDbSeq(AddDbSeq());
			wp.SetValues(
				mStaticDataIndex,
				isCompleted,
				offset,
				bin + offset, static_cast<PacketSize>(thisDataSize)
			);
		}

		size -= thisDataSize;
		offset += thisDataSize;
	}
}

void Transactor_CD_REQ_STATIC_DATA::OnError()
{
	DbSocketUtil::SendToClient<DC_ACK_STATIC_DATA::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.SetValues(
		mRp.Get_index(),
		1,
		0,
		nullptr, 0
	);
}

void Transactor_CD_REQ_STATIC_DATA::OnLog()
{

}
