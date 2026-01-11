// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;

// [WHY] Room은 import MainServer를 통해 모듈에서 이미 export됨


Room::Room(RoomId roomId, RoomType roomType)
	: 
	mRoomId(roomId),
	mRoomType(roomType)
{
}

void Room::AddUser(AppId appId, const USER& user)
{
	RemoveUser(appId, user.Get_c_user_id());

	const UserId userId = user.Get_c_user_id();
	PacketKeep<USER> userContainer(user);

	auto it = mUserTable.find(appId);
	if (it == mUserTable.end())
	{
		UserMap userMap;
		userMap.emplace(userId, userContainer);
		mUserTable.emplace(appId, std::move(userMap));
	}
	else
	{
		it->second.emplace(userId, userContainer);
	}
}

void Room::RemoveUser(AppId appId, UserId userId)
{
	auto it = mUserTable.find(appId);
	if (it != mUserTable.end())
	{
		it->second.erase(userId);
		if (it->second.empty())
		{
			mUserTable.erase(it);
		}
	}
}

void Room::RemoveUserAll(AppId appId)
{
	auto it = mUserTable.find(appId);
	if (it != mUserTable.end())
	{
		mUserTable.erase(it);
	}
}
