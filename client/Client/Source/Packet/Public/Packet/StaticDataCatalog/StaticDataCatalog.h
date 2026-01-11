// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataCatalog/Internal/StaticDataList.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StaticDataCatalog
// 모든 정적 데이터 리스트를 보관하고 관리하는 클래스
// 서버/클라이언트가 각 다른 데이터를 가지고 있음
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PACKET_API StaticDataCatalog final
{
public:
	enum class State : uint8_t
	{
		NONE = 0,
		SYNCING,
		SYNCED
	};

private:
	std::atomic<State> mState = { State::NONE };

private:
	using StaticDataListTypes = std::tuple<
		class STATIC_ACHIEVEMENT,
		class STATIC_ACHIEVEMENT_STEP,
		class STATIC_BUILDING,
		class STATIC_CHARACTER,
		class STATIC_CHARACTER_SKILL,
		class STATIC_GAME,
		class STATIC_ITEM,
		class STATIC_ITEM_STAT,
		class STATIC_MAIL,
		class STATIC_MAP,
		class STATIC_MISSION,
		class STATIC_PRODUCT,
		class STATIC_QUEST,
		class STATIC_REWARD,
		class STATIC_REWARD_GROUP,
		class STATIC_REWARD_RATIO,
		class STATIC_SCHEDULE,
		class STATIC_STAT,
		class STATIC_STOCK,
		class STATIC_TRANSLATION,
		class STATIC_USER_EXP
	> ;

private:
	using StaticDataListPtr = std::shared_ptr<StaticDataListBase>;
	using StaticDataTable = std::vector<StaticDataListPtr>;

	StaticDataTable mStaticDataTable;

	const StaticDataIndexList mStaticDataIndexListForGameServer;
	const StaticDataIndexList mStaticDataIndexListForClient;

private:
	Checksum mExpectedChecksumForGameServer;
	Checksum mExpectedChecksumForClient;
	std::vector<StaticDataSize> mExpectedSizeList;
	std::vector<Checksum> mExpectedChecksumList;

	Checksum mMyChecksumForGameServer;
	Checksum mMyChecksumForClient;
	std::vector<StaticDataSize> mMySizeList;
	std::vector<Checksum> mMyChecksumList;

	std::vector<uint8_t*> mBinaryList;

public:
	StaticDataCatalog(const StaticDataIndexList& staticDataIndexListForGameServer, const StaticDataIndexList& staticDataIndexListForClient);
	virtual ~StaticDataCatalog();

private:
	template <typename _StaticData>
	void RegisterList(size_t idx)
	{
		using _StaticDataType = typename std::decay<_StaticData>::type;
		using _StaticDataListType = StaticDataList<_StaticDataType>;

		_StaticDataListType::mStaticDataListIdx = idx;

		std::shared_ptr<_StaticDataListType> ptr = std::make_shared<_StaticDataListType>();
		mStaticDataTable.emplace_back(ptr);
	}

public:
	template <typename _StaticData>
	std::vector<PacketKeep<typename std::decay<_StaticData>::type>>& GetList() const
	{
		using _StaticDataType = typename std::decay<_StaticData>::type;
		using _StaticDataListType = StaticDataList<_StaticDataType>;

		size_t idx = _StaticDataListType::mStaticDataListIdx;
		std::shared_ptr<StaticDataListBase> ptr = mStaticDataTable.at(idx);
		std::shared_ptr<_StaticDataListType> castedPtr = std::static_pointer_cast<_StaticDataListType>(ptr);
		return castedPtr->List();
	}

public:
	void Cleanup();
	void StaticDataToBinary();

public:
	std::pair<bool, StaticDataIndex> BuildDataForGameServer();
	std::pair<bool, StaticDataIndex> BuildDataForClient();

private:
	bool BinaryToStaticDataList(StaticDataIndex idx);

public:
	// 싱크 상태 쿼리 및 설정
	bool IsSyncing() const { return (State::SYNCING == mState.load()) ? true : false; }
	bool IsSynced() const { return (State::SYNCED == mState.load()) ? true : false; }
	bool SetSyncing();
	bool SetSynced();
	bool ResetSync();

public:
	Checksum& ExpectedChecksumForGameServer() { return mExpectedChecksumForGameServer; }
	Checksum& ExpectedChecksumForClient() { return mExpectedChecksumForClient; }
	std::vector<StaticDataSize>& ExpectedSizeList() { return mExpectedSizeList; }
	std::vector<Checksum>& ExpectedChecksumList() { return mExpectedChecksumList; }	

public:
	const Checksum& GetMyChecksumForGameServer() const { return mMyChecksumForGameServer; }
	const Checksum& GetMyChecksumForClient() const { return mMyChecksumForClient; }
	const std::vector<StaticDataSize>& GetMySizeList() const { return mMySizeList; }
	const std::vector<Checksum>& GetMyChecksumList() const { return mMyChecksumList; }

//public:
//	Checksum& MyChecksumForGameServer() { return mMyChecksumForGameServer; }
//	Checksum& MyChecksumForClient() { return mMyChecksumForClient; }
//	std::vector<StaticDataSize>& MySizeList() { return mMySizeList; }
//	std::vector<Checksum>& MyChecksumList() { return mMyChecksumList; }

public:
	std::vector<uint8_t*>& BinaryList() { return mBinaryList; }
};

#if defined(__UNREAL__)
PACKET_API extern TSharedPtr<StaticDataCatalog> gStaticDataCatalog;
#else
inline std::shared_ptr<StaticDataCatalog> gStaticDataCatalog = nullptr;
#endif
