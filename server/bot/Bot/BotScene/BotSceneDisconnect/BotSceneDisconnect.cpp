// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;

#include "BotSceneDisconnect.h"


BotSceneDisconnect::BotSceneDisconnect(BotPtr& bot) 
	: 
	BotScene(bot)
{
	RegisterSeq(this, &BotSceneDisconnect::DISCONNECT);
	NextSeq();
}
void BotSceneDisconnect::OnHandshakeCompleted(MAYBE_UNUSED bool reconnected)
{
	_DEBUG_LOG(WHITE, L"[{}][{}]", GetRawSocket(), GetBotId());
	NextSeq();
}

void BotSceneDisconnect::OnLostConnection()
{
	_DEBUG_LOG(WHITE, L"[{}][{}]", GetRawSocket(), GetBotId());
	NextSeq();
}

bool BotSceneDisconnect::OnDispatchPacket(NetworkPacket& rp)
{
	HandleResult handleResult = DispatchPacket(GetRawSocket(), this, rp);

	if (handleResult == HandleResult::NOT_EXISTS)
	{
		_DEBUG_RED;
		return true;
	}

	return (handleResult == HandleResult::OK) ? true : false;
}

HandleResult BotSceneDisconnect::OnPacket(MAYBE_UNUSED NetworkPacket& rp)
{
	HandleResult handleResult = DispatchPacketToParent<BotScene>(GetRawSocket(), this, rp);

	if (handleResult == HandleResult::NOT_EXISTS)
		_DEBUG_RED;

	return handleResult;
}

void BotSceneDisconnect::DISCONNECT()
{
	if (true == IsConnectedWithFront())
	{
		SetEnableReconnect(false);
		DisconnectFromFront();
	}
}

