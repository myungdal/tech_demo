// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;

#include "BotSceneItem.h"


BotSceneItem::BotSceneItem(BotPtr& bot) 
	: 
	BotScene(bot)
{
	NextSeq();
}
void BotSceneItem::OnLostConnection()
{
	_DEBUG_LOG(WHITE, L"[{}][{}]", GetRawSocket(), GetBotId());
	TryChangeToReconnectScene();
}

bool BotSceneItem::OnDispatchPacket(NetworkPacket& rp)
{
	HandleResult handleResult = DispatchPacket(GetRawSocket(), this, rp);
	if (handleResult == HandleResult::NOT_EXISTS)
	{
		_DEBUG_RED;
		return true;
	}
	return (handleResult == HandleResult::OK) ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HandleResult BotSceneItem::OnPacket(MAYBE_UNUSED NetworkPacket& rp)
{
	HandleResult handleResult = DispatchPacketToParent<BotScene>(GetRawSocket(), this, rp);
	if (handleResult == HandleResult::NOT_EXISTS)
	{
		_DEBUG_RED;
	}
	return handleResult;
}

