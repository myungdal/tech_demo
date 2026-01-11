// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;

#include "UserAndPacketTransactor.h"


UserAndPacketTransactor::UserAndPacketTransactor(
	DbUser& dbUser, DbUserContext& userContext, PacketHeader& packetHeader
)
	:
	DbAndPacketTransactor(packetHeader),
	mDbUser(dbUser),
	mUserContext(std::make_shared<DbUserContext>(dbUser.GetContext())),
	mUserSocketState(userContext.GetUserSocketState()),
	mUserCacheAccessor(userContext.GetUserCache()),
	mUserCacheDiff(userContext.GetUserCacheDiff()),
	mUserCacheDiffWp(PARAM, TEMP_BUF)
{
}
UserId UserAndPacketTransactor::GetUserId() const
{
	return mDbUser.GetUserId();
}
DbSeq UserAndPacketTransactor::AddDbSeq()
{
	return mUserSocketState.AddDbSeq();
}
Result UserAndPacketTransactor::OnPreValidate()
{
	// DbSeq 검증
	if (false == mUserSocketState.ValidateDbSeq(GetCurrPacketHeader()))
		return Result::DB_SEQ_ERROR;

	// 상태 검증
	if (false == mUserSocketState.ValidateState(GetCurrPacketHeader()))
	{
		//	my.mAppIds != other.mAppIds
		//	my.mDbShardIdx != other.mDbShardIdx
		//	my.mAccountId != other.mAccountId
		//	my.mServerId != other.mServerId
		//	my.mUserId != other.mUserId
		//	my.mGuildId != other.mGuildId
		//	my.mGameId != other.mGameId
		//	my.mGameChannelIndex != other.mGameChannelIndex
		// 경우에 관한 에러 로그 처리

		PacketHeader& my = mUserSocketState.GetCurrPacketHeader();
		PacketHeader& other = GetCurrPacketHeader();

		size_t appType = static_cast<size_t>(PacketHeader::AppType_appIds::DB_SERVER);
		if (my.mAppIds[appType] != other.mAppIds[appType])
			_DEBUG_LOG(RED, L"mAppIds, my: {}, other: {}", my.mAppIds[appType], other.mAppIds[appType]);

		appType = static_cast<size_t>(PacketHeader::AppType_appIds::FRONT_SERVER);
		if (my.mAppIds[appType] != other.mAppIds[appType])
			_DEBUG_LOG(RED, L"mAppIds, my: {}, other: {}", my.mAppIds[appType], other.mAppIds[appType]);

		appType = static_cast<size_t>(PacketHeader::AppType_appIds::GAME_SERVER);
		if (my.mAppIds[appType] != other.mAppIds[appType])
			_DEBUG_LOG(RED, L"mAppIds, my: {}, other: {}", my.mAppIds[appType], other.mAppIds[appType]);

		if (my.mDbShardIdx != other.mDbShardIdx)
			_DEBUG_LOG(RED, L"mDbShardIdx, my: {}, other: {}", my.mDbShardIdx, other.mDbShardIdx);

		if (my.mAccountId != other.mAccountId)
			_DEBUG_LOG(RED, L"mAccountId, my: {}, other: {}", my.mAccountId, other.mAccountId);

		if (my.mServerId != other.mServerId)
			_DEBUG_LOG(RED, L"mServerId, my: {}, other: {}", my.mServerId, other.mServerId);

		if (my.mUserId != other.mUserId)
			_DEBUG_LOG(RED, L"mUserId, my: {}, other: {}", my.mUserId, other.mUserId);

		if (my.mGuildId != other.mGuildId)
			_DEBUG_LOG(RED, L"mGuildId, my: {}, other: {}", my.mGuildId, other.mGuildId);

		if (my.mGameId != other.mGameId)
			_DEBUG_LOG(RED, L"mGameId, my: {}, other: {}", my.mGameId, other.mGameId);

		return Result::USER_STATUS_ERROR;
	}

	return Result::SUCCEEDED;
}

void UserAndPacketTransactor::OnPreUpdate()
{
	if (HasError())
		return;

	mUserCacheDiff.Checkout();
}

bool UserAndPacketTransactor::OnCommit()
{
	// 차이 취합.
	mUserCacheDiff.Commit(OUT mUserCacheDiffWp);

	// 아이템 삭제가 발생했을 수 있기 때문에 trim 실행.
	ItemUtil::TrimInventory(GetUserDbSession(), mUserCacheAccessor);

	// 패킷 헤더 갱신
	mUserSocketState.SetCurrPacketHeader(GetCurrPacketHeader());

	return __super::OnCommit();
}

bool UserAndPacketTransactor::OnRollback()
{
	mUserCacheDiff.Rollback();

	return __super::OnRollback();
}

