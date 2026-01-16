// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEngineCommon;


DbUserSocketState::DbUserSocketState()
{
	const DbSeq dbSeq = (*tRandomEngine64)();

	mCurrPacketHeader.SetDbSeq(dbSeq);
}

void DbUserSocketState::SetSocket(SocketDbFromFront* socket)
{
	mSocketFront.Set(socket, L"DbUserSocketState");
}

SocketPtr<SocketDbFromFront> DbUserSocketState::GetSocket() const
{
	return mSocketFront.Lock();
}

void DbUserSocketState::SetDbShardIdx(DbShardIdx shardIdx)
{
	mCurrPacketHeader.SetDbShardIdx(shardIdx);
}

bool DbUserSocketState::ValidateDbSeq(const PacketHeader& packetHeader) const
{
	const bool validateDbShardIdx = (mCurrPacketHeader.GetDbShardIdx() == packetHeader.GetDbShardIdx());
	const bool validateDbSeq = (mCurrPacketHeader.GetDbSeq() == packetHeader.GetDbSeq());
	return (validateDbShardIdx && validateDbSeq);
}

DbSeq DbUserSocketState::AddDbSeq()
{ 
	DbSeq dbSeq = mCurrPacketHeader.GetDbSeq();
	++dbSeq;
	if (0 == dbSeq)
		++dbSeq;

	mCurrPacketHeader.SetDbSeq(dbSeq);

	return dbSeq;
}

bool DbUserSocketState::ValidateState(const PacketHeader& packetHeader) const
{
	if (false == mCurrPacketHeader.ValidateState(packetHeader))
	{
		_DEBUG_RED;
		return false;
	}

	return true;
}
