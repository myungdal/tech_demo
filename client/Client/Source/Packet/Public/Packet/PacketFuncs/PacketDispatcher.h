// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PacketDispatcher
// 패킷 타입에 따라 적절한 핸들러로 디스패치하는 템플릿 함수입니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 패킷을 owner의 OnPacket 핸들러로 디스패치
template <typename _Owner, typename _Packet, typename... _Args>
HandleResult DispatchPacketFunc(MAYBE_UNUSED uint64_t socket, _Owner* owner, NetworkPacket& rp, _Args... args)
{
	_Packet* packet = static_cast<_Packet*>(&rp);

	return (owner->OnPacket)(*packet, std::forward<_Args>(args)...);
}
