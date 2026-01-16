// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "ClientPacketFlowBase.h"

FClientPacketFlowBase::FClientPacketFlowBase()
{	
	mOnTimerRetrySeq.Add(this, &FClientPacketFlowBase::OnTimerRetrySeq);
	mOnTimerRetryJumpToSeq.Add(this, &FClientPacketFlowBase::OnTimerRetryJumpToSeq);
}

void FClientPacketFlowBase::NextSeq()
{
	++mSeq;

	const SeqHandler seqHandler = mSeqHandlerList[mSeq];
	seqHandler->Invoke();
}

void FClientPacketFlowBase::RetrySeq()
{
	mOnTimerRetrySeq.Invoke(CLOCK_NOW, RETRY_DELAY);
}

void FClientPacketFlowBase::Reset()
{
	mSeq = INVALID_SEQ;
}

void FClientPacketFlowBase::OnTimerRetrySeq()
{
	if (mSeqHandlerList.size() <= mSeq)
		return;

	const SeqHandler seqHandler = mSeqHandlerList[mSeq];
	seqHandler->Invoke();
}

void FClientPacketFlowBase::OnTimerRetryJumpToSeq()
{
	NextSeq();
}

void FClientPacketFlowBase::OnUpdate()
{
	mOnTimerRetrySeq.Update(CLOCK_NOW);
	mOnTimerRetryJumpToSeq.Update(CLOCK_NOW);
}
