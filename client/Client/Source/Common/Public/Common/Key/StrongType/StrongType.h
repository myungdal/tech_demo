// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 강타입 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// primitive 타입을 강타입으로 래핑하는 템플릿 클래스.
// - 책임: 타입 안전성 향상 및 formatter 특수화 지원
// - 사용 예: using PacketType = StrongType<int>;
template<typename T>
class StrongType
{
public:
	using PrimitiveType = T;  // 원시 타입

public:
	T value;  // 실제 값

public:
	constexpr StrongType() : value(T{}) {}
	constexpr StrongType(T v) : value(v) {}
	constexpr explicit StrongType(const StrongType& other) : value(other.value) {}
	constexpr explicit StrongType(StrongType&& other) noexcept : value(std::move(other.value)) {}

	constexpr operator T() const { return value; }

	constexpr StrongType& operator=(T v)
	{
		value = v;
		return *this;
	}

	constexpr StrongType& operator=(const StrongType& other)
	{
		value = other.value;
		return *this;
	}

	constexpr StrongType& operator=(StrongType&& other) noexcept
	{
		value = std::move(other.value);
		return *this;
	}

	StrongType& operator++()
	{
		++value;
		return *this;
	}

	StrongType operator++(int)
	{
		StrongType temp = *this;
		++(*this);
		return temp;
	}

	StrongType operator--(int)
	{
		StrongType temp = *this;
		--(*this);
		return temp;
	}

	StrongType& operator--()
	{
		--value;
		return *this;
	}

	StrongType operator+(const StrongType& other) const
	{
		return StrongType(value + other.value);
	}

	bool operator==(const StrongType& other) const
	{
		return value == other.value;
	}

	bool operator!=(const StrongType& other) const
	{
		return value != other.value;
	}

	friend struct std::hash<StrongType>;
};

namespace std
{
	template <typename T>
	struct hash<StrongType<T>>
	{
		std::size_t operator()(const StrongType<T>& value) const
		{
			return static_cast<std::size_t>(value);
		}
	};
}

// 사용 예시
//using PacketType = StrongType<int>;
