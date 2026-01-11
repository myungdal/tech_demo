// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerBase/Worker.h"


class SocketMainFromGame;

class ServerListManager : public Worker
{
private:
	ServerMap mServerMap;

private:
	mutable Lock mServerHomeGameListLock;
	ServerHomeGameList mServerHomeGameList;

public:
	ServerListManager() = default;

public:
	bool Load();

public:
	const ServerMap& GetServerMap() const { return mServerMap; }
	void GetServerHomeGameList(OUT ServerHomeGameList& serverHomeGameList) const;
};

inline std::shared_ptr<ServerListManager> gServerListManager = nullptr;
