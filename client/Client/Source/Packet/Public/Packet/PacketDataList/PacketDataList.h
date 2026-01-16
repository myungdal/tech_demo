// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/PacketDataList/PacketDataListIterator.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PacketDataList
// 패킷 내 가변 길이 데이터 리스트를 순회하기 위한 컨테이너 래퍼입니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Container, typename _Item>
class PacketDataList
{
public:
	using Iterator = PacketDataListIterator<_Container, _Item>;

private:
	const _Container* mContainer = nullptr;		// 패킷 컨테이너 포인터
	PacketSize mOffset = 0;						// 리스트 시작 오프셋

public:
	PacketDataList(const _Container* container, PacketSize offset)
		:
		mContainer(container),
		mOffset(offset)
	{
	}

	// 리스트 순회를 위한 이터레이터 반환
	Iterator begin()
	{
		return Iterator(mContainer, mOffset);
	}

	Iterator end()
	{
		return Iterator(mContainer, 0);
	}

	// 리스트가 비어있는지 확인
	bool empty() noexcept
	{
		if (nullptr == mContainer) {
			return true;
		}

		return (mOffset == 0) ? true : false;
	}
};
