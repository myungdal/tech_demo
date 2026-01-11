// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;

import ServerEngineDb;

bool AccountStore::Load()
{
	const size_t shardCnt = gUserDbConnPool->GetUserDbShardCnt();

	// userDb 샤드별 계정 수(근사치) 초기화
	mShardUserDbSize.resize(shardCnt);
	for (size_t& userDbSize : mShardUserDbSize)
	{
		userDbSize = 0;
	}

	MainDbSession mainDbSession(CommitType::AUTO);
	if (false == mainDbSession.IsReady())
	{
		_ASSERT_CRASH(false);
		return false;
	}

	SP_ACCOUNT_SELECT_ALL sp(mainDbSession);
	Result result = DbUtil::FetchSp(sp,
		[&]()
		{
			ACCOUNT::Writer wp = DbUtil::SpToPacket(sp);
			const ACCOUNT& account = wp.GetPacket();
			if (false == CanAddAccount(account))
				return Result::AUTH_ERROR;

			AccountPtr accountPtr = std::make_shared<Account>(account);
			if (false == AddAccountToMap(accountPtr))
				return Result::AUTH_ERROR;

			const DbShardIdx shardIdx = account.Get_c_db_shard_idx();
			if (static_cast<size_t>(shardIdx) >= shardCnt)
			{
				_ASSERT_CRASH(false);
				return Result::FATAL_ERROR;
			}

			// 샤드별 계정 수(근사치)를 누적. 샤드 선택/부하 분산 판단에 사용.
			++mShardUserDbSize[shardIdx];

			return Result::SUCCEEDED;
		}
	);
	if (Result::SUCCEEDED != result)
		return false;

	return true;
}

AccountPtr AccountStore::SetAuthTicket(const ACCOUNT& account, AuthTicket authTicket)
{
	const wchar_t* token2 = account.Get_c_device_token();
	const bool hasToken2 = (token2[0] != 0);
	if (!hasToken2)
		return nullptr; // DeviceToken이 없음.

	AccountPtr accountPtr = FindByAccountId(account.Get_c_account_id());
	if (!accountPtr)
		return nullptr; // 계정을 찾을 수 없음.

	const ACCOUNT& accountCached = accountPtr->GetAccount();

	const wchar_t* token0 = account.Get_c_google_token();
	const bool hasToken0 = (token0[0] != 0);
	const wchar_t* tokenCached0 = accountCached.Get_c_google_token();
	const bool hasTokenCached0 = (tokenCached0[0] != 0);
	if (hasTokenCached0)
	{
		if (!hasToken0 || (0 != wcscmp(token0, tokenCached0)))
			return nullptr;
	}

	const wchar_t* token1 = account.Get_c_apple_token();
	const bool hasToken1 = (token1[0] != 0);
	const wchar_t* tokenCached1 = accountCached.Get_c_apple_token();
	const bool hasTokenCached1 = (tokenCached1[0] != 0);
	if (hasTokenCached1)
	{
		if (!hasToken1 || (0 != wcscmp(token1, tokenCached1)))
			return nullptr;
	}

	if (!hasTokenCached0 && !hasTokenCached1)
	{
		const wchar_t* tokenCached2 = accountCached.Get_c_device_token();
		const bool hasTokenCached2 = (tokenCached2[0] != 0);
		if (hasTokenCached2)
		{
			if (!hasToken2 || (0 != wcscmp(token2, tokenCached2)))
				return nullptr;
		}
	}

	accountPtr->SetAuthTicket(authTicket);

	return accountPtr;
}

AccountPtr AccountStore::FindAccountAndSetAuthTicket(const ACCOUNT& account, AuthTicket authTicket)
{
	const wchar_t* token2 = account.Get_c_device_token();
	const bool hasToken2 = (token2[0] != 0);
	if (!hasToken2)
		return nullptr; // DeviceToken이 없음.

	AccountPtr accountPtr = nullptr;

	const wchar_t* token0 = account.Get_c_google_token();
	const bool hasToken0 = (token0[0] != 0);
	if (hasToken0)
	{
		auto it = mMapByGoogle.find(token0);
		if (it == mMapByGoogle.end())
			return nullptr;

		accountPtr = it->second;
	}

	const wchar_t* token1 = account.Get_c_apple_token();
	const bool hasToken1 = (token1[0] != 0);
	if (hasToken1)
	{
		auto it = mMapByApple.find(token1);
		if (it == mMapByApple.end())
			return nullptr;

		if (accountPtr)
		{
			// 이미 찾은 계정과 일치하는지 확인
			if (accountPtr != it->second)
				return nullptr;
		}
		else
		{
			accountPtr = it->second;
		}
	}

	// 마지막으로 디바이스로만 찾아보기 시도
	if (!accountPtr && !hasToken0 && !hasToken1)
	{
		auto it = mMapByDevice.find(account.Get_c_device_token());
		if (it != mMapByDevice.end())
			accountPtr = it->second;
	}

	// 못찾았으면 실패
	if (!accountPtr)
		return nullptr;

	const ACCOUNT& accountCached = accountPtr->GetAccount();

	// 디바이스토큰이 다르면 갱신
	const wchar_t* tokenCached2 = accountCached.Get_c_device_token();
	const bool hasTokenCached2 = (tokenCached2[0] != 0);
	if (hasTokenCached2 && (wcscmp(token2, tokenCached2) != 0))
	{
		MainDbSession mainDbSession(CommitType::AUTO);
		if (false == mainDbSession.IsReady())
		{
			_ASSERT_CRASH(false);
			return nullptr;
		}

		// DB에 Account 토큰 갱신
		{
			SP_ACCOUNT_UPDATE_TOKEN spAccountUpdateToken(
				mainDbSession,
				account.Get_c_account_id(),
				account.Get_c_device_token(),
				account.Get_c_google_token(),
				account.Get_c_apple_token(),
				tClock.GetGlobalNowTs()
			);
			Result result = DbUtil::ExecuteSp(spAccountUpdateToken);
			if (Result::SUCCEEDED != result)
				return nullptr;
		}

		// 갱신된 데이터 읽어서 반영
		{
			SP_ACCOUNT_SELECT spAccountSelect(
				mainDbSession,
				account.Get_c_account_id()
			);
			Result result = DbUtil::FetchSp(spAccountSelect,
				[&]()
				{
					ACCOUNT::Writer wp = DbUtil::SpToPacket(spAccountSelect);
					const ACCOUNT& accountUpdated = wp.GetPacket();

					// 캐시 갱신
					accountPtr->SetAccount(accountUpdated);

					return Result::SUCCEEDED;
				}
			);
			if (Result::SUCCEEDED != result)
				return nullptr;
		}

		// 디바이스토큰 맵 갱신
		mMapByDevice.erase(accountPtr->GetAccount().Get_c_device_token());
		mMapByDevice.insert_or_assign(account.Get_c_device_token(), accountPtr);
	}

	// 인증 티켓 발급.
	accountPtr->SetAuthTicket(authTicket);

	return accountPtr;
}

AccountPtr AccountStore::CreateAccount(const ACCOUNT& account, AuthTicket authTicket)
{
	if (false == CanAddAccount(account))
		return nullptr;

	DbShardIdx lightestShardIdx = GetLightestDb();

	MainDbSession mainDbSession(CommitType::AUTO);
	AccountId newAccountId = UuidUtil::GenerateUuid();
	AccountPtr accountPtr = nullptr;

	// DB에 계정 생성
	{
		SP_ACCOUNT_INSERT spAccountInsert(
			mainDbSession,
			newAccountId,
			account.Get_c_device_token(),
			account.Get_c_google_token(),
			account.Get_c_apple_token(),
			static_cast<uint64_t>(lightestShardIdx),
			static_cast<uint64_t>(AccountState::NORMAL),
			tClock.GetGlobalNowTs()
		);
		Result result = DbUtil::ExecuteSp(spAccountInsert);
		if (Result::SUCCEEDED != result)
			return nullptr;
	}

	// 갱신된 데이터 읽어서 반영
	{
		SP_ACCOUNT_SELECT spAccountSelect(
			mainDbSession,
			newAccountId
		);
		Result result = DbUtil::FetchSp(spAccountSelect,
			[&]()
			{
				ACCOUNT::Writer wp = DbUtil::SpToPacket(spAccountSelect);
				const ACCOUNT& accountCreated = wp.GetPacket();

				accountPtr = std::make_shared<Account>(accountCreated);

				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return nullptr;
	}

	accountPtr->SetAuthTicket(authTicket);

	if (false == AddAccountToMap(accountPtr))
		return nullptr;

	return accountPtr;
}

AccountPtr AccountStore::FindByAccountId(const AccountId accountId)
{
	auto it = mMapByAccountId.find(accountId);
	if (it == mMapByAccountId.end())
		return nullptr;

	return it->second;
}

bool AccountStore::CanModifyAccount(const ACCOUNT& account)
{
	// DeviceToken이 없음.
	const wchar_t* token2 = account.Get_c_device_token();
	const bool hasToken2 = (token2[0] != 0);
	if (!hasToken2)
		return false;

	const AccountId accountId(account.Get_c_account_id());
	AccountPtr accountPtr = FindByAccountId(accountId);
	if (!accountPtr)
		return false;

	const ACCOUNT& accountCached = accountPtr->GetAccount();

	const wchar_t* token0 = account.Get_c_google_token();
	const bool hasToken0 = (token0[0] != 0);
	const wchar_t* tokenCached0 = accountCached.Get_c_google_token();
	const bool hasTokenCached0 = (tokenCached0[0] != 0);
	if (hasTokenCached0)
	{
		if (!hasToken0 || wcscmp(token0, tokenCached0) != 0)
		{
			if (mMapByGoogle.contains(token0))
				return false;
		}
	}

	const wchar_t* token1 = account.Get_c_apple_token();
	const bool hasToken1 = (token1[0] != 0);
	const wchar_t* tokenCached1 = accountCached.Get_c_apple_token();
	const bool hasTokenCached1 = (tokenCached1[0] != 0);
	if (hasTokenCached1)
	{
		if (!hasToken1 || wcscmp(token1, tokenCached1) != 0)
		{
			if (mMapByApple.contains(token1))
				return false;
		}
	}

	return true;
}

bool AccountStore::ModifyAccount(const ACCOUNT& account)
{
	bool canModifyAccount = CanModifyAccount(account);
	if (!canModifyAccount)
		return false;

	const AccountId accountId(account.Get_c_account_id());
	AccountPtr accountPtr = FindByAccountId(accountId);
	if (!accountPtr)
		return false;

	const ACCOUNT& accountCached = accountPtr->GetAccount();

	const wchar_t* token0 = account.Get_c_google_token();
	const bool hasToken0 = (token0[0] != 0);
	const wchar_t* tokenCached0 = accountCached.Get_c_google_token();
	const bool hasTokenCached0 = (tokenCached0[0] != 0);

	const wchar_t* token1 = account.Get_c_apple_token();
	const bool hasToken1 = (token1[0] != 0);
	const wchar_t* tokenCached1 = accountCached.Get_c_apple_token();
	const bool hasTokenCached1 = (tokenCached1[0] != 0);

	const wchar_t* token2 = account.Get_c_device_token();
	const bool hasToken2 = (token2[0] != 0);
	const wchar_t* tokenCached2 = accountCached.Get_c_device_token();
	const bool hasTokenCached2 = (tokenCached2[0] != 0);

	if (hasTokenCached0) mMapByGoogle.erase(tokenCached0);
	if (hasTokenCached1) mMapByApple.erase(tokenCached1);
	if (hasTokenCached2) mMapByDevice.erase(tokenCached2);

	if (hasToken0) mMapByGoogle.insert_or_assign(token0, accountPtr);
	if (hasToken1) mMapByApple.insert_or_assign(token1, accountPtr);
	if (hasToken2) mMapByDevice.insert_or_assign(token2, accountPtr);

	MainDbSession mainDbSession(CommitType::AUTO);
	if (false == mainDbSession.IsReady())
	{
		_ASSERT_CRASH(false);
		return false;
	}

	// DB에 Account 토큰 갱신
	{
		SP_ACCOUNT_UPDATE_TOKEN spAccountUpdateToken(
			mainDbSession,
			account.Get_c_account_id(),
			account.Get_c_device_token(),
			account.Get_c_google_token(),
			account.Get_c_apple_token(),
			tClock.GetGlobalNowTs()
		);
		Result result = DbUtil::ExecuteSp(spAccountUpdateToken);
		if (Result::SUCCEEDED != result)
			return false;
	}

	// 갱신된 데이터 읽어서 반영
	{
		SP_ACCOUNT_SELECT spAccountSelect(
			mainDbSession,
			account.Get_c_account_id()
		);
		Result result = DbUtil::FetchSp(spAccountSelect,
			[&]()
			{
				ACCOUNT::Writer wp = DbUtil::SpToPacket(spAccountSelect);
				const ACCOUNT& accountUpdated = wp.GetPacket();

				// 캐시 갱신
				accountPtr->SetAccount(accountUpdated);

				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	return true;
}

bool AccountStore::CanAddAccount(const ACCOUNT& account) const
{
	// DeviceToken이 없음.
	const wchar_t* token2 = account.Get_c_device_token();
	const bool hasToken2 = (token2[0] != 0);
	if (!hasToken2)
		return false;

	// 이미 존재하면 실패
	const wchar_t* token0 = account.Get_c_google_token();
	const bool hasToken0 = (token0[0] != 0);
	if (hasToken0 && mMapByGoogle.contains(token0))
		return false;

	// 이미 존재하면 실패
	const wchar_t* token1 = account.Get_c_apple_token();
	const bool hasToken1 = (token1[0] != 0);
	if (hasToken1 && mMapByApple.contains(token1))
		return false;

	// 기기는 중복되어도 무방.

	return true;
}

bool AccountStore::AddAccountToMap(const AccountPtr& accountPtr)
{
	if (!accountPtr)
		return false;

	const ACCOUNT& account = accountPtr->GetAccount();

	const wchar_t* token0 = account.Get_c_google_token();
	const bool hasToken0 = (token0[0] != 0);
	if (hasToken0)
		mMapByGoogle.insert_or_assign(token0, accountPtr);

	const wchar_t* token1 = account.Get_c_apple_token();
	const bool hasToken1 = (token1[0] != 0);
	if (hasToken1)
		mMapByApple.insert_or_assign(token1, accountPtr);

	const wchar_t* token2 = account.Get_c_device_token();
	const bool hasToken2 = (token2[0] != 0);
	if (hasToken2)
		mMapByDevice.insert_or_assign(token2, accountPtr);

	const DbShardIdx shardIdx = static_cast<DbShardIdx>(account.Get_c_db_shard_idx());
	if ((static_cast<size_t>(shardIdx) != INVALID_DB_SHARD_IDX) &&
		(static_cast<size_t>(shardIdx) < mShardUserDbSize.size()))
	{
		++mShardUserDbSize[shardIdx];
	}

	return true;
}

DbShardIdx AccountStore::GetLightestDb() const
{
	if (mShardUserDbSize.empty())
		return INVALID_DB_SHARD_IDX;

	size_t minCount = SIZE_MAX;
	size_t lightestIdx = SIZE_MAX;
	const size_t listSize = mShardUserDbSize.size();
	for (size_t i = 0; i < listSize; ++i)
	{
		if (minCount > mShardUserDbSize[i])
		{
			minCount = mShardUserDbSize[i];
			lightestIdx = i;
		}
	}

	return static_cast<DbShardIdx>(lightestIdx);
}
