// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NetworkPacketWriter
// NetworkPacket 버퍼에 데이터를 직렬화하여 쓰는 클래스입니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class NetworkPacket;
class PacketHeader;

class PACKET_API NetworkPacketWriter
{
private:
	NetworkPacket* mPacketPtr = nullptr;	// 패킷 포인터

	PacketType& mPacketType;				// 패킷 타입 참조
	PacketSize& mPacketSize;				// 패킷 크기 참조
	PacketTraitType& mPacketTrait;			// 패킷 특성 참조
	PacketHeader& mPacketHeader;			// 패킷 헤더 참조

public:
	DISABLE_COPY(NetworkPacketWriter);

	// 새 패킷 생성 시 사용하는 생성자
	NetworkPacketWriter(PacketTraitType trait, PacketType packetType, uint8_t* buf);

	// 기존 패킷 버퍼를 래핑하는 생성자
	explicit NetworkPacketWriter(uint8_t* buf);

	// 값 직렬화
	template <typename _Type>
	bool WriteValue(const _Type& value)
	{
		if (MAX_PACKET_BUFFER_SIZE < (mPacketSize + sizeof(_Type)))
			return false;

		*reinterpret_cast<_Type*>(reinterpret_cast<uint8_t*>(mPacketPtr) + mPacketSize) = value;
		mPacketSize += sizeof(_Type);

		return true;
	}

	// 문자열 직렬화
	bool WriteString(const wchar_t* str, PacketSize len) const;

	// 바이너리 데이터 직렬화
	bool WriteBinary(const uint8_t* bin, PacketSize size) const;

	// 패킷 버퍼 접근자
	uint8_t* GetPacketBufPtr() const noexcept { return reinterpret_cast<uint8_t*>(mPacketPtr); }
	uint8_t* GetPacketBufWritingPtr() const noexcept { return (reinterpret_cast<uint8_t*>(mPacketPtr) + mPacketSize); }
	NetworkPacket* GetNetworkPacketPtr() const noexcept { return mPacketPtr; }

	// 패킷 타입 접근자
	PacketType GetPacketType() const noexcept { return mPacketType; }
	void SetPacketType(PacketType packetType) const noexcept { mPacketType = packetType; }

	// 패킷 크기 접근자
	PacketSize GetPacketSize() const noexcept { return mPacketSize; }
	void SetPacketSize(PacketSize packetSize) const noexcept { mPacketSize = packetSize; }

	// 패킷 특성 반환
	PacketTraitType GetPacketTrait() const noexcept { return mPacketTrait; }

	// 패킷 헤더 접근자
	PacketHeader& GetHeader() { return mPacketHeader; }
	const PacketHeader& ConstGetHeader() const { return mPacketHeader; }
	void SetHeader(const PacketHeader& val);

protected:
	// 읽기/쓰기 패킷에서 헤더 복사
	void SetReadPacket(const NetworkPacket& pb);
	void SetWritePacket(const NetworkPacketWriter& wp);

	static constexpr PacketSize MAX_STRING_LENGTH = (MAX_PACKET_BUFFER_SIZE >> 1);	// 최대 문자열 길이
};
