// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;

// [WHY] Game은 import MainServer를 통해 모듈에서 이미 export됨


bool Game::AddUser(UserId userId)
{
	auto it = std::find(mUserIdList.begin(), mUserIdList.end(), userId);
	if (it != mUserIdList.end())
		return false;

	mUserIdList.emplace_back(userId);
	
	return true;
}

bool Game::RemoveUser(UserId userId)
{
	auto it = std::find(mUserIdList.begin(), mUserIdList.end(), userId);
	if (it == mUserIdList.end())
		return false;

	mUserIdList.erase(it);
	
	return true;
}

