// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Common/Invoker/TimerInvoker.h"


class NetworkPacket;
class UUiEventBase;

/**
 * 클라이언트 패킷 플로우 공통 베이스.
 * 시퀀스(Seq) 기반 처리/재시도 타이머를 제공한다.
 */
class FClientPacketFlowBase
{
private:
	static constexpr ClockMs RETRY_DELAY = 1000ms;

	TimerInvoker<> mOnTimerRetrySeq;
	TimerInvoker<> mOnTimerRetryJumpToSeq;

private:
	using SeqHandler = std::shared_ptr<InvokerBase<>>;
	using SeqHandlerList = std::vector<SeqHandler>;
	using Seq = uint16_t;
	const Seq INVALID_SEQ = UINT16_MAX;

	SeqHandlerList mSeqHandlerList;
	Seq mSeq = INVALID_SEQ;

public:
	DISABLE_COPY(FClientPacketFlowBase);
	explicit FClientPacketFlowBase();
	virtual ~FClientPacketFlowBase() = default;

protected:
	template <typename _Owner, typename _Function>
	void RegisterSeq(_Owner* scene, _Function&& function)
	{
		using InvokerType = InvokerImpl<_Owner, _Function>;
		SeqHandler seqHandler =
			std::make_shared<InvokerType>(scene, std::forward<_Function>(function));
		mSeqHandlerList.emplace_back(seqHandler);
	}

	template <typename _Owner, typename _Function>
	void JumpToSeq(_Owner* scene, _Function&& function)
	{
		using InvokerType = InvokerImpl<_Owner, _Function>;
		InvokerType seqHandler(scene, std::forward<_Function>(function));

		auto it = std::find_if(
			mSeqHandlerList.begin(), mSeqHandlerList.end(),
			[&seqHandler](const SeqHandler& elem)
			{
				void* a = static_cast<void*>(&seqHandler);
				void* b = static_cast<void*>(elem.get());
				// padding 제외하고 비교.
				const size_t size = sizeof(InvokerBase<>) + sizeof(_Owner*) + sizeof(typename InvokerType::Function);
				const bool result = (0 == memcmp(a, b, size));
				return result;
			}
		);
		if (mSeqHandlerList.end() == it)
			return;

		mSeq = static_cast<Seq>(std::distance(mSeqHandlerList.begin(), it) - 1);

		NextSeq();
	}

	template <typename _Owner, typename _Function>
	void RetryJumpToSeq(_Owner* scene, _Function&& function)
	{
		using InvokerType = InvokerImpl<_Owner, _Function>;
		InvokerType seqHandler(scene, std::forward<_Function>(function));

		auto it = std::find_if(
			mSeqHandlerList.begin(), mSeqHandlerList.end(),
			[&seqHandler](const SeqHandler& elem)
			{
				void* a = static_cast<void*>(&seqHandler);
				void* b = static_cast<void*>(elem.get());
				// padding 제외하고 비교.
				const size_t size = sizeof(InvokerBase<>) + sizeof(_Owner*) + sizeof(typename InvokerType::Function);
				const bool result = (0 == memcmp(a, b, size));
				return result;
			}
		);
		if (mSeqHandlerList.end() == it)
			return;

		mSeq = static_cast<Seq>(std::distance(mSeqHandlerList.begin(), it) - 1);

		mOnTimerRetryJumpToSeq.Invoke(CLOCK_NOW, RETRY_DELAY);
	}

	void NextSeq();
	void RetrySeq();
	void Reset();

public:
	void OnTimerRetrySeq();
	void OnTimerRetryJumpToSeq();

	friend class UNetworkManager;
	friend class UUiServiceManager;
protected:
	virtual void OnConnected() {}
	virtual void OnDisconnected() {}
	virtual void OnFatalError() {}
	virtual void OnDispatchPacket(NetworkPacket& rp) {}
	virtual void OnDispatchUiEvent(UUiEventBase* uiEvent) {}
	virtual void OnUpdate();
};

