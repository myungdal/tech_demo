// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;

#include "DbServer/PacketHandler/DbPacketHandlerStaticData/Transactor_GD_REQ_STATIC_DATA.h"


Transactor_GD_REQ_STATIC_DATA::Transactor_GD_REQ_STATIC_DATA(
	SocketDbFromGame& gameSocket,
	GD_REQ_STATIC_DATA& rp
)
	:
	mGameSocket(gameSocket),
	mRp(rp)
{
}
Result Transactor_GD_REQ_STATIC_DATA::OnUpdate()
{
	if (!gStaticDataCatalog->IsSynced())
		return Result::RETRY_LATER;
	mStaticDataIndex = mRp.Get_index();
	if (gStaticDataCatalog->GetMySizeList().size() <= _IDX(mStaticDataIndex))
		return Result::WRONG_APP_VERSION;
	return Result::SUCCEEDED;
}
void Transactor_GD_REQ_STATIC_DATA::OnFinish()
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
			SocketUtil::Send<DG_ACK_STATIC_DATA::Writer> wp(mGameSocket, ACK, mRp, GetResult());
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

void Transactor_GD_REQ_STATIC_DATA::OnError()
{
	SocketUtil::Send<DG_ACK_STATIC_DATA::Writer> wp(mGameSocket, ACK, mRp, GetResult());
	wp.SetValues(
		mRp.Get_index(),
		1,
		0,
		nullptr, 0
	);
}

void Transactor_GD_REQ_STATIC_DATA::OnLog()
{
}
