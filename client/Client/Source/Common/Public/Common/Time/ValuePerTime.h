// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 시간당 값 계산 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 시간당 값을 계산하는 클래스 (FPS처럼 성능/부하 측정에 사용).
// - 책임: 슬라이딩 윈도우 기반 시간당 값 계산
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
template<size_t MaxSlotCount = 1024>
class ValuePerTime
{
public:
	using TimeDeltaArray = std::array<ClockMs, MaxSlotCount>;
	using ValueArray = std::array<int64_t, MaxSlotCount>;

private:
	TimeDeltaArray mTimeDeltaArray;  // 시간 델타 배열 (슬라이딩 윈도우)
	ClockMs mTimeDeltaTotal = 0ms;   // 전체 시간 델타 합계

	ValueArray mValueArray;          // 값 배열 (슬라이딩 윈도우)
	int64_t mValueTotal = 0;         // 전체 값 합계

	size_t mDataPos = (MaxSlotCount - 1);  // 현재 데이터 위치 (순환 버퍼)
	size_t mDataCount = 0;                 // 데이터 개수

	ClockMs mUpdateTime = 0ms;        // 마지막 업데이트 시간
	size_t mUpdateCount = 0;          // 업데이트 횟수

	float mUpdatePerTime = 0.f;        // 시간당 업데이트 횟수
	float mValuePerTime = 0.f;        // 시간당 값

public:
	DISABLE_COPY(ValuePerTime);
	ValuePerTime()
	{
		mTimeDeltaArray.fill(0ms);
		mValueArray.fill(0);
		mUpdateTime = 0ms;
	}

public:
	// 값을 업데이트하고 시간당 값을 재계산한다.
	// - value: 추가할 값
	// - now: 현재 시간
	void Update(int64_t value, ClockMs now)
	{
		// [WHY] 첫 업데이트 시 시간 초기화
		if (0ms == mUpdateTime)
			mUpdateTime = now;

		ClockMs updateTimeDelta = (now > mUpdateTime) ? (now - mUpdateTime) : 0ms;
		mUpdateTime = now;

		// [WHY] 순환 버퍼로 슬라이딩 윈도우 구현
		++mDataPos;
		mDataPos = mDataPos % MaxSlotCount;

		// [WHY] 오래된 값을 제거하고 새 값을 추가
		mValueTotal += value;
		mValueTotal -= mValueArray[mDataPos];
		mValueArray[mDataPos] = value;

		mTimeDeltaTotal += updateTimeDelta;
		mTimeDeltaTotal -= mTimeDeltaArray[mDataPos];
		mTimeDeltaArray[mDataPos] = updateTimeDelta;

		++mDataCount;
		if (MaxSlotCount < mDataCount)
			mDataCount = MaxSlotCount;

		// [WHY] 시간당 값 계산 (밀리초를 초로 변환)
		if (0ms == mTimeDeltaTotal)
		{
			mUpdatePerTime = 0;
			mValuePerTime = 0.f;
		}
		else
		{
			float perTime = 1000.f / mTimeDeltaTotal.count();

			mUpdatePerTime = mDataCount * perTime;
			mValuePerTime = mValueTotal * perTime;
		}

		++mUpdateCount;
	}

	// 업데이트 횟수를 반환한다.
	// - return: 업데이트 횟수
	size_t GetUpdateCount() const noexcept { return mUpdateCount; }

	// 시간당 업데이트 횟수를 반환한다.
	// - return: 시간당 업데이트 횟수
	float GetUpdatePerTime() const noexcept { return mUpdatePerTime; }

	// 시간당 값을 반환한다.
	// - return: 시간당 값
	float GetValuePerTime() const noexcept { return mValuePerTime; }
};
