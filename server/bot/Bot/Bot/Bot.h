// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerBase/Worker.h"

#include "Bot/Socket/SocketBotToFront.h"


class NetworkPacketWriter;
class Iocp;
class BotScene;
class BotContext;
class LogWriterFile;
struct AppConfigData;

// 부하 테스트 및 자동화 테스트용 개별 봇 인스턴스
// - Worker를 상속받아 비동기 작업 단위로 동작
// - BotScene을 통해 시나리오별 로직 수행
class Bot final : public Worker
{
	friend class BotScene;

private:
	const AppConfigData& mAppConfigData;
	BotId mBotId = INVALID_BOT_ID;
	std::wstring mBotName;
	std::shared_ptr<Iocp> mIocp = nullptr;
	std::shared_ptr<BotContext> mBotContext = nullptr;
	std::shared_ptr<LogWriterFile> mLogWriterFile = nullptr;
	std::shared_ptr<Log> mLog = nullptr;

	const BotTestList* mTestList = nullptr;
	int mTestListIdx = 0;

	SocketBotToFrontWeakPtr mSocketBotToFrontWeakPtr = nullptr;
	std::shared_ptr<BotScene> mBotScene = nullptr;
	std::wstring mSceneName = L"";
	int32_t mScenarioFinishCount = 0;

private:
	using BotHistorySendRecv = std::list<const wchar_t*>;
	BotHistorySendRecv mBotHistorySendRecv;

public:
	explicit Bot(BotId botId, std::shared_ptr<Iocp>& iocp, const AppConfigData& appConfigData);

public:
	void Start();

private:
	void ConnectToFront();	
	void DisconnectFromFront();	

	void NextScene_async();
	void ChangeToReconnectScene_async();

public:
	void OnHandshakeCompletedFront_async(bool reconnected);
	void OnLostConnectionFront_async();
	void OnDispatchPacketFront_async(PacketTemp tp);

private:
	void CreateScene_async(std::wstring sceneName);

private:
	void ReportBotEvent(BotEventType botEventType) const;
	void ReportRecv(PacketType packetType);
	void ReportSend(PacketType packetType);
	void ReportResult(Result result) const;

public:
	bool IsConnectedWithFront();
	void SetEnableReconnect(bool enable);
	BotContext& GetBotContext() const { return *mBotContext; }
	ClockMs GetBotTaskDelay() const;
	BotId GetBotId() const { return mBotId; }
	const std::wstring& GetBotName() const;
	const std::wstring& GetSceneName() const { return mSceneName; }
	SOCKET GetRawSocket();
	SocketPtr<SocketBotToFront> GetSocket();
	std::shared_ptr<LogWriterFile> GetLogWriterFile() { return mLogWriterFile; }
	std::shared_ptr<Log> GetLog() { return mLog; }
};
