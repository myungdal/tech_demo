// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import FrontServer;

#include "FrontDataManager.h"


FrontDataManager::FrontDataManager()
{
	mTlsFrontUserTable.resize(gWorkerThreadCount);
	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		mTlsFrontUserTable[i] = FrontUserMap{};
	}
	mTlsFrontGameTable.resize(gWorkerThreadCount);
	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		mTlsFrontGameTable[i] = FrontGameMap{};
	}
	mTlsFrontRoomTable.resize(gWorkerThreadCount);
	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		mTlsFrontRoomTable[i] = FrontRoomMap{};
	}
	mThreadDataLock.resize(gWorkerThreadCount);
	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		mThreadDataLock[i] = std::make_shared<Lock>();
	}
}
FrontUserPtr FrontDataManager::InsertUser(
	const ACCOUNT_USER& accountUser,
	const USER& user, 
	const PacketHeader& packetHeader, 
	SocketPtr<SocketFrontFromClient> socket
)
{
	// [WHY] 다른 Worker에서 호출되지 않음 (= 별도 락 불필요)
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	const UserId userId = packetHeader.GetUserId();

	if (true == mManagedUserIdSet.contains(userId))
		return nullptr;

	mManagedUserIdSet.insert(userId);

	++mUserCount;

	FrontUserPtr frontUser = std::make_shared<FrontUser>();

	frontUser->SetUser(accountUser, user);
	frontUser->SetCurrPacketHeader(packetHeader);
	frontUser->SetClientSocket(socket);
	frontUser->SetGameSocket(SocketPtr<SocketFrontToGame>());

	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		WriteLock lock(*mThreadDataLock[i]);
		FrontUserMap& frontUserMap = mTlsFrontUserTable[i];
		frontUserMap.emplace(userId, frontUser);
	}

	return frontUser;
}

bool FrontDataManager::DeleteUser(UserId userId)
{
	// [WHY] 다른 Worker에서 호출되지 않음 (= 별도 락 불필요)
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	_DEBUG_LOG(RED, L"userId, {}", userId);

	auto managerUserIt = mManagedUserIdSet.find(userId);
	if (mManagedUserIdSet.end() == managerUserIt)
	{
		return false;
	}

	mManagedUserIdSet.erase(managerUserIt);

	--mUserCount;

	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		WriteLock lock(*mThreadDataLock[i]);

		FrontUserMap& frontUserMap = mTlsFrontUserTable[i];
		auto it = frontUserMap.find(userId);
		if (it != frontUserMap.end())
		{
			frontUserMap.erase(it);
		}

		// 게임 테이블 정리
		FrontGameMap& gameMap = mTlsFrontGameTable[i];
		for (auto gameIt = gameMap.begin(); gameIt != gameMap.end(); )
		{
			GameUserMap& gameUserMap = gameIt->second;
			gameUserMap.erase(userId);

			if (gameUserMap.empty())
				gameIt = gameMap.erase(gameIt);
			else
				++gameIt;
		}

		// 방 테이블 정리
		FrontRoomMap& roomMap = mTlsFrontRoomTable[i];
		for (auto roomIt = roomMap.begin(); roomIt != roomMap.end(); )
		{
			RoomUserMap& roomUserMap = roomIt->second;
			roomUserMap.erase(userId);

			if (roomUserMap.empty())
				roomIt = roomMap.erase(roomIt);
			else
				++roomIt;
		}
	}

	return true;
}

void FrontDataManager::GameUserEnter(
	const PacketHeader& packetHeader,
	SocketPtr<SocketFrontToGame> socket
)
{
	// [WHY] 다른 Worker에서 호출되지 않음 (= 별도 락 불필요)
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	const UserId userId = packetHeader.GetUserId();

	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		WriteLock lock(*mThreadDataLock[i]);

		if (FrontUserPtr frontUser = FindUser(i, userId))
		{
			frontUser->SetCurrPacketHeader(packetHeader);
			frontUser->SetGameSocket(socket);

			FrontGameMap& gameMap = mTlsFrontGameTable[i];
			auto gameIt = gameMap.find(packetHeader.GetGameId());
			if (gameIt == gameMap.end())
			{
				auto [newIt, _] = gameMap.emplace(packetHeader.GetGameId(), GameUserMap{});
				gameIt = newIt;
			}

			GameUserMap& gameUserMap = gameIt->second;
			auto [gameUserIt, newly] = gameUserMap.emplace(userId, frontUser);
			if (!newly)
			{
				gameUserIt->second = frontUser;
			}
		}
	}
}

void FrontDataManager::GameUserLeave(const PacketHeader& packetHeader)
{
	// [WHY] 다른 Worker에서 호출되지 않음 (= 별도 락 불필요)
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	const UserId userId = packetHeader.GetUserId();

	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		WriteLock lock(*mThreadDataLock[i]);

		if (FrontUserPtr frontUser = FindUser(i, userId))
		{
			FrontGameMap& gameMap = mTlsFrontGameTable[i];
			auto gameIt = gameMap.find(frontUser->GetCurrPacketHeader().GetGameId());
			if (gameIt != gameMap.end())
			{
				GameUserMap& gameUserMap = gameIt->second;
				auto userIt = gameUserMap.find(userId);
				if (userIt != gameUserMap.end())
				{
					gameUserMap.erase(userIt);
				}

				if (gameUserMap.empty())
				{
					gameMap.erase(gameIt);
				}
			}

			frontUser->SetCurrPacketHeader(packetHeader);
			frontUser->SetGameSocket(SocketPtr<SocketFrontToGame>());
		}
	}
}

void FrontDataManager::RoomUserEnter(const RoomId roomId, const UserId userId)
{
	// [WHY] 다른 Worker에서 호출되지 않음 (= 별도 락 불필요)
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		WriteLock lock(*mThreadDataLock[i]);

		if (FrontUserPtr frontUser = FindUser(i, userId))
		{
			FrontRoomMap& frontRoomMap = mTlsFrontRoomTable[i];
			auto roomIt = frontRoomMap.find(roomId);
			if (roomIt == frontRoomMap.end())
			{
				auto [newIt, _] = frontRoomMap.emplace(roomId, RoomUserMap{});
				roomIt = newIt;
			}

			RoomUserMap& roomUserMap = roomIt->second;
			auto [roomUserIt, newly] = roomUserMap.emplace(userId, frontUser);
			if (!newly)
			{
				roomUserIt->second = frontUser;
			}
		}
	}
}

void FrontDataManager::RoomUserLeave(const RoomId roomId, const UserId userId)
{
	// [WHY] 다른 Worker에서 호출되지 않음 (= 별도 락 불필요)
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		WriteLock lock(*mThreadDataLock[i]);

		if (FrontUserPtr frontUser = FindUser(i, userId))
		{
			FrontRoomMap& frontRoomMap = mTlsFrontRoomTable[i];
			auto roomIt = frontRoomMap.find(roomId);
			if (roomIt != frontRoomMap.end())
			{
				RoomUserMap& roomUserMap = roomIt->second;
				auto userIt = roomUserMap.find(userId);
				if (userIt != roomUserMap.end())
				{
					roomUserMap.erase(userIt);
				}

				if (roomUserMap.empty())
				{
					frontRoomMap.erase(roomIt);
				}
			}
		}
	}
}

FrontUserPtr FrontDataManager::FindUser(ThreadId threadId, UserId userId) const
{
	ReadLock lock(*mThreadDataLock[threadId]);

	const FrontUserMap& frontUserMap = mTlsFrontUserTable[threadId];
	auto it = frontUserMap.find(userId);	
	return (it != frontUserMap.end()) ? it->second : nullptr;
}

FrontUserPtr FrontDataManager::FindUser(UserId userId) const
{
	return FindUser(tThreadId, userId);
}
