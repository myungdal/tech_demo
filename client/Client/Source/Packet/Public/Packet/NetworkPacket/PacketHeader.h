// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PacketHeader
// 네트워크 패킷의 헤더 정보를 담는 클래스입니다.
// 서버 간 라우팅, 사용자 식별, 게임 세션 정보 등을 포함합니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class PacketHeader
{
public:
	// AppId 배열 인덱스 열거형
	enum class AppType_appIds : int64_t
	{
		DB_SERVER = 0,
		FRONT_SERVER = 1,
		GAME_SERVER = 2,
		MAX = 3,
	};

	enum class AppType_remoteSocketIds : int64_t
	{
		DB_SERVER = 0,
		FRONT_SERVER = 1,
		GAME_SERVER = 2,
		MAIN_SERVER = 3,
		MAX = 4,
	};

	enum class AppType_remoteRequestIds : int64_t
	{
		DB_SERVER = 0,
		MAIN_SERVER = 1,
		SHELL = 1,
		MAX = 2,
	};

public:
	std::array<AppId, static_cast<size_t>(AppType_appIds::MAX)> mAppIds;
	std::array<RpcId, static_cast<size_t>(AppType_remoteSocketIds::MAX)> mRemoteSocketIds;
	std::array<RpcId, static_cast<size_t>(AppType_remoteRequestIds::MAX)> mRemoteRequestIds;

public:
	DbShardIdx mDbShardIdx = INVALID_DB_SHARD_IDX;
	DbSeq mDbSeq = INVALID_DB_SEQ;

	AccountId mAccountId = INVALID_UUID;
	ServerId mServerId = INVALID_SERVER_ID;
	UserId mUserId = INVALID_UUID;
	GuildId mGuildId = INVALID_UUID;

	GameId mGameId = INVALID_UUID;
	GameChannelIndex mGameChannelIndex = INVALID_GAME_CHANNEL_INDEX;
	WorldUserId mWorldUserId = INVALID_WORLD_USER_ID;
	
	Result mPacketResult = Result::SUCCEEDED;

public:
	PacketHeader()
	{
		Initialize();
	}

	void Initialize()
	{
		mAppIds.fill(INVALID_APP_ID);
		mRemoteSocketIds.fill(INVALID_RPC_ID);
		mRemoteRequestIds.fill(INVALID_RPC_ID);

		mDbShardIdx = INVALID_DB_SHARD_IDX;
		mDbSeq = INVALID_DB_SEQ;

		mAccountId = INVALID_UUID;
		mServerId = INVALID_SERVER_ID;
		mUserId = INVALID_UUID;
		mGuildId = INVALID_UUID;

		mGameId = INVALID_UUID;
		mGameChannelIndex = INVALID_GAME_CHANNEL_INDEX;
		mWorldUserId = INVALID_WORLD_USER_ID;

		mPacketResult = Result::SUCCEEDED;
	}

public:
	bool ValidateState(const PacketHeader& other) const
	{
		return
			mAppIds == other.mAppIds &&

			mDbShardIdx == other.mDbShardIdx &&

			mAccountId == other.mAccountId &&
			mServerId == other.mServerId &&
			mUserId == other.mUserId &&
			mGuildId == other.mGuildId &&

			mGameId == other.mGameId &&
			mGameChannelIndex == other.mGameChannelIndex;
	}

public:
	AppId GetAppId(AppType appType) const 
	{ 
		switch (appType)
		{
		case AppType::DB_SERVER: return mAppIds.at(static_cast<size_t>(AppType_appIds::DB_SERVER));
		case AppType::FRONT_SERVER: return mAppIds.at(static_cast<size_t>(AppType_appIds::FRONT_SERVER));
		case AppType::GAME_SERVER: return mAppIds.at(static_cast<size_t>(AppType_appIds::GAME_SERVER));
		default: 
			_ASSERT_CRASH(false);
			return INVALID_APP_ID;
		}		 
	}

	void SetAppId(AppType appType, AppId appId) 
	{ 
		switch (appType)
		{
		case AppType::DB_SERVER: mAppIds.at(static_cast<size_t>(AppType_appIds::DB_SERVER)) = appId; break;
		case AppType::FRONT_SERVER: mAppIds.at(static_cast<size_t>(AppType_appIds::FRONT_SERVER)) = appId; break;
		case AppType::GAME_SERVER: mAppIds.at(static_cast<size_t>(AppType_appIds::GAME_SERVER)) = appId; break;
		default:
			_ASSERT_CRASH(false);
			break;
		}
	}

public:
	RpcId GetRemoteSocketId(AppType appType) const 
	{ 
		switch (appType)
		{
		case AppType::DB_SERVER: return mRemoteSocketIds.at(static_cast<size_t>(AppType_remoteSocketIds::DB_SERVER));
		case AppType::FRONT_SERVER: return mRemoteSocketIds.at(static_cast<size_t>(AppType_remoteSocketIds::FRONT_SERVER));
		case AppType::GAME_SERVER: return mRemoteSocketIds.at(static_cast<size_t>(AppType_remoteSocketIds::GAME_SERVER));
		case AppType::MAIN_SERVER: return mRemoteSocketIds.at(static_cast<size_t>(AppType_remoteSocketIds::MAIN_SERVER));
		default:
			_ASSERT_CRASH(false);
			return INVALID_RPC_ID;
		}
	}

	void SetRemoteSocketId(AppType appType, RpcId remoteSocketId) 
	{ 
		switch (appType)
		{
		case AppType::DB_SERVER: mRemoteSocketIds.at(static_cast<size_t>(AppType_remoteSocketIds::DB_SERVER)) = remoteSocketId; break;
		case AppType::FRONT_SERVER: mRemoteSocketIds.at(static_cast<size_t>(AppType_remoteSocketIds::FRONT_SERVER)) = remoteSocketId; break;
		case AppType::GAME_SERVER: mRemoteSocketIds.at(static_cast<size_t>(AppType_remoteSocketIds::GAME_SERVER)) = remoteSocketId; break;
		case AppType::MAIN_SERVER: mRemoteSocketIds.at(static_cast<size_t>(AppType_remoteSocketIds::MAIN_SERVER)) = remoteSocketId; break;
		default:
			_ASSERT_CRASH(false);
			break;
		}
	}

public:
	RpcId GetRemoteRequestId(AppType appType) const 
	{
		switch (appType)
		{
		case AppType::DB_SERVER: return mRemoteRequestIds.at(static_cast<size_t>(AppType_remoteRequestIds::DB_SERVER));
		case AppType::MAIN_SERVER: return mRemoteRequestIds.at(static_cast<size_t>(AppType_remoteRequestIds::MAIN_SERVER));
		case AppType::SHELL: return mRemoteRequestIds.at(static_cast<size_t>(AppType_remoteRequestIds::SHELL));			
		default:
			_ASSERT_CRASH(false);
			return INVALID_RPC_ID;
		}
	}

	void SetRemoteRequestId(AppType appType, RpcId remoteRequestId) 
	{
		switch (appType)
		{
		case AppType::DB_SERVER: mRemoteRequestIds.at(static_cast<size_t>(AppType_remoteRequestIds::DB_SERVER)) = remoteRequestId; break;
		case AppType::MAIN_SERVER: mRemoteRequestIds.at(static_cast<size_t>(AppType_remoteRequestIds::MAIN_SERVER)) = remoteRequestId; break;
		case AppType::SHELL: mRemoteRequestIds.at(static_cast<size_t>(AppType_remoteRequestIds::SHELL)) = remoteRequestId; break;
		default:
			_ASSERT_CRASH(false);
			break;
		}
	}

public:
	DbShardIdx GetDbShardIdx() const noexcept { return mDbShardIdx; }
	void SetDbShardIdx(DbShardIdx dbShardIdx) noexcept { mDbShardIdx = dbShardIdx; }

	DbSeq GetDbSeq() const noexcept { return mDbSeq; }
	void SetDbSeq(DbSeq dbSeq) noexcept { mDbSeq = dbSeq; }

public:
	const AccountId& GetAccountId() const noexcept { return mAccountId; }
	void SetAccountId(const AccountId& accountId) noexcept { mAccountId = accountId; }

	ServerId GetServerId() const noexcept { return mServerId; }
	void SetServerId(ServerId serverId) noexcept { mServerId = serverId; }

	const UserId& GetUserId() const noexcept { return mUserId; }
	void SetUserId(const UserId& userId) noexcept { mUserId = userId; }

	const GuildId& GetGuildId() const noexcept { return mGuildId; }
	void SetGuildId(const GuildId& guildId) noexcept { mGuildId = guildId; }

public:
	const GameId& GetGameId() const noexcept { return mGameId; }
	void SetGameId(const GameId& gameId) noexcept { mGameId = gameId; }

	GameChannelIndex GetGameChannelIndex() const noexcept { return mGameChannelIndex; }
	void SetGameChannelIndex(GameChannelIndex gameChannelIndex) noexcept { mGameChannelIndex = gameChannelIndex; }

	WorldUserId GetWorldUserId() const noexcept { return mWorldUserId; }
	void SetWorldUserId(WorldUserId worldUserId) noexcept { mWorldUserId = worldUserId; }

public:
	Result GetPacketResult() const noexcept { return mPacketResult; }
	void SetPacketResult(Result packetResult) noexcept { mPacketResult = packetResult; }
};
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<PacketHeader, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const PacketHeader& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");

		// mAppIds
		{
			if (t.mAppIds[static_cast<size_t>(PacketHeader::AppType_appIds::DB_SERVER)] != INVALID_APP_ID)
				result += std::format(L"\n{}AppIds[DB_SERVER]: {}", tTabStr, t.mAppIds[static_cast<size_t>(PacketHeader::AppType_appIds::DB_SERVER)]);

			if (t.mAppIds[static_cast<size_t>(PacketHeader::AppType_appIds::FRONT_SERVER)] != INVALID_APP_ID)
				result += std::format(L"\n{}AppIds[FRONT_SERVER]: {}", tTabStr, t.mAppIds[static_cast<size_t>(PacketHeader::AppType_appIds::FRONT_SERVER)]);

			if (t.mAppIds[static_cast<size_t>(PacketHeader::AppType_appIds::GAME_SERVER)] != INVALID_APP_ID)
				result += std::format(L"\n{}AppIds[GAME_SERVER]: {}", tTabStr, t.mAppIds[static_cast<size_t>(PacketHeader::AppType_appIds::GAME_SERVER)]);
		}

		// mRemoteSocketIds
		{
			if (t.mRemoteSocketIds[static_cast<size_t>(PacketHeader::AppType_remoteSocketIds::DB_SERVER)] != INVALID_RPC_ID)
				result += std::format(L"\n{}RemoteSocketIds[DB_SERVER]: {}", tTabStr, t.mRemoteSocketIds[static_cast<size_t>(PacketHeader::AppType_remoteSocketIds::DB_SERVER)]);

			if (t.mRemoteSocketIds[static_cast<size_t>(PacketHeader::AppType_remoteSocketIds::FRONT_SERVER)] != INVALID_RPC_ID)
				result += std::format(L"\n{}RemoteSocketIds[FRONT_SERVER]: {}", tTabStr, t.mRemoteSocketIds[static_cast<size_t>(PacketHeader::AppType_remoteSocketIds::FRONT_SERVER)]);

			if (t.mRemoteSocketIds[static_cast<size_t>(PacketHeader::AppType_remoteSocketIds::GAME_SERVER)] != INVALID_RPC_ID)
				result += std::format(L"\n{}RemoteSocketIds[GAME_SERVER]: {}", tTabStr, t.mRemoteSocketIds[static_cast<size_t>(PacketHeader::AppType_remoteSocketIds::GAME_SERVER)]);

			if (t.mRemoteSocketIds[static_cast<size_t>(PacketHeader::AppType_remoteSocketIds::MAIN_SERVER)] != INVALID_RPC_ID)
				result += std::format(L"\n{}RemoteSocketIds[MAIN_SERVER]: {}", tTabStr, t.mRemoteSocketIds[static_cast<size_t>(PacketHeader::AppType_remoteSocketIds::MAIN_SERVER)]);
		}

		// mRemoteRequestIds
		{
			if (t.mRemoteRequestIds[static_cast<size_t>(PacketHeader::AppType_remoteRequestIds::DB_SERVER)] != INVALID_RPC_ID)
				result += std::format(L"\n{}RemoteRequestIds[DB_SERVER]: {}", tTabStr, t.mRemoteRequestIds[static_cast<size_t>(PacketHeader::AppType_remoteRequestIds::DB_SERVER)]);

			if (t.mRemoteRequestIds[static_cast<size_t>(PacketHeader::AppType_remoteRequestIds::MAIN_SERVER)] != INVALID_RPC_ID)
				result += std::format(L"\n{}RemoteRequestIds[MAIN_SERVER]: {}", tTabStr, t.mRemoteRequestIds[static_cast<size_t>(PacketHeader::AppType_remoteRequestIds::MAIN_SERVER)]);
		}

		if (t.GetDbShardIdx() != INVALID_DB_SHARD_IDX)
			result += std::format(L"\n{}DbShardIdx: {}", tTabStr, t.GetDbShardIdx());

		if (t.GetDbSeq() != INVALID_DB_SEQ)
			result += std::format(L"\n{}DbSeq: {}", tTabStr, t.GetDbSeq());

		if (t.GetAccountId() != INVALID_UUID)
			result += std::format(L"\n{}AccountId: {}", tTabStr, t.GetAccountId());

		if (t.GetServerId() != INVALID_SERVER_ID)
			result += std::format(L"\n{}ServerId: {}", tTabStr, t.GetServerId());

		if (t.GetUserId() != INVALID_UUID)
			result += std::format(L"\n{}UserId: {}", tTabStr, t.GetUserId());

		if (t.GetGuildId() != INVALID_UUID)
			result += std::format(L"\n{}GuildId: {}", tTabStr, t.GetGuildId());

		if (t.GetGameId() != INVALID_UUID)
			result += std::format(L"\n{}GameId: {}", tTabStr, t.GetGameId());

		if (t.GetGameChannelIndex() != INVALID_GAME_CHANNEL_INDEX)
			result += std::format(L"\n{}GameChannelIndex: {}", tTabStr, t.GetGameChannelIndex());

		if (t.GetWorldUserId() != INVALID_WORLD_USER_ID)
			result += std::format(L"\n{}WorldUserId: {}", tTabStr, t.GetWorldUserId());

		if (t.GetPacketResult() != Result::MAX)
			result += std::format(L"\n{}PacketResult: {}", tTabStr, t.GetPacketResult());

		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
