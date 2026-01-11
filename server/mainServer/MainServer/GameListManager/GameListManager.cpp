// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;
import ServerEngineDb;
// [WHY] Game, GameRegistry는 import MainServer를 통해 모듈에서 이미 export됨


GameListManager::GameListManager()
{
	mGameRegistry = std::make_shared<GameRegistry>();
}

bool GameListManager::Load()
{
	const bool loadGameList = LoadGameList();
	if (!loadGameList)
		return false;

	const bool loadUserGameList = LoadUserGameList();
	if (!loadUserGameList)
		return false;

	// homeGame 이 존재하지 않을 경우 생성한다.
	ServerHomeGameList serverHomeGameList;
	gServerListManager->GetServerHomeGameList(OUT serverHomeGameList);
	for (auto& [serverId, homeGameSid] : serverHomeGameList)
	{
		Game* game = mGameRegistry->FindServerHomeGame(serverId);
		if (game != nullptr)
			continue;

		WorkerManager::RunAsyncForced(shared_from_this(), this, &GameListManager::CreateGame, serverId, INVALID_UUID, homeGameSid);
	}

	return true;
}

bool GameListManager::LoadGameList()
{
	MainDbSession mainDbSession(CommitType::AUTO);
	if (false == mainDbSession.IsReady())
		return false;

	SP_GAME_SELECT_ALL sp(mainDbSession);
	Result result = DbUtil::FetchSp(sp,
		[&]()
		{
			GAME::Writer wp = DbUtil::SpToPacket(sp);

			Game* game = mGameRegistry->CreateGame(wp.GetPacket());
			if (!game)
				return Result::CACHE_ERROR;

			return Result::SUCCEEDED;
		}
	);
	if (Result::SUCCEEDED != result)
		return false;

	return true;
}

bool GameListManager::LoadUserGameList()
{
	MainDbSession mainDbSession(CommitType::AUTO);
	if (false == mainDbSession.IsReady())
		return false;

	SP_ACCOUNT_USER_GAME_SELECT_ALL sp(mainDbSession);
	Result result = DbUtil::FetchSp(sp,
		[&]()
		{
			ACCOUNT_USER_GAME::Writer wp = DbUtil::SpToPacket(sp);

			UserId userId = wp.GetPacket().Get_c_user_id();
			GameId gameId = wp.GetPacket().Get_c_game_id();

			mUserGameMap.insert_or_assign(userId, gameId);

			return Result::SUCCEEDED;
		}
	);
	if (Result::SUCCEEDED != result)
		return false;

	return true;
}

Game* GameListManager::CreateGame(ServerId serverId, UserId userId, GameSid gameSid)
{	
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	MainDbSession mainDbSession(CommitType::MANUAL);
	if (false == mainDbSession.IsReady())
		return nullptr;

	Result result = Result::SUCCEEDED;

	do
	{
		GameId gameId = UuidUtil::GenerateUuid();

		SP_GAME_INSERT spGameInsert(mainDbSession,
			gameId,
			serverId,
			userId,
			gameSid,
			tClock.GetGlobalNowTs()
		);
		result = DbUtil::ExecuteSp(spGameInsert);
		if (Result::SUCCEEDED != result)
			break;

		Game* game = nullptr;

		SP_GAME_SELECT spGameSelect(mainDbSession,
			gameId
		);
		result = DbUtil::FetchSp(spGameSelect,
			[&]()
			{
				GAME::Writer wp = DbUtil::SpToPacket(spGameSelect);

				game = mGameRegistry->CreateGame(wp.GetPacket());
				if (!game)
					return Result::CACHE_ERROR;

				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			break;

		if (false == mainDbSession.Commit())
		{
			result = Result::DB_ERROR;
			break;
		}

		return game;

	} while (false);

	mainDbSession.Rollback();
	return nullptr;
}

bool GameListManager::RemoveGame(GameId gameId)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	Game* game = mGameRegistry->FindGame(gameId);
	if (!game)
		return false;

	MainDbSession mainDbSession(CommitType::MANUAL);
	if (false == mainDbSession.IsReady())
		return false;

	Result result = Result::SUCCEEDED;

	do
	{
		SP_GAME_DELETE spGameDelete(mainDbSession,
			UuidUtil::GenerateUuid(),
			gameId,
			tClock.GetGlobalNowTs()
		);
		result = DbUtil::ExecuteSp(spGameDelete);
		if (Result::SUCCEEDED != result)
			break;

		for (UserId userId : game->GetUserIdList())
		{
			SP_ACCOUNT_USER_GAME_UPDATE spAccountUserGameUpdate(mainDbSession,
				userId,
				INVALID_UUID,
				tClock.GetGlobalNowTs()
			);
			result = DbUtil::ExecuteSp(spAccountUserGameUpdate);
			if (Result::SUCCEEDED != result)
				break;
		}

		if (Result::SUCCEEDED != result)
			break;

		if (false == mainDbSession.Commit())
		{
			result = Result::DB_ERROR;
			break;
		}

		for (UserId userId : game->GetUserIdList())
		{
			auto it = mUserGameMap.find(userId);
			if (it == mUserGameMap.end())
			{
				_ASSERT_CRASH(false);
				continue;
			}

			mUserGameMap.erase(it);
		}

		mGameRegistry->RemoveGame(gameId);

	} while (false);

	if (Result::SUCCEEDED != result)
	{
		mainDbSession.Rollback();
		return false;
	}

	return true;
}

bool GameListManager::IsExistGame(GameId gameId) const
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	return (mGameRegistry->FindGame(gameId) != nullptr);
}

bool GameListManager::AddUserToGame(UserId userId, GameId gameId)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	Game* game = mGameRegistry->FindGame(gameId);
	if (!game)
		return false;

	if (false == RemoveUserFromGame(userId))
		return false;

	MainDbSession mainDbSession(CommitType::AUTO);
	if (false == mainDbSession.IsReady())
		return false;

	SP_ACCOUNT_USER_GAME_UPDATE spAccountUserGameUpdate(mainDbSession,
		userId,
		gameId,
		tClock.GetGlobalNowTs()
	);
	Result result = DbUtil::ExecuteSp(spAccountUserGameUpdate);
	if (Result::SUCCEEDED != result)
	{
		SP_ACCOUNT_USER_GAME_INSERT spAccountUserGameInsert(mainDbSession,
			userId,
			gameId,
			tClock.GetGlobalNowTs()
		);
		result = DbUtil::ExecuteSp(spAccountUserGameInsert);
		if (Result::SUCCEEDED != result)
			return false;
	}

	mUserGameMap.insert_or_assign(userId, gameId);

	return true;
}

bool GameListManager::RemoveUserFromGame(UserId userId)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	auto it = mUserGameMap.find(userId);
	if (it == mUserGameMap.end())
		return true;

	GameId gameId = it->second;
	Game* game = mGameRegistry->FindGame(gameId);
	if (game && (game->GetUserIdList().size() == 1))
		return RemoveGame(gameId);

	MainDbSession mainDbSession(CommitType::AUTO);
	if (false == mainDbSession.IsReady())
		return false;

	SP_ACCOUNT_USER_GAME_UPDATE spAccountUserGameUpdate(mainDbSession,
		userId,
		INVALID_UUID,
		tClock.GetGlobalNowTs()
	);
	Result result = DbUtil::ExecuteSp(spAccountUserGameUpdate);
	if (Result::SUCCEEDED != result)
		return false;

	if (game)
		game->RemoveUser(userId);

	mUserGameMap.erase(it);

	return true;
}

const GameMap& GameListManager::GetGameList() const
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	return mGameRegistry->GetGameList();
}

void GameListManager::GetGameListAsync(std::promise<std::wstring>* promise) const
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	std::wstring result = L"";

	for (const auto& [gameId, gamePtr] : mGameRegistry->GetGameList())
	{
		result = std::format(L"{}\n", &gamePtr->GetGame()).data();
	}
	
	result += L"\n";

	promise->set_value(result);
}

const Game* GameListManager::FindGame(GameId gameId) const
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	const Game* gamePtr = mGameRegistry->FindGame(gameId);
	return gamePtr;
}

const AppId GameListManager::GetGameAppId(GameId gameId) const
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	const Game* gamePtr = mGameRegistry->FindGame(gameId);
	if (!gamePtr)
		return INVALID_APP_ID;

	const GAME& game = gamePtr->GetGame();
	const size_t hash = GetHashFromGame(game);
	const AppId pickedAppId = MainSocketUtil::PickGameAppId(hash);
	return pickedAppId;
}

size_t GameListManager::GetHashFromGame(const GAME& game) const
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	if (game.Get_c_user_id() == INVALID_UUID)
	{
		return static_cast<size_t>(game.Get_c_server_id());
	}
	else
	{
		return std::hash<GameId>()(game.Get_c_game_id());
	}
}

