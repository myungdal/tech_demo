// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Util/TimeStep/TimeStepInternal/TimeStepQueue.h"
#include "MmoSync/Util/TimeStep/TimeStepInternal/TimeStepTuner.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeStepPlayer
// Peer 가 사용한다.
// 
// immediateMode:
//   - true: 시간 제어 없이 쌓인 데이터 즉시 처리 (서버용)
//   - false: 시간 기반 부드러운 재생 (클라이언트용)
// 
// catchUpMode (immediateMode=false일 때만 유효):
//   - true: 공격적 따라잡기 (속도 0.0~2.0, 빠른 반응)
//   - false: 부드러운 재생 (속도 0.8~1.2, 느린 반응)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _CommandQueue> 
class TimeStepPlayer final
{
public:
	using CommandStepPair = std::pair<_CommandQueue*, TimeStep>;
	using CommandStepQueue = std::deque<CommandStepPair>;

private:
	CommandStepQueue mDataQueue;
	TimeStep mDataRemaindStep = 0;

	TimeStepQueue mPlayerQueue;
	TimeStep mPlayerRemaindStep = 0;

	TimeStepTuner mTuner;
	const int64_t mInterval = 0;
	double mJitterTime = 0.0;
	double mQueuedTime = 0.0;

	bool mImmediateMode = false;

public:
	DISABLE_COPY(TimeStepPlayer);

	// 서버용: immediateMode = true (시간 제어 없이 즉시 처리)
	explicit TimeStepPlayer(int64_t interval, ClockPoint now, bool immediateMode)
		:
		mPlayerQueue(interval, now),
		mTuner(false),
		mInterval(interval),
		mImmediateMode(immediateMode)
	{
	}

	// 클라이언트용: immediateMode = false, catchUpMode 지정
	explicit TimeStepPlayer(int64_t interval, ClockPoint now, bool immediateMode, bool catchUpMode)
		:
		mPlayerQueue(interval, now),
		mTuner(catchUpMode),
		mInterval(interval),
		mImmediateMode(immediateMode)
	{
	}

public:
	void Reset(ClockPoint now)
	{
		mDataQueue.clear();
		mDataRemaindStep = 0;

		mPlayerQueue.Reset(now);
		mPlayerRemaindStep = 0;

		mTuner.Reset();

		mJitterTime = 0.0;
		mQueuedTime = 0.0;
	}

	void Push(_CommandQueue* queue, TimeStep timeStep)
	{
		mDataQueue.emplace_back(std::make_pair(queue, timeStep));

		// 즉시 모드에서는 시간 제어 관련 작업 스킵
		if (!mImmediateMode)
		{
			mJitterTime = mJitterTime * 0.9 + mInterval * timeStep * 0.1;
			mPlayerQueue.PushStep(timeStep);
		}
	}

	CommandStepPair Play(ClockPoint now)
	{
		// 즉시 모드: 시간 제어 없이 데이터 있으면 바로 반환
		if (mImmediateMode)
		{
			if (mDataQueue.empty())
				return std::pair{ nullptr, 0 };

			auto [commandQueue, dataStep] = mDataQueue.front();
			mDataQueue.pop_front();
			return std::pair{ commandQueue, dataStep };
		}

		// 시간 기반 모드: 기존 로직
		_CommandQueue* resultCommandQueue = nullptr;
		TimeStep resultTimeStep = 0;

		if (mPlayerRemaindStep >= mDataRemaindStep)
		{
			if (mDataQueue.empty())
			{
				return std::pair{ nullptr, 0 };
			}

			auto [commandQueue, dataStep] = mDataQueue.front();
			mDataQueue.pop_front();

			resultCommandQueue = commandQueue;

			mDataRemaindStep += dataStep;
		}
		else
		{
			// 적당한 시간 속도로 스텝을 가져온다
			const double speed = mTuner.GetSpeed();
			TimeStep playerStep = mPlayerQueue.PopStep(now, speed);

			if (INVALID_TIME_STEP == playerStep)
				return std::pair{ nullptr, 0 };

			// 시간 속도 튜닝
			TuneSpeed(playerStep);

			// 남아았던 플레이어 시간 병합
			mPlayerRemaindStep += playerStep;
		}

		if (mPlayerRemaindStep > mDataRemaindStep)
		{
			resultTimeStep = mDataRemaindStep;

			mPlayerRemaindStep -= mDataRemaindStep;
			mDataRemaindStep = 0;
		}
		else
		{
			resultTimeStep = mPlayerRemaindStep;

			mDataRemaindStep -= mPlayerRemaindStep;
			mPlayerRemaindStep = 0;
		}

		return std::pair{ resultCommandQueue,  resultTimeStep };
	}

private:
	void TuneSpeed(TimeStep stepCount)
	{
		const TimeStep queuedStepCount = mPlayerQueue.GetQueuedStepCount();
		const double queuedTime = mInterval * (double)queuedStepCount;

		while (0 < stepCount--)
		{			
			mTuner.TuneSpeed(mJitterTime, queuedTime);
		}
	}

public:
	CommandStepQueue& GetDataQueue() { return mDataQueue; }
	TimeStep GetQueuedStepCount() const { return mPlayerQueue.GetQueuedStepCount(); }
};
