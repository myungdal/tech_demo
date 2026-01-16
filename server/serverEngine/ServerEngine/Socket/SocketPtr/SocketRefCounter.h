// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/UsageMeter/UsageMeter.h"


class SocketRefCounter
{
private:
	std::atomic<int32_t> mRefCnt = 0;

public:
	void InitRefCnt()
	{
		UsageMeter<SocketRefCounter, SocketRefCounter>::OnInit(0);

		int32_t result = mRefCnt.exchange(1);
		if (0 != result)
		{
			_DEBUG_BREAK;
			_DEBUG_RED;
		}
	}

	void ResetRefCnt()
	{
		mRefCnt.store(0);
	}

	bool IncreaseRefCnt()
	{
		int32_t expected = mRefCnt.load();

		// [WHY] 디버그용 안전장치: 코딩 실수(이중 Decrease/누수 등)로 ref count가 비정상적으로 커지는지 빠르게 감지한다.
		// [TODO] 안정화 후 정책 결정(제거/디버그 전용 유지/로그만 남김).

#if _SOCKET_HISTORY_REF_COUNT
		//if (0 == expected)
		//{
		//	_DEBUG_BREAK;
		//}
		if (100 < expected)
		{
			_DEBUG_BREAK;
		}
#endif

		while (0 != expected)
		{
			const bool result = mRefCnt.compare_exchange_strong(expected, expected + 1);
			if (true == result)
			{
				UsageMeter<SocketRefCounter, SocketRefCounter>::OnIncrease();

				return true;
			}

			expected = mRefCnt.load();
		}

		return false;
	}

	int32_t DecreaseRefCnt()
	{
		int32_t result = mRefCnt.fetch_sub(1) - 1;

		UsageMeter<SocketRefCounter, SocketRefCounter>::OnDecrease();

		return result;
	}

	int32_t GetRefCnt() const
	{
		return mRefCnt.load();
	}
};

using SocketRefCounterPtr = std::shared_ptr<SocketRefCounter>;
