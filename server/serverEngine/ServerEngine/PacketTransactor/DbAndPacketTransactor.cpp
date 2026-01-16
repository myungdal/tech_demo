// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ServerEngine/PCH/pch_serverEngine.h"

import ServerEngineDb;

#include "DbAndPacketTransactor.h"


DbAndPacketTransactor::DbAndPacketTransactor(PacketHeader& packetHeader)
	:
	mMainDbSession(std::make_shared<MainDbSession>(CommitType::MANUAL)),
	mUserDbSession(std::make_shared<UserDbSession>(CommitType::MANUAL, packetHeader.GetDbShardIdx())),
	mCurrPacketHeader(packetHeader)
{
}
bool DbAndPacketTransactor::OnCommit()
{
	bool result0 = mUserDbSession->Commit();
	bool result1 = mMainDbSession->Commit();

	return result0 && result1;
}

bool DbAndPacketTransactor::OnRollback()
{
	bool result0 = mUserDbSession->Rollback();
	bool result1 = mMainDbSession->Rollback();

	return result0 && result1;
}
