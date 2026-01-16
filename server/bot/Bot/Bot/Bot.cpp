// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;


Bot::Bot(BotId botId, std::shared_ptr<Iocp>& iocp, const AppConfigData& appConfigData)
	:
	mAppConfigData(appConfigData)
{
	mBotId = botId;
	mBotName = std::format(L"BOT{:0>16}", botId);
	mIocp = iocp;
	mBotContext = std::make_shared<BotContext>();
	mLogWriterFile = std::make_shared<LogWriterFile>(L"INDIVIDUAL_BOT/", std::format(L"BOT{:04}", mBotId).c_str());
	mLog = std::make_shared<Log>();
	mTestList = gAppConfigManager->GetBotTestList(mAppConfigData.mBotScenarioId);
	_ASSERT_CRASH(nullptr != mTestList);
	mTestListIdx = 0;
	ReportBotEvent(BotEventType::CREATED);	
}
void Bot::Start()
{
	ReportBotEvent(BotEventType::SCENARIO_STARTED);

	WorkerManager::Run(
		shared_from_this(), this, &Bot::CreateScene_async, (*mTestList)[mTestListIdx]
	);
}

void Bot::ConnectToFront()
{
	if (auto socket = mSocketBotToFrontWeakPtr.Lock())
	{
		ReportBotEvent(BotEventType::CONNECT_FRONT);

		socket->Connect(
			mAppConfigData.mBotDestIp.c_str(), 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_CF"),
			INVALID_APP_ID,
			AppType::MAX,
			INVALID_APP_ID,
			AppType::MAX
		);
	}
	else
	{
		SocketBotToFront* socketBotToFront = SocketBotToFront::AcquireSocketImpl();
		
		socketBotToFront->SetBot(std::static_pointer_cast<Bot>(shared_from_this()));
		socketBotToFront->SetIocp(mIocp);
		socketBotToFront->SetEnableReconnect(true);
		
		ReportBotEvent(BotEventType::CONNECT_FRONT);
		
		socketBotToFront->Connect(
			mAppConfigData.mBotDestIp.c_str(), 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_CF"),
			INVALID_APP_ID,
			AppType::MAX,
			INVALID_APP_ID,
			AppType::MAX
		);
		
		mSocketBotToFrontWeakPtr.Set(socketBotToFront, L"Bot, WeakPtr");
	}
}

void Bot::DisconnectFromFront()
{
	if (auto socket = mSocketBotToFrontWeakPtr.Lock())
	{
		ReportBotEvent(BotEventType::DISCONNECT_FRONT);

		socket->Disconnect();
	}
}

void Bot::NextScene_async()
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	ReportBotEvent(BotEventType::SCENE_FINISHED);

	++mTestListIdx;

	if (mTestList->size() > mTestListIdx)
	{
		CreateScene_async((*mTestList)[mTestListIdx]);

		return;
	}
	
	ReportBotEvent(BotEventType::SCENARIO_FINISHED);

	++mScenarioFinishCount;

	if ((1 > mAppConfigData.mBotRepeatCount) ||
		(mAppConfigData.mBotRepeatCount > mScenarioFinishCount))
	{
		mTestListIdx = 0;

		ReportBotEvent(BotEventType::SCENARIO_STARTED);

		CreateScene_async((*mTestList)[mTestListIdx]);
	}
	else
	{
		ReportBotEvent(BotEventType::DESTROYED);
		
		//mBotScene = nullptr;
	}
}

void Bot::ChangeToReconnectScene_async()
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	// 재연결 성공하면 로그인 부터 다시 시작하게 하기 위한.
	mTestListIdx = -1;

	CreateScene_async(L"BotSceneReconnect");
}

void Bot::OnHandshakeCompletedFront_async(bool reconnected)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	ReportBotEvent(BotEventType::CONNECTED_FRONT);

	mBotScene->OnHandshakeCompleted(reconnected);
}

void Bot::OnLostConnectionFront_async()
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	ReportBotEvent(BotEventType::DISCONNECTED_FRONT);

	mBotScene->OnLostConnection();
}

void Bot::OnDispatchPacketFront_async(PacketTemp tp)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	ReportRecv(tp->GetPacketType());

	auto& rp = **tp;

	if (false == ValidatePacket(rp))
	{
		_DEBUG_RED;

		if (auto socket = mSocketBotToFrontWeakPtr.Lock())
		{
			socket->Disconnect();
		}

		return;
	}

	if (mBotContext->CurrPacketHeader().GetUserId() == rp.GetHeader().GetUserId())
	{
		mBotContext->CurrPacketHeader() = rp.GetHeader();
	}

	if (false == mBotScene->OnDispatchPacket(**tp))
	{
		_DEBUG_RED;

		if (auto socket = mSocketBotToFrontWeakPtr.Lock())
		{
			socket->Disconnect();
		}

		return;
	}
}

void Bot::CreateScene_async(std::wstring sceneName)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	mSceneName = sceneName;
	mBotScene = BotSceneFactory::CreateScene(std::static_pointer_cast<Bot>(shared_from_this()), sceneName);

	ReportBotEvent(BotEventType::SCENE_STARTED);
}

void Bot::ReportBotEvent(BotEventType botEventType) const
{
	WorkerManager::Run(
		gBotMonitor, gBotMonitor.get(), &BotMonitor::OnBotEvent_async,
		botEventType, mBotId, mSceneName, INVALID_BOT_SCENE_SEQ, tClock.GetGlobalNow()
	);
}

void Bot::ReportRecv(PacketType packetType)
{
#if _BOT_HISTORY_SEND_RECV
	mBotHistorySendRecv.push_front(GetPacketTypeString(packetType));
#endif

	WorkerManager::Run(
		gBotMonitor, gBotMonitor.get(), &BotMonitor::OnPacketRecv_async,
		packetType
	);
}

void Bot::ReportSend(PacketType packetType)
{
#if _BOT_HISTORY_SEND_RECV
	mBotHistorySendRecv.push_front(GetPacketTypeString(packetType));
#endif

	WorkerManager::Run(
		gBotMonitor, gBotMonitor.get(), &BotMonitor::OnPacketSend_async,
		packetType
	);
}

void Bot::ReportResult(Result result) const
{
	WorkerManager::Run(
		gBotMonitor, gBotMonitor.get(), &BotMonitor::OnPacketResult_async,
		result
	);
}

bool Bot::IsConnectedWithFront()
{
	if (auto socket = mSocketBotToFrontWeakPtr.Lock())
	{
		return socket->IsConnected();
	}

	return false;
}

ClockMs Bot::GetBotTaskDelay() const
{ 
	return mAppConfigData.mBotTaskDelay; 
}

void Bot::SetEnableReconnect(bool enable)
{
	if (auto socket = mSocketBotToFrontWeakPtr.Lock())
	{
		socket->SetEnableReconnect(enable);
	}
}

const std::wstring& Bot::GetBotName() const
{	
	return mBotName;
}

SOCKET Bot::GetRawSocket()
{
	if (auto socket = mSocketBotToFrontWeakPtr.Lock())
	{
		return socket->GetRawSocket();
	}
	else
	{
		return INVALID_SOCKET;
	}
}

SocketPtr<SocketBotToFront> Bot::GetSocket()
{
	return mSocketBotToFrontWeakPtr.Lock();
}
