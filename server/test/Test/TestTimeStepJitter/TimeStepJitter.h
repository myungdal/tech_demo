// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// TimeStepJitter.h - TimeStep Jitter 시뮬레이션 클래스
#pragma once

#include <cmath>
#include <cstdint>
#include <chrono>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeStepGenerator: 일정 간격으로 타임스텝 생성
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TimeStepJitterGenerator
{
private:
	int64_t mInterval = 0;		// 타임스텝 간격 (ms)
	int64_t mStartTime = 0;		// 시작 시간 (ms)
	int64_t mPastStepCount = 0;	// 지금까지 생성된 스텝 수

public:
	explicit TimeStepJitterGenerator(int64_t interval)
		: mInterval(interval)
		, mStartTime(GetNowMs())
	{
	}

	// 현재 시간 기준으로 생성해야 할 스텝 수 반환
	int64_t Generate()
	{
		int64_t now = GetNowMs();

		if (now <= mStartTime)
			return 0;

		int64_t timeStepNow = mStartTime + mPastStepCount * mInterval;
		int64_t deltaMs = now - timeStepNow;

		if (deltaMs <= 0)
			return 0;

		int64_t stepCount = deltaMs / mInterval;
		mPastStepCount += stepCount;

		return stepCount;
	}

	static int64_t GetNowMs()
	{
		using namespace std::chrono;
		return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeStepJitterQueue: 스텝 큐 관리
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TimeStepJitterQueue
{
private:
	int64_t mInterval = 0;			// 타임스텝 간격 (ms)
	int64_t mStartTime = 0;			// 시작 시간 (ms)
	int64_t mQueuedStepCount = 0;	// 큐에 쌓인 스텝 수
	int64_t mPastStepCount = 0;		// 지금까지 소비된 스텝 수

public:
	explicit TimeStepJitterQueue(int64_t interval)
		: mInterval(interval)
		, mStartTime(TimeStepJitterGenerator::GetNowMs())
	{
	}

	void PushStep(int64_t count)
	{
		mQueuedStepCount += count;
	}

	int64_t PopStep(double speed)
	{
		int64_t now = TimeStepJitterGenerator::GetNowMs();

		if (now <= mStartTime)
			return 0;

		int64_t timeStepNow = mStartTime + mPastStepCount * mInterval;
		int64_t deltaMs = now - timeStepNow;

		int64_t stepCount = deltaMs / mInterval;
		mPastStepCount += stepCount;

		int64_t adaptedMs = static_cast<int64_t>(deltaMs * speed);
		int64_t adaptedCount = adaptedMs / mInterval;

		if (adaptedCount > mQueuedStepCount)
			adaptedCount = mQueuedStepCount;

		mQueuedStepCount -= adaptedCount;

		return adaptedCount;
	}

	int64_t GetQueuedStepCount() const { return mQueuedStepCount; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeStepJitterTuner: 지터 보상을 위한 속도 조절
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TimeStepJitterTuner
{
private:
	static constexpr double JITTER_COEFFICIENT = 3.0;
	static constexpr double TIME_COEFFICIENT = 1.0 / 300.0;
	static constexpr double CATCH_UP_THRESHOLD = 300.0;
	static constexpr double MAX_SPEED = 1.5;
	static constexpr double SPEED_COEFFICIENT = 0.1;

	double mSpeed = 1.0;

public:
	double GetSpeed() const { return mSpeed; }

	void TuneSpeed(double jitterTime, double queuedTime)
	{
		double backlog = queuedTime - jitterTime * JITTER_COEFFICIENT;
		double t = backlog * TIME_COEFFICIENT;
		double speed = 1.0 + DoubleSigmoid(t) * SPEED_COEFFICIENT;

		if (backlog > CATCH_UP_THRESHOLD)
		{
			double extra = (backlog - CATCH_UP_THRESHOLD) / CATCH_UP_THRESHOLD * SPEED_COEFFICIENT;
			speed += extra;
		}

		speed = std::min(speed, MAX_SPEED);
		mSpeed = mSpeed * 0.9 + speed * 0.1;
	}

	// 시그모이드 기반 속도 조절 함수 (시각화용 public 접근)
	static double DoubleSigmoid(double time)
	{
		time *= 5.0;
		double sigmoid = 1.0 / (1.0 + std::exp(-time));

		if (time < 0)
			return (sigmoid * (1.0 - sigmoid) - 0.25) * 4.0;

		return -(sigmoid * (1.0 - sigmoid) - 0.25) * 4.0;
	}

	static double GetJitterCoefficient() { return JITTER_COEFFICIENT; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeStepJitterPlayer: Generator + Queue + Tuner 통합 플레이어
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TimeStepJitterPlayer
{
private:
	TimeStepJitterQueue mQueue;
	TimeStepJitterTuner mTuner;
	int64_t mInterval = 0;
	double mJitterTime = 0.0;
	double mQueuedTime = 0.0;

public:
	explicit TimeStepJitterPlayer(int64_t interval)
		: mQueue(interval)
		, mInterval(interval)
	{
	}

	void PushStep(int64_t count)
	{
		if (count == 0)
			return;

		mJitterTime = mJitterTime * 0.9 + count * static_cast<double>(mInterval) * 0.1;
		mQueue.PushStep(count);
	}

	int64_t PlayStep()
	{
		double speed = mTuner.GetSpeed();
		int64_t stepCount = mQueue.PopStep(speed);

		if (stepCount == 0)
			return 0;

		int64_t queuedStepCount = mQueue.GetQueuedStepCount();

		for (int64_t i = stepCount; i > 0; --i)
		{
			mQueuedTime = mQueuedTime * 0.9 + queuedStepCount * static_cast<double>(mInterval) * 0.1;
			mTuner.TuneSpeed(mJitterTime, mQueuedTime);
		}

		return stepCount;
	}

	// 시각화용 접근자
	int64_t GetQueuedStepCount() const { return mQueue.GetQueuedStepCount(); }
	double GetSpeed() const { return mTuner.GetSpeed(); }
	double GetJitterTime() const { return mJitterTime; }
	double GetTargetQueuedTime() const { return mJitterTime * TimeStepJitterTuner::GetJitterCoefficient(); }

	const TimeStepJitterTuner& GetTuner() const { return mTuner; }
};
