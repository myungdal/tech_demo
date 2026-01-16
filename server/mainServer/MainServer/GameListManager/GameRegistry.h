// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class GAME;

class GameRegistry final
{
private:
	GameMap mGameMap;

public:
	Game* CreateGame(const GAME& game);
	bool RemoveGame(GameId gameId);
	Game* FindGame(GameId gameId);
	Game* FindServerHomeGame(ServerId serverId);

	bool AddUserToGame(UserId userId, GameId gameId);
	bool RemoveUserFromGame(UserId userId, GameId gameId);

	const GameMap& GetGameList() const { return mGameMap; }
};

