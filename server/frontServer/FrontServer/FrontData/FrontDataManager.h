// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerBase/Worker.h"
#include "ServerEngine/Socket/SocketPtr/SocketPtr.h"


class PacketHeader;
class ACCOUNT_USER;
class USER;
class SocketFrontFromClient;
class SocketFrontToGame;

// 프론트 서버의 유저 데이터를 관리하는 매니저
// Worker를 상속받아 단일 스레드에서 동작 (다른 Worker에서 호출 시 락 필요)
class FrontDataManager : public Worker
{
private:
	// 유저 카운트 (atomic으로 여러 스레드에서 읽기 가능)
	std::atomic<int32_t> mUserCount = 0;

	// 이 Worker가 관리하는 유저 ID 집합
	using ManagedUserIdSet = std::unordered_set<UserId>;
	ManagedUserIdSet mManagedUserIdSet;

	// TLS 유저 테이블 (스레드별 유저 맵)
	using FrontUserMap = std::unordered_map<UserId, FrontUserPtr>;
	using TlsFrontUserTable = std::vector<FrontUserMap>;
	TlsFrontUserTable mTlsFrontUserTable;

	// TLS 게임 테이블 (게임 ID -> 게임 내 유저 맵)
	using GameUserMap = std::unordered_map<UserId, FrontUserPtr>;
	using FrontGameMap = std::unordered_map<GameId, GameUserMap>;
	using TlsFrontGameTable = std::vector<FrontGameMap>;
	TlsFrontGameTable mTlsFrontGameTable;

	// TLS 방 테이블 (방 ID -> 방 내 유저 맵)
	using RoomUserMap = std::unordered_map<UserId, FrontUserPtr>;
	using FrontRoomMap = std::unordered_map<RoomId, RoomUserMap>;
	using TlsFrontRoomTable = std::vector<FrontRoomMap>;
	TlsFrontRoomTable mTlsFrontRoomTable;

	// 스레드별 락
	using ThreadDataLock = std::vector<std::shared_ptr<Lock>>;
	ThreadDataLock mThreadDataLock;

public:
	FrontDataManager();

	// 유저 추가. 성공 시 FrontUserPtr 반환, 실패 시 nullptr
	FrontUserPtr InsertUser(
		const ACCOUNT_USER& accountUser,
		const USER& user,
		const PacketHeader& packetHeader,
		SocketPtr<SocketFrontFromClient> socket
	);

	// 유저 삭제. 성공 시 true 반환
	bool DeleteUser(UserId userId);

	// 게임 입장 처리
	void GameUserEnter(
		const PacketHeader& packetHeader,
		SocketPtr<SocketFrontToGame> socket
	);

	// 게임 퇴장 처리
	void GameUserLeave(const PacketHeader& packetHeader);

	// 방 입장 처리
	void RoomUserEnter(const RoomId roomId, const UserId userId);

	// 방 퇴장 처리
	void RoomUserLeave(const RoomId roomId, const UserId userId);

	// 현재 스레드에서 유저 검색
	FrontUserPtr FindUser(UserId userId) const;

	// 유저 수 반환
	int32_t GetUserCount() const { return mUserCount.load(); }

	// 현재 스레드의 모든 유저에 대해 함수 실행
	template<class _Func>
	void ForEachUser(_Func func)
	{
		ReadLock lock(*mThreadDataLock[tThreadId]);

		FrontUserMap& userMap = mTlsFrontUserTable[tThreadId];
		
		for (auto [_, frontUser] : userMap)
		{
			func(frontUser);
		}
	}

	// 특정 방의 유저들에 대해 함수 실행
	template<class _Func>
	void ForEachUserInRoom(RoomId roomId, _Func func)
	{
		ReadLock lock(*mThreadDataLock[tThreadId]);

		FrontRoomMap& roomMap = mTlsFrontRoomTable[tThreadId];

		auto roomIt = roomMap.find(roomId);
		if (roomIt == roomMap.end())
			return;

		RoomUserMap& userMap = roomIt->second;
		for (auto [_, frontUser] : userMap)
		{
			func(frontUser);
		}
	}

	// 특정 게임의 유저들에 대해 함수 실행
	template<class _Func>
	void ForEachUserInGame(GameId gameId, _Func func)
	{
		ReadLock lock(*mThreadDataLock[tThreadId]);

		FrontGameMap& gameMap = mTlsFrontGameTable[tThreadId];
		
		auto gameIt = gameMap.find(gameId);	
		if (gameIt == gameMap.end())
			return;
		
		GameUserMap& userMap = gameIt->second;
		for (auto [_, frontUser] : userMap)
		{
			func(frontUser);
		}
	}

private:
	void CleanupUserFromGameAndRoom(UserId userId);
	FrontUserPtr FindUser(ThreadId threadId, UserId userId) const;
};

inline std::shared_ptr<FrontDataManager> gFrontDataManager = nullptr;
