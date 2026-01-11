// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class ACCOUNT;

class AccountStore
{
private:
	AccountMapByAccountId mMapByAccountId;

	AccountMapByToken mMapByDevice;
	AccountMapByToken mMapByGoogle;
	AccountMapByToken mMapByApple;

private:
	using DbSizeList = std::vector<size_t>;

	// DB 샤드별 사이즈.
	DbSizeList mShardUserDbSize;

public:
	bool Load();

public:
	AccountPtr SetAuthTicket(const ACCOUNT& account, AuthTicket authTicket);
	AccountPtr FindAccountAndSetAuthTicket(const ACCOUNT& account, AuthTicket authTicket);
	AccountPtr CreateAccount(const ACCOUNT& account, AuthTicket authTicket);
	AccountPtr FindByAccountId(const AccountId accountId);

	bool CanModifyAccount(const ACCOUNT& account);
	bool ModifyAccount(const ACCOUNT& account);

private:
	bool CanAddAccount(const ACCOUNT& account) const;
	bool AddAccountToMap(const AccountPtr& accountPtr);
	DbShardIdx GetLightestDb() const;
};
