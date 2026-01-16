// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 직렬화 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Md5;

// 바이너리 데이터 직렬화를 담당하는 클래스.
// - 책임: 다양한 타입의 값을 바이너리 버퍼에 직렬화
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
class COMMON_API Serializer final
{
private:
	static constexpr size_t DEFAULT_BUF_SIZE = 1024 * 1024;  // 기본 버퍼 크기 (1MB)
	std::vector<uint8_t> mBuf;                                // 직렬화 버퍼
	size_t mBufSize = 0;                                      // 버퍼 크기
	size_t mOffset = 0;                                       // 현재 쓰기 위치

public:
	Serializer();

	// 직렬화 위치를 초기화한다.
	void Reset() { mOffset = 0; }

	// 값을 버퍼에 쓴다.
	// - value: 쓸 값
	template <typename _Type>
	void WriteValue(const _Type& value)
	{
		Reserve(sizeof(_Type));
		*reinterpret_cast<_Type*>(mBuf.data() + mOffset) = value;
		mOffset += sizeof(_Type);
	}
	
	// 바이너리 데이터를 버퍼에 쓴다.
	// - data: 쓸 데이터 포인터
	// - size: 데이터 크기 (bytes)
	void WriteBinary(const uint8_t* data, size_t size);

	// StringKey를 버퍼에 쓴다.
	// - key: 쓸 StringKey
	void WriteStringKey(const StringKey& key);

private:
	// 버퍼 크기를 확보한다.
	// - size: 필요한 크기 (bytes)
	void Reserve(size_t size);

public:
	// 직렬화된 데이터 크기를 반환한다.
	// - return: 데이터 크기 (bytes)
	size_t GetDataSize() const { return mOffset; }

	// 버퍼 포인터를 반환한다.
	// - return: 버퍼 시작 주소
	const uint8_t* GetBufPtr() const { return mBuf.data(); }

public:
	// 체크섬을 계산한다.
	// - c0, c1, c2, c3: 체크섬 값 (출력 파라미터)
	void GetChecksum(OUT uint32_t& c0, OUT uint32_t& c1, OUT uint32_t& c2, OUT uint32_t& c3) const;

	// 체크섬을 문자열로 반환한다.
	// - return: 체크섬 문자열
	std::wstring MakeChecksumStr() const;
};
