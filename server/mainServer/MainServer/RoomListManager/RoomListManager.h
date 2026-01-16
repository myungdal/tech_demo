// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerBase/Worker.h"


class Room;
class USER;

class RoomListManager : public Worker
{
private:
	using RoomMap = std::unordered_map<RoomId, RoomPtr>;

	RoomMap mRoomMap;

public:
	RoomPtr AddUserToRoom(RoomId roomId, RoomType roomType, AppId appId, const USER& user);
	RoomPtr RemoveUserFromRoom(RoomId roomId, AppId appId, UserId userId);
	void RemoveAllUserFromRoom(RoomId roomId, AppId appId);
};

inline std::shared_ptr<RoomListManager> gRoomListManager = nullptr;
