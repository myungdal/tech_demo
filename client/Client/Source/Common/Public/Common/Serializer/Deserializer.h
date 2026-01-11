// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 역직렬화 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class StringKey;
class StringKeyPool;

// 바이너리 데이터 역직렬화를 담당하는 클래스.
// - 책임: 바이너리 버퍼에서 다양한 타입의 값을 역직렬화
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
class COMMON_API Deserializer final
{
private:
	const uint8_t* mData = nullptr;  // 역직렬화할 데이터 포인터
	const size_t mDataSize = 0;       // 데이터 크기 (bytes)
	size_t mOffset = 0;               // 현재 읽기 위치

public:
	Deserializer(const uint8_t* data, size_t size);

	// 버퍼에서 값을 읽는다.
	// - return: 읽은 값의 참조
	template <typename _Type>
	const _Type& ReadValue()
	{
		_ASSERT_CRASH((mOffset + sizeof(_Type)) <= mDataSize);
		const _Type& value = *(reinterpret_cast<const _Type*>(mData + mOffset));
		mOffset += sizeof(_Type);
		return value;
	}

	// 바이너리 데이터를 읽는다.
	// - dest: 저장할 버퍼 포인터
	// - size: 읽을 크기 (bytes)
	// - return: 성공 시 true
	bool ReadBinary(uint8_t* dest, size_t size);

	// StringKey를 읽는다.
	// - pool: StringKeyPool 참조
	// - return: 읽은 StringKey
	const StringKey ReadStringKey(StringKeyPool& pool);

public:
	// 남은 데이터 크기를 반환한다.
	// - return: 남은 크기 (bytes)
	size_t GetRemainedSize() const { return mDataSize - mOffset; }
};
