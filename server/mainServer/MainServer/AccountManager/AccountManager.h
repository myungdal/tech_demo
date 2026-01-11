// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerBase/Worker.h"


class ACCOUNT;
class AccountStore;

class AccountManager : public Worker
{
private:
	// 활성 상태의 Auth 소유자.
	std::shared_ptr<AccountStore> mAccountStore = nullptr;

	// 인증 티켓.
	AuthTicket mCurrentAuthTicket = INVALID_AUTH_TICKET;

public:
	AccountManager();

public:
	bool Load();

public:
	AccountPtr FindOrCreateAccount(const ACCOUNT& account);
	AccountPtr FindAccount(const AccountId accountId);

public:
	bool CanModifyAccount(const ACCOUNT& account);
	bool ModifyAccount(const ACCOUNT& account);
};

inline std::shared_ptr<AccountManager> gAccountManager = nullptr;
