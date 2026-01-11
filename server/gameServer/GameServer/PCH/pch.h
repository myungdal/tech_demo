// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PCH/pch_serverEngine.h"

#include "MmoSync/PCH/pch_mmoSync.h"

#include "Packet/NetworkPacket/PacketHeader.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GameChannel 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GameChannel;
using GameChannelPtr = std::shared_ptr<GameChannel>;


using GameChannelUserIdTable = std::array<UserIdList, MAX_APP_COUNT>; // Front서버 별 유저ID 목록

enum class GameChannelState : uint8_t
{
	NONE = 0,
	PLAYING = 1,
	FINISHING = 2,
	FINISHED = 3
};

using UserCacheAccessorPtr = std::shared_ptr<class UserCacheAccessor>;

struct GameChannelUserInfo
{
	PacketHeader mCurrPacketHeader;

	UserCacheAccessorPtr mUserCacheAccessor = nullptr;
};

using GameChannelUserInfoPtr = std::shared_ptr<GameChannelUserInfo>;
using GamechannelUserMap = std::unordered_map<UserId, GameChannelUserInfoPtr>;

