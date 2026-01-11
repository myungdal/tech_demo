// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;

import ServerEngineDb;

Account::Account(const ACCOUNT& account)
	:
	mAccount(account)
{
}
void Account::SetAccount(const ACCOUNT& account)
{
	mAccount = account;
}

const ACCOUNT& Account::GetAccount() const
{
	return **mAccount;
}

const ACCOUNT_USER* Account::CreateAccountUser(const wchar_t* userName)
{
	MainDbSession mainDbSession(CommitType::AUTO);

	SP_ACCOUNT_USER_INSERT sp(mainDbSession,
		mAccount->Ref_c_account_id(),
		INVALID_SERVER_ID,
		UuidUtil::GenerateUuid(),
		static_cast<int64_t>(UserType::NORMAL),
		static_cast<int64_t>(UserState::NORMAL),
		userName,
		tClock.GetGlobalNowTs()
	);
	Result result = DbUtil::ExecuteSp(sp);
	if (Result::SUCCEEDED != result)
		return nullptr;

	ResetAccountUserList();

	const ACCOUNT_USER* accountUser = LoadAccountUser(sp.mOut.p_new_user_id);

	return accountUser;
}

bool Account::DeleteAccountUser(UserId userId)
{
	MainDbSession mainDbSession(CommitType::AUTO);

	SP_ACCOUNT_USER_DELETE sp(mainDbSession,
		UuidUtil::GenerateUuid(),
		userId,
		tClock.GetGlobalNowTs()
	);
	Result result = DbUtil::ExecuteSp(sp);
	if (Result::SUCCEEDED != result)
		return false;

	ResetAccountUserList();

	return true;
}

const ACCOUNT_USER* Account::LoadAccountUser(UserId userId)
{
	const AccountUserList& accountUserList = LoadAccountUserList();

	for (const auto& accountUser : accountUserList)
	{
		if (accountUser->ConstRef_c_user_id() == userId)
			return *accountUser;
	}

	return nullptr;
}

const AccountUserList& Account::LoadAccountUserList()
{
	if (mAccountUserList.empty())
	{
		MainDbSession mainDbSession(CommitType::AUTO);

		SP_ACCOUNT_USER_SELECT_BY_ACCOUNT sp(mainDbSession, mAccount->ConstRef_c_account_id().GetData());
		DbUtil::FetchSp(sp,
			[&]()
			{
				ACCOUNT_USER::Writer wp = DbUtil::SpToPacket(sp);
				mAccountUserList.emplace_back(PacketKeep<ACCOUNT_USER>(wp.GetPacket()));
				return Result::SUCCEEDED;
			}
		);
	}

	return mAccountUserList;
}

void Account::ResetAccountUserList()
{
	mAccountUserList.clear();
}

void Account::SetAuthTicket(AuthTicket authTicket)
{
	mAuthTicket = authTicket;
}

AuthTicket Account::GetAuthTicket() const
{
	return mAuthTicket;
}

