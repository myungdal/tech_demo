// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;
import ServerEngineCommon;


AccountManager::AccountManager()
{
	mAccountStore = std::make_shared<AccountStore>();
	// tRandomEngine64 생성 전이어서 직접 만들어서 실행.
	Mt19937Random64 randomEngine64;
	mCurrentAuthTicket = randomEngine64();
}

bool AccountManager::Load()
{
	return mAccountStore->Load();
}

AccountPtr AccountManager::FindOrCreateAccount(const ACCOUNT& account)
{	
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());
	
	// 인증 시도
	if (account.Get_c_account_id() != INVALID_UUID)
	{
		AccountPtr accountPtr = mAccountStore->SetAuthTicket(account, ++mCurrentAuthTicket);
		if (nullptr != accountPtr)
			return accountPtr;
	}

	{
		// 찾아서 인증
		AccountPtr accountPtr = mAccountStore->FindAccountAndSetAuthTicket(account, ++mCurrentAuthTicket);
		if (nullptr != accountPtr)
			return accountPtr;
	}

	{
		// 만들어서 반환
		AccountPtr accountPtr = mAccountStore->CreateAccount(account, ++mCurrentAuthTicket);
		return accountPtr;
	}
}

AccountPtr AccountManager::FindAccount(const AccountId accountId)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());
	
	AccountPtr accountPtr = mAccountStore->FindByAccountId(accountId);
	return accountPtr;
}

bool AccountManager::CanModifyAccount(const ACCOUNT& account)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	const bool result = mAccountStore->CanModifyAccount(account);
	return result;
}

bool AccountManager::ModifyAccount(const ACCOUNT& account)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	const bool result = mAccountStore->ModifyAccount(account);
	return result;
}

