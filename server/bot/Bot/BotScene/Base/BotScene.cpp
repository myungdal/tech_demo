// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;
import ServerEnginePacket;


BotScene::BotScene(BotPtr& bot) 
	: 
	mBot(bot)
{	
}
void BotScene::EndSeq()
{
	if (INVALID_BOT_SCENE_SEQ != mSeq)
		ReportBotEvent(BotEventType::SEQ_FINISHED);

	mSeq = static_cast<BotSceneSeq>(mSeqHandlerList.size());

	_ASSERT_CRASH(false == mIsOnTimer);

	mIsOnTimer = true;

	WorkerManager::RunOnTimer(GetBotTaskDelay(), mBot, this, &BotScene::NextSeq_timer_async);
}

void BotScene::NextSeq()
{
	if (INVALID_BOT_SCENE_SEQ != mSeq)
		ReportBotEvent(BotEventType::SEQ_FINISHED);

	_ASSERT_CRASH(false == mIsOnTimer);

	mIsOnTimer = true;

	WorkerManager::RunOnTimer(GetBotTaskDelay(), mBot, this, &BotScene::NextSeq_timer_async);
}

void BotScene::NextSeq_timer_async()
{
	_ASSERT_CRASH(true == mIsOnTimer);

	mIsOnTimer = false;

	_ASSERT_CRASH(tWorkingWorker == mBot);

	if (true == CheckReconnectScene())
		return;

	++mSeq;

	if (mSeqHandlerList.size() <= mSeq)
	{
		mBot->NextScene_async();
		return;
	}

	ReportBotEvent(BotEventType::SEQ_STARTED);

	const SeqHandler seqHandler = mSeqHandlerList[mSeq];

	seqHandler->Invoke();
}

void BotScene::RetrySeq()
{
	ReportBotEvent(BotEventType::SEQ_RETRIED);

	_ASSERT_CRASH(false == mIsOnTimer);

	mIsOnTimer = true;

	WorkerManager::RunOnTimer(RETRY_DELAY, mBot, this, &BotScene::RetrySeq_timer_async);
}

void BotScene::RetrySeq_timer_async()
{
	_ASSERT_CRASH(true == mIsOnTimer);

	mIsOnTimer = false;

	_ASSERT_CRASH(tWorkingWorker == mBot);

	if (true == CheckReconnectScene())
		return;

	if (mSeqHandlerList.size() <= mSeq)
	{
		_ASSERT_CRASH(false);
		return;
	}

	const SeqHandler seqHandler = mSeqHandlerList[mSeq];

	seqHandler->Invoke();
}

void BotScene::TryChangeToReconnectScene()
{
	if (INVALID_AUTH_TICKET == GetBotContext().AuthTicket())
		return;

	if (false == mIsOnTimer)
		ChangeToReconnectScene();
	else
		mReservedReconnectScene = true;
}

bool BotScene::CheckReconnectScene() const
{
	if (mReservedReconnectScene)
	{
		ChangeToReconnectScene();
		return true;
	}

	return false;
}

void BotScene::ChangeToReconnectScene() const
{
	mBot->ChangeToReconnectScene_async();
}

void BotScene::ConnectToFront() const
{
	mBot->ConnectToFront();
}

bool BotScene::IsConnectedWithFront() const
{
	return mBot->IsConnectedWithFront();
}

void BotScene::SetEnableReconnect(bool enable) const
{
	mBot->SetEnableReconnect(enable);
}

void BotScene::DisconnectFromFront() const
{
	mBot->DisconnectFromFront();
}

void BotScene::ReportResultAndYield(Result result) const
{
	_YIELD_APP;
	return mBot->ReportResult(result);
}

void BotScene::ReportBotEvent(BotEventType botEventType)
{
	WorkerManager::Run(
		gBotMonitor, gBotMonitor.get(), &BotMonitor::OnBotEvent_async,
		botEventType, GetBotId(), GetSceneName(), mSeq, tClock.GetGlobalNow()
	);
}

void BotScene::ReportSend(PacketType packetType) const
{
	mBot->ReportSend(packetType);
}

BotContext& BotScene::GetBotContext() const
{
	return mBot->GetBotContext();
}

ClockMs BotScene::GetBotTaskDelay() const
{
	return mBot->GetBotTaskDelay();
}

BotId BotScene::GetBotId() const
{
	return mBot->GetBotId();
}

const std::wstring& BotScene::GetBotName() const
{
	return mBot->GetBotName();
}

const std::wstring& BotScene::GetSceneName() const
{
	return mBot->GetSceneName();
}

SOCKET BotScene::GetRawSocket()
{
	return mBot->GetRawSocket();
}

SocketPtr<SocketBotToFront> BotScene::GetSocket()
{
	return mBot->GetSocket();
}

HandleResult BotScene::OnPacket(MAYBE_UNUSED NetworkPacket& rp) const
{
	_DEBUG_RED;
	return HandleResult::OK;
}

void BotScene::CacheUserData(const USER_DATA& rp)
{
	UserCacheAccessor& userCache = GetBotContext().GetUserCache();
	
	// 업적
	if (UserAchievementTablePtr table = userCache.Get<UserAchievementTable>())
	{
		table->Clear();

		for (const ACHIEVEMENT* data : rp.Get_achievementList())
		{
			UserAchievementRowPtr row = std::make_shared<UserAchievementRow>(
				PacketKeep<ACHIEVEMENT>{ *data }
			);
			table->Upsert(row);
		}
	}

	// 캐릭터
	//if (UserCharacterTablePtr table = userCache.Get<UserCharacterTable>())
	//{
	//	table->Clear();

	//	for (const CHARACTER* data : rp.Get_characterList())
	//	{
	//		UserCharacterRowPtr row = std::make_shared<UserCharacterRow>(
	//			PacketKeep<CHARACTER>{ *data }, nullptr
	//		);
	//		table->Upsert(row);
	//	}
	//}

	// 아이템
	if (UserItemTablePtr table = userCache.Get<UserItemTable>())
	{
		table->Clear();

		for (const ITEM* data : rp.Get_itemList())
		{
			UserItemRowPtr row = std::make_shared<UserItemRow>(
				PacketKeep<ITEM>{ *data }, nullptr
			);
			table->Upsert(row);
		}
	}

	// 미션
	if (UserMissionTablePtr table = userCache.Get<UserMissionTable>())
	{
		table->Clear();

		for (const MISSION* data : rp.Get_missionList())
		{
			UserMissionRowPtr row = std::make_shared<UserMissionRow>(
				PacketKeep<MISSION>{ *data }
			);
			table->Upsert(row);
		}
	}

	// 퀘스트
	if (UserQuestTablePtr table = userCache.Get<UserQuestTable>())
	{
		table->Clear();

		for (const QUEST* data : rp.Get_questList())
		{
			UserQuestRowPtr row = std::make_shared<UserQuestRow>(
				PacketKeep<QUEST>{ *data }
			);
			table->Upsert(row);
		}
	}
}

void BotScene::CacheUserAssetDelta(const USER_CACHE_DIFF& rp)
{
	UserCacheAccessor& userCache = GetBotContext().GetUserCache();

	USER& user = userCache.GetUser();

	// 경험치
	user.Ref_c_user_exp() += rp.Get_userExp();

	if (UserItemTablePtr table = userCache.Get<UserItemTable>())
	{		
		// 아이템 수량 변경
		for (PacketSize i = 0; i < rp.Get_itemIdList_size(); ++i)
		{
			const ItemId itemId = rp.Get_itemIdList()[i];
			const ItemQuantity itemQuantity = rp.Get_itemQuantityList()[i];

			if (UserItemRowPtr itemRow = table->Find(itemId))
			{ 
				itemRow->SetItemQuantity(itemQuantity);
			}
		}

		// 신규 아이템
		for (const ITEM* data : rp.Get_itemList())
		{
			UserItemRowPtr row = std::make_shared<UserItemRow>(PacketKeep<ITEM>{ *data }, nullptr);
			table->Upsert(row);
		}
	}
}

HandleResult BotScene::OnPacket(DC_ACK_CHEAT& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();
	ReportResultAndYield(result);

	switch (result)
	{
	case Result::SUCCEEDED:
		CacheUserData(rp.Get_userData());
		NextSeq();
		break;
	case Result::RETRY_LATER:
		RetrySeq();
		break;
	default:
		_DEBUG_LOG(RED, L"[{}], {}", GetBotId(), result);
	}

	return HandleResult::OK;
}

HandleResult BotScene::OnPacket(GC_GAME_UPDATE& rp)
{
	_WILL_USE(rp);
	mUpdateGamePerSec.Update(1, tClock.GetLocalNowMs());
	return HandleResult::OK;
}

HandleResult BotScene::OnPacket(class FC_ERROR& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();
	ReportResultAndYield(result);

	RetrySeq();
	return HandleResult::OK;
}

HandleResult BotScene::OnPacket(class MC_ERROR& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();
	ReportResultAndYield(result);

	RetrySeq();
	return HandleResult::OK;
}

HandleResult BotScene::OnPacket(class DC_ERROR& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();
	ReportResultAndYield(result);

	RetrySeq();
	return HandleResult::OK;
}

HandleResult BotScene::OnPacket(class GC_ERROR& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();
	ReportResultAndYield(result);

	RetrySeq();
	return HandleResult::OK;
}

HandleResult BotScene::OnPacket(class BC_ERROR& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();
	ReportResultAndYield(result);

	RetrySeq();
	return HandleResult::OK;
}
