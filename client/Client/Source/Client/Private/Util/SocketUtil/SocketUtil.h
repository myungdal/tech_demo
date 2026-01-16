// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "SubSystem/NetworkManager/NetworkManager.h"
#include "SubSystem/NetworkManager/SendBuffer/SendBuffer.h"


class NetworkPacketWriter;

struct SocketUtil
{
	template<typename _Packet>
	class Send : public _Packet
	{
	public:
		template<typename... _Args>
		explicit Send(PacketTraitType trait, _Args&&... args)
			:
			_Packet(trait, FSendBuffer::Pop(), std::forward<_Args>(args)...)
		{
		}

		virtual ~Send()
		{
			UNetworkManager::Get(this)->Send(*this);
		}
	};
};
