// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PacketDataListIterator
// PacketDataList를 순회하기 위한 이터레이터 클래스입니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Container, typename _Item>
class PacketDataListIterator
{
public:
	using Iterator = PacketDataListIterator<_Container, _Item>;

private:
	const _Container* mContainer = nullptr;		// 패킷 컨테이너 포인터
	PacketSize mOffset = 0;						// 현재 오프셋

public:
	PacketDataListIterator(const _Container* container, PacketSize offset)
		:
		mContainer(container),
		mOffset(offset)
	{
	}

	bool operator!=(const Iterator& other) const noexcept
	{
		return (0 != mOffset) || (0 != other.mOffset);
	}

	const _Item* operator*() noexcept
	{
		return reinterpret_cast<const _Item*>(reinterpret_cast<const uint8_t*>(mContainer) + mOffset);
	}

	Iterator operator++() noexcept
	{
		if (0 == mOffset)
			return *this;

		const _Item& item = *reinterpret_cast<const _Item*>(reinterpret_cast<const uint8_t*>(mContainer) + mOffset);
		mOffset = *reinterpret_cast<const PacketSize*>(reinterpret_cast<const uint8_t*>(mContainer) + mOffset + item.GetPacketSize());
		return *this;
	}
};
