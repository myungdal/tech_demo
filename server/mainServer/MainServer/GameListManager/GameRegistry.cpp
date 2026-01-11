// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;

// [WHY] Game, GameRegistry는 import MainServer를 통해 모듈에서 이미 export됨


Game* GameRegistry::CreateGame(const GAME& game)
{
	const GameId gameId = game.Get_c_game_id();
	if (mGameMap.contains(gameId))
		return nullptr;

	// 홈 게임인 경우 서버당 하나만 존재해야 함
	if (game.Get_c_user_id() == INVALID_UUID)
	{
		if (FindServerHomeGame(game.Get_c_server_id()))
			return nullptr;
	}

	GamePtr gamePtr = std::make_shared<Game>();
	gamePtr->SetGame(game);
	mGameMap.emplace(gameId, gamePtr);

	return gamePtr.get();
}

bool GameRegistry::RemoveGame(GameId gameId)
{
	if (mGameMap.erase(gameId) == 0)
		return false;

	return true;
}

Game* GameRegistry::FindGame(GameId gameId)
{
	if (gameId == INVALID_UUID)
		return nullptr;

	auto it = mGameMap.find(gameId);
	if (it == mGameMap.end())
		return nullptr;

	return (it->second).get();
}

Game* GameRegistry::FindServerHomeGame(ServerId serverId)
{
	for (auto [gameId, gamePtr] : mGameMap)
	{
		GAME& game = gamePtr->GetGame();
		if ((game.Get_c_user_id() == INVALID_UUID) &&
			(game.Get_c_server_id() == serverId))
		{
			return gamePtr.get();
		}
	}

	return nullptr;
}

bool GameRegistry::AddUserToGame(UserId userId, GameId gameId)
{
	Game* game = FindGame(gameId);
	if (!game)
		return false;

	return game->AddUser(userId);
}

bool GameRegistry::RemoveUserFromGame(UserId userId, GameId gameId)
{
	Game* game = FindGame(gameId);
	if (!game)
		return false;

	return game->RemoveUser(userId);
}
