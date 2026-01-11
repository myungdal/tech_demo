// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 수학 유틸리티 네임스페이스
// - Angle 체계: 0 == 0도, 16'384 == 90도, -32'768 == 180도, -16'384 == 270도
// - Sin/Cos 값: 1024 == 1.0 (고정소수점)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace MathUtil
{
	// 각도를 Angle 타입으로 변환한다.
	// - deg: 각도 (도 단위)
	// - return: Angle 값
	inline Angle DegreeToAngle(int deg) noexcept;

	// Angle을 라디안으로 변환한다.
	// - angle: Angle 값
	// - return: 라디안 값
	inline float AngleToRadian(Angle angle) noexcept;

	// 라디안을 Angle로 변환한다.
	// - radian: 라디안 값
	// - return: Angle 값
	inline Angle RadianToAngle(float radian) noexcept;

	// 값이 0에 가까운지 확인한다.
	// - value: 확인할 값
	// - return: 0에 가까우면 true
	inline bool IsZero(float value) noexcept;

	// 로그2 값을 계산한다.
	// - n: 입력값
	// - return: log2(n)
	inline size_t Log2(size_t n) noexcept;

	// 값을 범위 내로 제한한다.
	// - v: 제한할 값
	// - lo: 최소값
	// - hi: 최대값
	// - return: 제한된 값
	template<typename _Type>
	_Type Clamp(const _Type v, const _Type lo, const _Type hi) noexcept
	{
		return (v < lo) ? lo : (hi < v) ? hi : v;
	}

	// 값의 제곱을 계산한다.
	// - v: 입력값
	// - return: v^2
	int64_t Pow(int64_t v) noexcept;

	// 나눗셈을 수행한다. 0으로 나누면 0을 반환.
	// - a: 피제수
	// - b: 제수
	// - return: a / b (b가 0이면 0)
	inline int64_t Div(int64_t a, int64_t b) noexcept;

	// 나눗셈을 수행하고 결과에 1024를 곱하여 반환한다 (고정소수점).
	// - a: 피제수
	// - b: 제수
	// - return: (a / b) * 1024 (b가 0이면 0)
	inline int64_t Div1024(int64_t a, int64_t b) noexcept;

	// 만분률로 곱하기를 수행한다.
	// - a: 첫 번째 값
	// - b: 두 번째 값 (만분률)
	// - return: (a * b) / 10000
	template<typename _Type>
	inline _Type MultiplyBp(_Type a, _Type b) noexcept;

	// 두 점 사이의 거리를 계산한다 (-2^30 ~ 2^30 범위 지원, 오차 5% 미만).
	// - dx: x 차이
	// - dy: y 차이
	// - return: 거리
	inline int64_t Distance(int64_t dx, int64_t dy) noexcept;

	// 두 점 사이의 거리 제곱을 계산한다.
	// - dx: x 차이
	// - dy: y 차이
	// - return: 거리^2
	inline int64_t DistanceSq(int64_t dx, int64_t dy) noexcept;

	// 제곱근을 계산한다 (-2^30 ~ 2^30 범위 지원, 오차 없음).
	// - number: 입력값
	// - return: 제곱근
	inline int64_t Sqrt(int64_t number) noexcept;

	// 사인 값을 계산한다.
	// - angle: 각도 (Angle 타입)
	// - return: sin 값 (1024 == 1.0)
	inline int64_t Sin(Angle angle) noexcept;

	// 코사인 값을 계산한다.
	// - angle: 각도 (Angle 타입)
	// - return: cos 값 (1024 == 1.0)
	inline int64_t Cos(Angle angle) noexcept;

	// 탄젠트 값을 계산한다.
	// - angle: 각도 (Angle 타입)
	// - return: tan 값
	inline int64_t Tan(Angle angle) noexcept;

	// 아크탄젠트를 계산한다 (Doom 알고리즘 기반).
	// - x: x 좌표
	// - y: y 좌표
	// - return: 각도 (Angle 타입)
	inline Angle ArcTan(int64_t x, int64_t y) noexcept;

	// 두 각도 사이의 차이를 계산한다 (최단 각도).
	// - from: 시작 각도
	// - to: 목표 각도
	// - return: 각도 차이 (-32768 ~ 32767 범위)
	inline Angle GetAngleDelta(Angle from, Angle to) noexcept;

	// 각도로부터 단위 벡터를 계산한다 (1024 스케일).
	// - angle: 각도 (Angle 타입)
	// - return: (x, y) 벡터 (1024 스케일)
	inline std::pair<int64_t, int64_t> GetVec1024FromAngle(Angle angle) noexcept;

	// 벡터를 정규화하여 반환한다 (1024 스케일).
	// - x: x 좌표
	// - y: y 좌표
	// - return: 정규화된 (x, y) 벡터 (1024 스케일)
	inline std::pair<int64_t, int64_t> GetVec1024(int64_t x, int64_t y) noexcept;

	template<typename _RAND_FNC>
	Angle GetRandomAngle(_RAND_FNC randFnc) noexcept
	{
		return static_cast<Angle>(randFnc(-32'768, 32'767));
	}

	template<typename _RAND_FNC>
	std::pair<int64_t, int64_t> GetVec1024FromRandomAngle(_RAND_FNC randFnc) noexcept
	{
		const Angle angle = GetRandomAngle(randFnc);
		return GetVec1024FromAngle(angle);
	}

	// 내적을 계산한다.
	// - x0, y0: 첫 번째 벡터
	// - x1, y1: 두 번째 벡터
	// - return: 내적 값
	inline int64_t GetInnerProduct(int64_t x0, int64_t y0, int64_t x1, int64_t y1) noexcept;

	// 외적을 계산한다.
	// - x0, y0: 첫 번째 벡터
	// - x1, y1: 두 번째 벡터
	// - return: 외적 값
	inline int64_t GetCrossProduct(int64_t x0, int64_t y0, int64_t x1, int64_t y1) noexcept;

	// Cubic 이징 함수 (In-Out).
	// - x: 입력값 (0 ~ 1024)
	// - return: 이징된 값
	inline int64_t EaseInOutCubic(int64_t x) noexcept;

	// Cubic 이징 함수 (Out).
	// - x: 입력값 (0 ~ 1024)
	// - return: 이징된 값
	inline int64_t EaseOutCubic(int64_t x) noexcept;

	// Circular 이징 함수 (Out).
	// - x: 입력값 (0 ~ 1024)
	// - return: 이징된 값
	inline int64_t EaseOutCirc(int64_t x) noexcept;

	// Sine 이징 함수 (Out).
	// - x: 입력값 (0 ~ 1024)
	// - return: 이징된 값
	inline int64_t EaseOutSine(int64_t x) noexcept;

	// 최대공약수를 계산한다.
	// - a: 첫 번째 수
	// - b: 두 번째 수
	// - return: 최대공약수
	inline int64_t GetGcd(int64_t a, int64_t b) noexcept;

	// 최소공배수를 계산한다.
	// - a: 첫 번째 수
	// - b: 두 번째 수
	// - return: 최소공배수
	inline int64_t GetLcm(int64_t a, int64_t b) noexcept;
}


#include "Common/Math/MathUtilSinTable.h"
#include "Common/Math/MathUtilArcTanTable.h"


namespace MathUtil
{
	inline Angle DegreeToAngle(int deg) noexcept
	{
		return static_cast<Angle>(std::round(deg * (65536.0 / 360.0)));
	}

	inline float AngleToRadian(Angle angle) noexcept
	{
		return (angle * ANGLE_TO_RADIAN);
	}

	inline Angle RadianToAngle(float radian) noexcept
	{
		return static_cast<Angle>(radian * RADIAN_TO_ANGLE);
	}

	inline bool IsZero(float value) noexcept
	{
		if ((value > std::numeric_limits<float>::epsilon()) ||
			(value < -std::numeric_limits<float>::epsilon()))
		{
			return false;
		}

		return true;
	}

	constexpr std::size_t AlignBitCountTo64(std::size_t bits) 
	{
		return (bits + 63) & ~63;
	}

	inline size_t Log2(size_t n) noexcept
	{
		return ((n < 2) ? 1 : 1 + Log2(n >> 1));
	}

	inline int64_t Pow(int64_t v) noexcept
	{
		return (v * v);
	}

	inline int64_t Pow(int64_t v, uint32_t exp) noexcept
	{
		int64_t result = 1;
		while (exp)
		{
			if (exp & 1) 
				result *= v;

			v *= v;
			exp >>= 1;
		}
		return result;
	}

	inline int64_t Div(int64_t a, int64_t b) noexcept
	{
		return (0 == b) ? 0 : (a / b);
	}

	inline int64_t Div1024(int64_t a, int64_t b) noexcept
	{
		return (0 == b) ? 0 : ((a << 10) / b);
	}

	template<typename _Type>
	inline _Type MultiplyBp(_Type a, _Type b) noexcept
	{
		const int64_t result = (static_cast<int64_t>(a) * static_cast<int64_t>(b) + 10'000 - 1) / 10'000;
		return static_cast<_Type>(result);
	}

	inline int64_t Distance(int64_t dx, int64_t dy) noexcept
	{
		dx = std::abs(dx);
		dy = std::abs(dy);

		if (dx > dy)
		{
			std::swap(dx, dy);
		}

		return ((dy << 8) + (dy << 3) - (dy << 4) - (dy << 1) + (dx << 7) - (dx << 5) + (dx << 3) - (dx << 1)) >> 8;
	}

	inline int64_t DistanceSq(int64_t dx, int64_t dy) noexcept
	{
		return Pow(dx) + Pow(dy);
	}

	inline int64_t Sqrt(int64_t number) noexcept
	{
		number = std::abs(number);
		++number;

		int64_t pivot = 1LL << 30;
		int64_t result = 0;

		do
		{
			int64_t tester = result | pivot;
			if (number > (tester * tester))
				result = tester;

			pivot >>= 1;

		} while (pivot > 0LL);

		return(result);
	}

	inline int64_t Sin(Angle angle) noexcept
	{
		if (-16'384 > angle)
		{
			return (-gSinTable[_IDX(angle & 16'383)]);
		}
		else if (0 > angle)
		{
			return (-gSinTable[_IDX(16'383 - (angle & 16'383))]);
		}
		else if (16'384 > angle)
		{
			return gSinTable[_IDX(angle)];
		}
		else
		{
			return gSinTable[_IDX(32'767 - angle)];
		}
	}

	inline int64_t Cos(Angle angle) noexcept
	{
		return Sin(angle + 16'384);
	}

	inline int64_t Tan(Angle angle) noexcept
	{
		return Div(Sin(angle), Cos(angle));
	}

	inline Angle ArcTan(int64_t x, int64_t y) noexcept
	{
		if (0 == x)
		{
			return (0 > y) ? -16'384 : 16'384;
		}

		if (x >= 0)
		{
			if (y >= 0)
			{
				return (x > y) ? gArcTanTable[Div1024(y, x)] : (16'384 - gArcTanTable[Div1024(x, y)]);
			}

			return (x > -y) ? -gArcTanTable[Div1024(-y, x)] : (-16'384 + gArcTanTable[Div1024(x, -y)]);
		}

		if (y >= 0)
		{
			return (-x > y) ? (-32'768 - gArcTanTable[Div1024(y, -x)]) : (16'384 + gArcTanTable[Div1024(-x, y)]);
		}

		return (-x > -y) ? (-32'768 + gArcTanTable[Div1024(-y, -x)]) : (-16'384 - gArcTanTable[Div1024(-x, -y)]);
	}

	inline Angle GetAngleDelta(Angle from, Angle to) noexcept
	{
		// int16_t 오버플로우를 방지하기 위해 int32_t로 계산
		Angle delta = (to - from);
		
		// 최단 각도로 정규화 (-32768 ~ 32767 범위)
		// 예: 350도 -> 10도로 변환 (더 짧은 각도)
		if (delta > static_cast<Angle>(32'767))
			delta -= static_cast<Angle>(65'535);
		else if (delta < static_cast<Angle>(-32'768))
			delta += static_cast<Angle>(65'535);
		
		return delta;
	}

	inline std::pair<int64_t, int64_t> GetVec1024FromAngle(Angle angle) noexcept
	{
		return std::make_pair(Cos(angle), Sin(angle));
	}

	inline std::pair<int64_t, int64_t> GetVec1024(int64_t x, int64_t y) noexcept
	{
		Angle angle = ArcTan(x, y);
		return GetVec1024FromAngle(angle);
	}

	inline int64_t GetInnerProduct(int64_t x0, int64_t y0, int64_t x1, int64_t y1) noexcept
	{
		const int64_t d0 = Distance(x0, y0);
		const int64_t d1 = Distance(x1, y1);

		return (Div1024(x0, d0) * Div1024(x1, d1) + Div1024(y0, d0) * Div1024(y1, d1)) >> 10;
	}

	inline int64_t GetCrossProduct(int64_t x0, int64_t y0, int64_t x1, int64_t y1) noexcept
	{
		return (x0 * y1) - (y0 * x1);
	}

	inline int64_t EaseInOutCubic(int64_t x) noexcept
	{
		if (512 > x)
		{
			return (4 * x * x * x) >> 20;
		}
		else
		{
			int64_t t = (512 - x);
			return 512 - ((t * t * t) >> 18);
		}
	}

	inline int64_t EaseOutCubic(int64_t x) noexcept
	{
		int64_t t = (1024 - x);
		return 1024 - ((t * t * t) >> 20);
	}

	inline int64_t EaseOutCirc(int64_t x) noexcept
	{
		return Sqrt(1024 * 1024 - (x - 1024) * (x - 1024));
	}

	inline int64_t EaseOutSine(int64_t x) noexcept
	{
		return Sin(static_cast<Angle>(x << 4));
	}

	inline int64_t GetGcd(int64_t a, int64_t b) noexcept
	{
		if (a < b) return GetGcd(b, a);

		while (b)
		{
			int64_t c = a % b;
			a = b;
			b = c;
		}
		return a;
	}

	inline int64_t GetLcm(int64_t a, int64_t b) noexcept
	{
		return (a * b) / GetGcd(a, b);
	}
}
