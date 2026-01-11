// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class NetworkPacketWriter;
class BotScene;

// 봇 소켓 유틸리티
// - SendToFront: 패킷을 FrontServer로 전송하는 RAII 래퍼
struct BotSocketUtil
{
	template<typename _Packet>
	class SendToFront : public _Packet
	{
	private:
		BotScene& mBotScene;

	public:
		template<typename... _Args>
		explicit SendToFront(BotScene& botScene, PacketTraitType trait, _Args&&... args)
			:
			mBotScene(botScene),
			_Packet(trait, SendBuffer::Pop(__FUNCTIONW__), std::forward<_Args>(args)...)
		{
		}

		template<typename... _Args>
		explicit SendToFront(BotScene& botScene)
			:
			mBotScene(botScene),
			_Packet(SendBuffer::Pop(__FUNCTIONW__))
		{
		}

		virtual ~SendToFront()
		{
			auto socket = mBotScene.GetSocket();
			if (socket.IsNotNull())
			{
				mBotScene.ReportSend(this->GetPacketType());

				socket->Send(*this);
			}
		}
	};
};
