// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Invoker/Invoker.h"
// ValuePerTime은 BotScene.ixx의 GMF에서 include 및 export

#include "Bot/Socket/SocketBotToFront.h"


class NetworkPacket;
class NetworkPacketWriter;
class BotContext;

// 봇 시나리오 단계를 표현하는 기본 씬 클래스
// - RegisterSeq()로 시퀀스 등록, NextSeq()로 다음 단계 진행
// - OnDispatchPacket()을 오버라이드하여 패킷 처리
class BotScene
{
private:
	static constexpr ClockMs RETRY_DELAY = 1000ms;
	
private:
	BotPtr mBot;	

private:
	using SeqHandler = std::shared_ptr<InvokerBase<>>;
	using SeqHandlerList = std::vector<SeqHandler>;

	SeqHandlerList mSeqHandlerList;
	BotSceneSeq mSeq = INVALID_BOT_SCENE_SEQ;

	bool mIsOnTimer = false;
	bool mReservedReconnectScene = false;

	ValuePerTime<1024> mUpdateGamePerSec;

public:
	explicit BotScene(BotPtr& bot);
	virtual ~BotScene() = default;

protected:
	template <typename _Owner, typename _Function>
	void RegisterSeq(_Owner* scene, _Function&& function);

	template <typename _Owner, typename _Function>
	void JumpToSeq(_Owner* scene, _Function&& function);

	friend class Bot;
	void EndSeq();
	void NextSeq();
	void NextSeq_timer_async();
	void RetrySeq();
	void RetrySeq_timer_async();

	void TryChangeToReconnectScene();
	bool CheckReconnectScene() const;
	void ChangeToReconnectScene() const;

protected:
	void ConnectToFront() const;
	bool IsConnectedWithFront() const;
	void SetEnableReconnect(bool enable) const;
	void DisconnectFromFront() const;

	void ReportResultAndYield(Result result) const;
	void ReportBotEvent(BotEventType botEventType);

public:
	void ReportSend(PacketType packetType) const;

public:
	virtual void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) {}
	virtual void OnLostConnection() {}

public:
	virtual bool OnDispatchPacket(NetworkPacket& rp) = 0;

public:
	BotContext& GetBotContext() const;
	ClockMs GetBotTaskDelay() const;	
	BotId GetBotId() const;
	const std::wstring& GetBotName() const;
	const std::wstring& GetSceneName() const;
	SOCKET GetRawSocket();
	SocketPtr<SocketBotToFront> GetSocket();
	BotSceneSeq GetSeq() const { return mSeq; }

public:	
	void CacheUserData(const class USER_DATA& rp);
	void CacheUserAssetDelta(const class USER_CACHE_DIFF& rp);

	HandleResult OnPacket(MAYBE_UNUSED NetworkPacket& rp) const;
	HandleResult OnPacket(class DC_ACK_CHEAT& rp);
	HandleResult OnPacket(class GC_GAME_UPDATE& rp);

	HandleResult OnPacket(class FC_ERROR& rp);
	HandleResult OnPacket(class MC_ERROR& rp);
	HandleResult OnPacket(class DC_ERROR& rp);
	HandleResult OnPacket(class GC_ERROR& rp);
	HandleResult OnPacket(class BC_ERROR& rp);
	HandleResult OnPacket(class FC_KICK&) const { return HandleResult::OK; }
	HandleResult OnPacket(class FC_NOTIFY&) const { return HandleResult::OK; }
	HandleResult OnPacket(class GC_WORLD_DATA&) const { return HandleResult::OK; }
	HandleResult OnPacket(class GC_GAME_CHANNEL_USER_ENTER&) const { return HandleResult::OK; }
};

template <typename _Owner, typename _Function>
void BotScene::RegisterSeq(_Owner* scene, _Function&& function)
{
	using InvokerType = InvokerImpl<_Owner, _Function>;
	SeqHandler seqHandler =
		std::make_shared<InvokerType>(scene, std::forward<_Function>(function));
	mSeqHandlerList.emplace_back(seqHandler);
}

template <typename _Owner, typename _Function>
void BotScene::JumpToSeq(_Owner* scene, _Function&& function)
{
	using InvokerType = InvokerImpl<_Owner, _Function>;
	InvokerType seqHandler(scene, std::forward<_Function>(function));

	auto it = std::find_if(
		mSeqHandlerList.begin(), mSeqHandlerList.end(),
		[&seqHandler](const SeqHandler& elem)
		{
			const void* a = static_cast<void*>(&seqHandler);
			const void* b = static_cast<void*>(elem.get());
			// padding 제외하고 비교.
			const size_t size = sizeof(InvokerBase<>) + sizeof(_Owner*) + sizeof(typename InvokerType::Function);
			const bool result = (0 == memcmp(a, b, size));
			return result;
		}
	);
	if (mSeqHandlerList.end() == it)
	{
		_ASSERT_CRASH(false);
		return;
	}

	mSeq = static_cast<BotSceneSeq>(std::distance(mSeqHandlerList.begin(), it) - 1);

	NextSeq();
}
