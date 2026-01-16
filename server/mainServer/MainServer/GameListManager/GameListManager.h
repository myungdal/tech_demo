// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerBase/Worker.h"


class GAME;
class GameRegistry;
class SocketMainFromGame;

class GameListManager final : public Worker
{
private:
	std::shared_ptr<GameRegistry> mGameRegistry = nullptr;
	std::unordered_map<UserId, GameId> mUserGameMap;

public:
	GameListManager();

public:
	bool Load();

private:
	bool LoadGameList();
	bool LoadUserGameList();

public:
	Game* CreateGame(ServerId serverId, UserId userId, GameSid gameSid);
	bool RemoveGame(GameId gameId);

	bool IsExistGame(GameId gameId) const;
	bool AddUserToGame(UserId userId, GameId gameId);
	bool RemoveUserFromGame(UserId userId);
	
	const GameMap& GetGameList() const;
	void GetGameListAsync(std::promise<std::wstring>* promise) const;
	const Game* FindGame(GameId gameId) const;
	const AppId GetGameAppId(GameId gameId) const;

	size_t GetHashFromGame(const GAME& game) const;
};

inline std::shared_ptr<GameListManager> gGameListManager = nullptr;
