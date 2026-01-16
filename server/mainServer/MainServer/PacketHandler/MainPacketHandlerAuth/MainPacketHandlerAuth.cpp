// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;

#include "MainServer/PacketHandler/MainPacketHandlerAuth/Transactor_CM_REQ_ACCOUNT_USER_CREATE.h"
#include "MainServer/PacketHandler/MainPacketHandlerAuth/Transactor_CM_REQ_ACCOUNT_USER_DELETE.h"
#include "MainServer/PacketHandler/MainPacketHandlerAuth/Transactor_CM_REQ_ACCOUNT_USER_LIST.h"
#include "MainServer/PacketHandler/MainPacketHandlerAuth/Transactor_CM_REQ_AUTH_MODIFY.h"
#include "MainServer/PacketHandler/MainPacketHandlerAuth/Transactor_CM_REQ_AUTH_TICKET.h"
#include "MainServer/PacketHandler/MainPacketHandlerAuth/Transactor_CM_REQ_SERVER_LIST.h"

// 서버 목록 요청 받음.
HandleResult MainPacketHandlerAuth::OnPacket(class CM_REQ_SERVER_LIST& rp, MAYBE_UNUSED SocketMainFromFront& socket)
{
	_ASSERT_CRASH(tWorkingWorker == gServerListManager);

	Transactor_CM_REQ_SERVER_LIST transactor(socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

// 계정의 유저 목록 요청 받음.
HandleResult MainPacketHandlerAuth::OnPacket(MAYBE_UNUSED CM_REQ_ACCOUNT_USER_LIST& rp, MAYBE_UNUSED SocketMainFromFront& socket)
{
	_ASSERT_CRASH(tWorkingWorker == gAccountManager);

	Transactor_CM_REQ_ACCOUNT_USER_LIST transactor(socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

HandleResult MainPacketHandlerAuth::OnPacket(MAYBE_UNUSED CM_REQ_ACCOUNT_USER_CREATE& rp, MAYBE_UNUSED SocketMainFromFront& socket)
{
	_ASSERT_CRASH(tWorkingWorker == gAccountManager);

	Transactor_CM_REQ_ACCOUNT_USER_CREATE transactor(socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

HandleResult MainPacketHandlerAuth::OnPacket(MAYBE_UNUSED CM_REQ_ACCOUNT_USER_DELETE& rp, MAYBE_UNUSED SocketMainFromFront& socket)
{
	_ASSERT_CRASH(tWorkingWorker == gAccountManager);

	Transactor_CM_REQ_ACCOUNT_USER_DELETE transactor(socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

// 인증 티켓 요청 받음.
HandleResult MainPacketHandlerAuth::OnPacket(MAYBE_UNUSED CM_REQ_AUTH_TICKET& rp, MAYBE_UNUSED SocketMainFromFront& socket)
{
	_ASSERT_CRASH(tWorkingWorker == gAccountManager);
	
	Transactor_CM_REQ_AUTH_TICKET transactor(socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

HandleResult MainPacketHandlerAuth::OnPacket(MAYBE_UNUSED CM_REQ_AUTH_MODIFY& rp, MAYBE_UNUSED SocketMainFromFront& socket)
{
	_ASSERT_CRASH(tWorkingWorker == gAccountManager);

	Transactor_CM_REQ_AUTH_MODIFY transactor(socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

