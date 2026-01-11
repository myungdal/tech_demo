// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import BotScene;
import BotUtil;
import ServerEngineCore;
import ServerEngineNetwork;
import ServerEngineWorker;
import ServerEngineUtil;
import ServerEngineCommon;

#include "BotSceneGame.h"

// BotContext.h contains GameMap typedef - must include directly
#include "Bot/Bot/BotContext.h"


BotSceneGame::BotSceneGame(BotPtr& bot) 
	: 
	BotScene(bot)
{	
	RegisterSeq(this, &BotSceneGame::CD_REQ_CHEAT_CREATE_ITEM_GAME);
	RegisterSeq(this, &BotSceneGame::CM_REQ_GAME_LIST);
	RegisterSeq(this, &BotSceneGame::CD_REQ_GAME_CREATE);
	RegisterSeq(this, &BotSceneGame::CD_REQ_GAME_USER_ENTER);
	RegisterSeq(this, &BotSceneGame::CF_REQ_GAME_CHANNEL_USER_ENTER);
	RegisterSeq(this, &BotSceneGame::CD_REQ_GAME_USER_LEAVE);
	NextSeq();
}
void BotSceneGame::OnLostConnection()
{
	_DEBUG_LOG(WHITE, L"[{}][{}]", GetRawSocket(), GetBotId());

	TryChangeToReconnectScene();
}

bool BotSceneGame::OnDispatchPacket(NetworkPacket& rp)
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

HandleResult BotSceneGame::OnPacket(MAYBE_UNUSED NetworkPacket& rp)
{
	HandleResult handleResult = DispatchPacketToParent<BotScene>(GetRawSocket(), this, rp);
	if (handleResult == HandleResult::NOT_EXISTS)
		_DEBUG_RED;
	return handleResult;
}

// '게임 시작' 아이템 생성 치트 요청
void BotSceneGame::CD_REQ_CHEAT_CREATE_ITEM_GAME()
{
	// 존재하지 않는 아이템
	const UserItemRowPtr& itemRow = GetBotContext().FindItemIdByItemSid(1);
	if (!itemRow)
	{
		NextSeq();
		return;
	}

	// 이미 '게임 시작' 아이템이 생성되어 있으면 다음 단계로 진행
	const ITEM& item = itemRow->Data();
	const ItemId itemId = item.Get_c_item_id();
	if (INVALID_UUID != itemId)
	{
		NextSeq();
		return;
	}

	// 50% 확률로 패스
	//if (0 == ((*tRandomEngine32)() & 1))
	//{
	//	NextSeq();
	//	return;
	//}

	BotSocketUtil::SendToFront<CD_REQ_CHEAT::Writer> wp(*this, REQ);
	wp.SetHeader(GetBotContext().CurrPacketHeader());
	wp.SetValues(
		L"CreateItem 1 1"
	);
}

HandleResult BotSceneGame::OnPacket(DC_ACK_CHEAT& rp)
{	
	// BotScene 에서 처리한다
	return BotScene::OnPacket(rp);
}

// 게임 목록 요청
void BotSceneGame::CM_REQ_GAME_LIST()
{
	GameMap& gameMap = GetBotContext().GetGameMap();
	gameMap.clear();

	BotSocketUtil::SendToFront<CM_REQ_GAME_LIST_OVER::Writer> wp(*this, REQ);
	wp.SetHeader(GetBotContext().CurrPacketHeader());
	wp.SetValues(
		INVALID_UUID
	);
}

HandleResult BotSceneGame::OnPacket(MC_ACK_GAME_LIST_OVER& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();
	ReportResultAndYield(result);

	switch (result)
	{
	case Result::SUCCEEDED:
	{
		GameMap& gameMap = GetBotContext().GetGameMap();
		for (const GAME* game : rp.Get_gameList())
			gameMap.emplace(game->Get_c_game_id(), *game);
		NextSeq();
		break;
	}
	case Result::RETRY_LATER:
		RetrySeq();
		break;
	default:
		_DEBUG_LOG(RED, L"{}", result);		
		NextSeq();
	}

	return HandleResult::OK;
}

// 게임 시작 요청
void BotSceneGame::CD_REQ_GAME_CREATE()
{
	// '게임 시작' 아이템이 없으면(생성 실패/미보유) 게임 생성 단계를 건너뜀
	const UserItemRowPtr& itemRow = GetBotContext().FindItemIdByItemSid(1);
	if (!itemRow)
	{
		NextSeq();
		return;
	}

	// '게임 시작' 아이템이 아직 생성되지 않았으면 게임 생성 단계를 건너뜀
	const ITEM& item = itemRow->Data();
	const ItemId itemId = item.Get_c_item_id();	
	if (INVALID_UUID == itemId)
	{
		NextSeq();
		return;
	}
		
	BotSocketUtil::SendToFront<CD_REQ_GAME_CREATE::Writer> wp(*this, REQ);
	wp.SetHeader(GetBotContext().CurrPacketHeader());
	wp.SetValues(
		itemId
	);
}

HandleResult BotSceneGame::OnPacket(DC_ACK_GAME_CREATE& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();
	ReportResultAndYield(result);

	switch (result)
	{
	case Result::SUCCEEDED:
	{
		BotScene::CacheUserAssetDelta(rp.Get_userCacheDiff());

		GameMap& gameMap = GetBotContext().GetGameMap();
		gameMap.clear();
		gameMap.emplace(rp.Get_game().Get_c_game_id(), rp.Get_game());

		NextSeq();
		break;
	}
	case Result::RETRY_LATER:
		if ((*tRandomEngine32)(0, 1) == 0)
			RetrySeq();
		else
			NextSeq();
		break;
	default:
		_DEBUG_LOG(RED, L"{}", result);
		NextSeq();
	}

	return HandleResult::OK;
}

// 게임 입장 요청
void BotSceneGame::CD_REQ_GAME_USER_ENTER()
{
	GameMap& gameMap = GetBotContext().GetGameMap();
	if (gameMap.empty())
	{
		JumpToSeq(this, &BotSceneGame::CD_REQ_CHEAT_CREATE_ITEM_GAME);
		return;
	}

	auto& [gameId, game] = *gameMap.begin();

	BotSocketUtil::SendToFront<CD_REQ_GAME_USER_ENTER::Writer> wp(*this, REQ);
	wp.SetHeader(GetBotContext().CurrPacketHeader());
	wp.SetValues(
		gameId
	);
}

HandleResult BotSceneGame::OnPacket(DC_ACK_GAME_USER_ENTER& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();
	ReportResultAndYield(result);

	switch (result)
	{
	case Result::SUCCEEDED:
		NextSeq();
		break;
	case Result::RETRY_LATER:
		RetrySeq();
		break;
	default:
		_DEBUG_LOG(RED, L"{}", result);		
		NextSeq();
	}

	return HandleResult::OK;
}

// 게임 입장 요청
void BotSceneGame::CF_REQ_GAME_CHANNEL_USER_ENTER()
{
	BotSocketUtil::SendToFront<CF_REQ_GAME_CHANNEL_USER_ENTER::Writer> wp(*this, REQ);
	wp.SetHeader(GetBotContext().CurrPacketHeader());
}

HandleResult BotSceneGame::OnPacket(FC_ACK_GAME_CHANNEL_USER_ENTER& rp)
{
	const Result result = rp.GetHeader().GetPacketResult();

	ReportResultAndYield(result);

	switch (result)
	{
	case Result::SUCCEEDED:
		NextSeq();
		break;
	case Result::RETRY_LATER:
		RetrySeq();
		break;
	default:
		_DEBUG_LOG(RED, L"{}", result);
		NextSeq();
	}

	return HandleResult::OK;
}

// 게임 퇴장 요청
void BotSceneGame::CD_REQ_GAME_USER_LEAVE()
{
	BotSocketUtil::SendToFront<CD_REQ_GAME_USER_LEAVE::Writer> wp(*this, REQ);
	wp.SetHeader(GetBotContext().CurrPacketHeader());
}

HandleResult BotSceneGame::OnPacket(MAYBE_UNUSED DC_ACK_GAME_USER_LEAVE& rp)
{
	NextSeq();
	return HandleResult::OK;
}

