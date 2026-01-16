// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketBase/SocketBase.h"


class NetworkPacketWriter;

struct SocketUtil
{
	template<typename _Packet, EncryptionOption _EncryptionOption = EncryptionOption::ENCRYPTION>
	class Send : public _Packet
	{
	private:
		SocketBase& mSocket;

	public:
		template<typename... _Args>
		explicit Send(SocketBase& socket, PacketTraitType trait, _Args&&... args)
			:
			mSocket(socket),
			_Packet(trait, SendBuffer::Pop(__FUNCTIONW__), std::forward<_Args>(args)...)
		{
		}

		template<typename... _Args>
		explicit Send(SocketBase& socket)
			:
			mSocket(socket),
			_Packet(SendBuffer::Pop(__FUNCTIONW__))
		{
		}

		virtual ~Send()
		{
			mSocket.Send(*this, _EncryptionOption);
		}
	};

	template<typename _Packet>
	class Request : public _Packet
	{
	private:
		SocketBase& mSocket;

	public:
		template<typename... _Args>
		explicit Request(SocketBase& socket, PacketTraitType trait, _Args&&... args)
			:
			mSocket(socket),
			_Packet(trait, SendBuffer::Pop(__FUNCTIONW__), std::forward<_Args>(args)...)
		{
		}

		virtual ~Request()
		{
		}

		//bool Wait()
		//{
		//	std::promise<NetworkPacket*> promise;
		//	std::future<NetworkPacket*> future = promise.get_future();
		//	RpcId remoteRequestId = mSocket.MakeRemoteRequestId(std::move(promise));
		//	this->GetCurrPacketHeader().SetRemoteRequestId(gMyAppType, remoteRequestId);
		//	mSocket.Send(*this);

		//	std::chrono::milliseconds span(100'000);
		//	if (future.wait_for(span) == std::future_status::timeout)
		//	{
		//		if (std::promise<NetworkPacket*>* remoteRequest = mSocket.GetRemoteRequest(remoteRequestId))
		//		{
		//			remoteRequest->set_value(nullptr);
		//		}

		//		return false;
		//	}

		//	return true;
		//}

		template<typename _PacketType>
		bool Wait(class PacketHeader& reqHeader, OUT _PacketType*& ack)
		{
			std::promise<NetworkPacket*> promise;
			std::future<NetworkPacket*> future = promise.get_future();
			RpcId remoteRequestId = mSocket.MakeRemoteRequestId(std::move(promise));
			this->GetHeader().SetRemoteRequestId(gMyAppType, remoteRequestId);
			mSocket.Send(*this);

#if defined(_DEBUG)
			std::chrono::milliseconds span(5'000);
#else
			std::chrono::milliseconds span(5'000);
#endif

			if (future.wait_for(span) == std::future_status::timeout)
			{ 
				if (std::promise<NetworkPacket*>* remoteRequest = mSocket.GetRemoteRequest(remoteRequestId))
				{
					remoteRequest->set_value(nullptr);
				}
				
				ack = nullptr;
				return false;
			}

			ack = static_cast<_PacketType*>(future.get());

			reqHeader = ack->GetHeader();

			return true;
		}
	};

	template<typename _BypassPacket, typename _Packet>
	class SendBypass : public _Packet
	{
	private:
		SocketBase& mSocket;

	public:
		template<typename... _Args>
		explicit SendBypass(SocketBase& socket, PacketTraitType trait, _Args&&... args)
			:
			mSocket(socket),
			_Packet(trait, TEMP_BUF, std::forward<_Args>(args)...)
		{
		}

		virtual ~SendBypass()
		{
			_BypassPacket wp(ACK, SendBuffer::Pop(__FUNCTIONW__), *this, Result::SUCCEEDED);
			wp.SetValues(
				this->GetPacketBufPtr(),
				this->GetPacketSize()
			);
			mSocket.SendBypass(wp, *this);
		}
	};

	template<typename _BypassPacket, typename _Packet>
	class SendBypassToUser : public _Packet
	{
	private:
		SocketBase& mSocket;

	public:
		template<typename... _Args>
		explicit SendBypassToUser(SocketBase& socket, PacketTraitType trait, _Args&&... args)
			:
			mSocket(socket),
			_Packet(trait, TEMP_BUF, std::forward<_Args>(args)...)
		{
		}

		virtual ~SendBypassToUser()
		{
			_BypassPacket wp(NOTIFY, SendBuffer::Pop(__FUNCTIONW__));
			wp.SetValues(
				&this->ConstGetHeader().GetUserId(),
				1,
				this->GetPacketBufPtr(),
				this->GetPacketSize()
			);
			mSocket.SendBypass(wp, *this);
		}
	};
};
