// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import FrontServer;


void FrontUser::SetUser(const ACCOUNT_USER& accountUser, const USER& user)
{
	mAccountUser = accountUser;
	mUser = user;
}

void FrontUser::SetClientSocket(SocketPtr<SocketFrontFromClient> socket)
{
	mClientSocketWeakPtr.Set(socket, L"FrontUser");
}

void FrontUser::SetGameSocket(SocketPtr<SocketFrontToGame> socket)
{	
	mGameSocketWeakPtr.Set(socket, L"FrontUser");
}

SocketPtr<SocketFrontFromClient> FrontUser::GetClientSocket() 
{ 
	return mClientSocketWeakPtr.Lock(); 
}

SocketPtr<SocketFrontToGame> FrontUser::GetGameSocket() 
{ 
	return mGameSocketWeakPtr.Lock(); 
}
