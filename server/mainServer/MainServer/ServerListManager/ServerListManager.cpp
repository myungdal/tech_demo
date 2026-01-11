// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;
import ServerEngineDb;
// [WHY] Server는 import MainServer를 통해 모듈에서 이미 export됨


bool ServerListManager::Load()
{
	{
		MainDbSession mainDbSession(CommitType::AUTO);
		if (false == mainDbSession.IsReady())
			return false;

		SP_SERVER_SELECT_ALL sp(mainDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				const SERVER::Writer wp = DbUtil::SpToPacket(sp);
				const ServerPtr serverPtr = std::make_shared<Server>(*wp.GetPacketPtr());
				const ServerId serverId = serverPtr->GetData().Get_c_server_id();
				mServerMap.emplace(serverId, serverPtr);
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		WriteLock lock(mServerHomeGameListLock);

		mServerHomeGameList.clear();

		for (const auto [serverId, serverPtr] : mServerMap)
		{
			const SERVER& server = serverPtr->GetData();
			const GameSid homeGameSid = server.Get_c_home_game_sid();
			mServerHomeGameList.emplace_back(serverId, homeGameSid);
		}
	}

	return true;
}

void ServerListManager::GetServerHomeGameList(OUT ServerHomeGameList& serverHomeGameList) const
{
	ReadLock lock(mServerHomeGameListLock);

	OUT serverHomeGameList = mServerHomeGameList;
}
