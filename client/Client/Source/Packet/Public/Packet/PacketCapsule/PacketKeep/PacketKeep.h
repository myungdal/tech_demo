// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PacketKeep - 영구 보존 콘테이너 (힙 메모리 할당)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _PacketType>
class PacketKeep
{
public:
	using packet_type = typename _PacketType;

private:
	_PacketType* mPacket = nullptr;
	bool mIsRef = false;

public:
	PacketKeep() = default;
	explicit PacketKeep(const _PacketType& packet)
	{
		Clear();

		mPacket = reinterpret_cast<_PacketType*>(_MALLOC<uint8_t>(packet.GetPacketSize()));

		std::copy_n(packet.GetPacketBufPtr(), packet.GetPacketSize(), reinterpret_cast<uint8_t*>(mPacket));
	}
	explicit PacketKeep(const PacketKeep& packetKeep)
	{
		Clear();

		mPacket = packetKeep.mPacket;
		mIsRef = true;
	}
	explicit PacketKeep(PacketKeep&& packetKeep) noexcept
	{
		Clear();

		mPacket = packetKeep.mPacket;
		mIsRef = packetKeep.mIsRef;
		const_cast<PacketKeep&>(packetKeep).mPacket = nullptr;
	}
	explicit PacketKeep(const uint8_t* packetBufPtr)
	{
		Clear();

		const _PacketType& packet = *reinterpret_cast<const _PacketType*>(packetBufPtr);
		mPacket = reinterpret_cast<_PacketType*>(_MALLOC<uint8_t>(packet.GetPacketSize()));

		std::copy_n(packet.GetPacketBufPtr(), packet.GetPacketSize(), reinterpret_cast<uint8_t*>(mPacket));
	}
	~PacketKeep()
	{
		Clear();
	}

public:
	PacketKeep& operator=(const _PacketType& packet)
	{
		Clear();

		mPacket = reinterpret_cast<_PacketType*>(_MALLOC<uint8_t>(packet.GetPacketSize()));

		std::copy_n(packet.GetPacketBufPtr(), packet.GetPacketSize(), reinterpret_cast<uint8_t*>(mPacket));

		return *this;
	}
	PacketKeep& operator=(const PacketKeep& packetKeep)
	{
		Clear();

		mPacket = packetKeep.mPacket;
		mIsRef = true;

		return *this;
	}
	PacketKeep& operator=(PacketKeep&& packetKeep) noexcept
	{
		Clear();

		mPacket = packetKeep.mPacket;
		mIsRef = packetKeep.mIsRef;
		const_cast<PacketKeep&>(packetKeep).mPacket = nullptr;

		return *this;
	}
	PacketKeep& operator=(const uint8_t* packetBufPtr)
	{
		Clear();

		const _PacketType& packet = *reinterpret_cast<const _PacketType*>(packetBufPtr);
		mPacket = reinterpret_cast<_PacketType*>(_MALLOC<uint8_t>(packet.GetPacketSize()));

		std::copy_n(packet.GetPacketBufPtr(), packet.GetPacketSize(), reinterpret_cast<uint8_t*>(mPacket));

		return *this;
	}

public:
	void Clear()
	{
		if ((nullptr != mPacket) &&
			(false == mIsRef))
		{
			_FREE(mPacket);
			mPacket = nullptr;
		}

		mIsRef = false;
	}

	bool IsEmpty() const noexcept { return (nullptr == mPacket) ? true : false; }

	_PacketType* operator*() noexcept { return mPacket; }
	const _PacketType* operator*() const noexcept { return mPacket; }

	_PacketType* operator->() noexcept { return mPacket; }
	const _PacketType* operator->() const noexcept { return mPacket; }
};
