// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;

// [WHY] Room은 import MainServer를 통해 모듈에서 이미 export됨


RoomPtr RoomListManager::AddUserToRoom(RoomId roomId, RoomType roomType, AppId appId, const USER& user)
{
	const UserId userId = user.Get_c_user_id();
	RoomPtr roomPtr = RemoveUserFromRoom(roomId, appId, userId);
	if (roomPtr == nullptr)
	{
		roomPtr = std::make_shared<Room>(roomId, roomType);
		mRoomMap.emplace(roomId, roomPtr);
	}
	roomPtr->AddUser(appId, user);
	return roomPtr;
}

RoomPtr RoomListManager::RemoveUserFromRoom(RoomId roomId, AppId appId, UserId userId)
{
	auto it = mRoomMap.find(roomId);
	if (it == mRoomMap.end())
		return nullptr;

	RoomPtr roomPtr = it->second;
	roomPtr->RemoveUser(appId, userId);
	if (roomPtr->IsEmpty())
	{
		mRoomMap.erase(it);
		return nullptr;
	}

	return roomPtr;
}

void RoomListManager::RemoveAllUserFromRoom(RoomId roomId, AppId appId)
{
	auto it = mRoomMap.find(roomId);
	if (it == mRoomMap.end())
		return;

	RoomPtr roomPtr = it->second;
	roomPtr->RemoveUserAll(appId);

	mRoomMap.erase(it);
}
