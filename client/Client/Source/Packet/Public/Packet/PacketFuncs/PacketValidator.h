// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PacketValidator
// 패킷의 유효성을 검증하는 템플릿 함수입니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 패킷 데이터의 무결성 검증
template <typename _Packet>
bool ValidatePacketFunc(const Packet& rp)
{
	const _Packet* packet = static_cast<const _Packet*>(&rp);
	size_t validateSize = 0;
	if (false == packet->Validate(validateSize))
	{
		_DEBUG_BREAK;

#if defined(UE_EDITOR) || defined(_DEBUG)
		// 브레이크포인트 걸린상태에서 따라가보기
		packet->Validate(validateSize);
#endif
		return false;
	}

	return true;
}
