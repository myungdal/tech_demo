// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import GameServer;

#include "MmoSync/Engine/Engine.h"


SocketGameToMain::SocketGameToMain()
{
	mCommandDispatcher.Register(
		L"help", L"도움말",
		std::bind(&SocketGameToMain::OnCommand_help, this, std::placeholders::_1)
	);
	mCommandDispatcher.Register(
		L"print", L"디버깅 파일 출력",
		std::bind(&SocketGameToMain::OnCommand_print, this, std::placeholders::_1)
	);
	mCommandDispatcher.Register(
		L"pause", L"엔진 중단",
		std::bind(&SocketGameToMain::OnCommand_pause, this, std::placeholders::_1)
	);
	mCommandDispatcher.Register(
		L"step", L"[stepCount] 엔진 스텝",
		std::bind(&SocketGameToMain::OnCommand_step, this, std::placeholders::_1)
	);
	mCommandDispatcher.Register(
		L"resume", L"엔진 재개",
		std::bind(&SocketGameToMain::OnCommand_resume, this, std::placeholders::_1)
	);
	mCommandDispatcher.Register(
		L"save", L"엔진 월드데이터 저장",
		std::bind(&SocketGameToMain::OnCommand_save, this, std::placeholders::_1)
	);
	mCommandDispatcher.Register(
		L"rewind", L"엔진 월드데이터 되돌리기",
		std::bind(&SocketGameToMain::OnCommand_rewind, this, std::placeholders::_1)
	);
	mCommandDispatcher.Register(
		L"reloaded", L"db 다시 읽기 적용",
		std::bind(&SocketGameToMain::OnCommand_reloaded, this, std::placeholders::_1)
	);
}
// 이미 락이 걸린 상태에서 들어온다.
void SocketGameToMain::OnAppAdded(IN OUT AppInfo& appInfo)
{
	if (AppType::DB_SERVER == appInfo.GetData().Get_appType())
	{
		SocketGameToDb* socketGameToDb = _NEW<SocketGameToDb>();
		socketGameToDb->SetIocp(mIocp);
		socketGameToDb->Connect(
			appInfo.GetData().Get_ip(),
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_GD"), 
			gGameServerApp->GetMyAppId(),
			gGameServerApp->GetMyAppType(),
			appInfo.GetData().Get_appId(),
			appInfo.GetData().Get_appType()
		);	

		appInfo.SetSocket(socketGameToDb->GetSocketPtr(), L"OnAppAdded");
	}
}

std::wstring SocketGameToMain::OnShellCommand(MAYBE_UNUSED const wchar_t* command)
{
	return mCommandDispatcher.Dispatch(command);
}

std::wstring SocketGameToMain::OnCommand_help(MAYBE_UNUSED ArgList& argList)
{
	std::wstring result;
	result += L"\n";
	result += L"* GameServer 명령:\n";
	for (const auto& [key, handler] : mCommandDispatcher.GetHandlerMap())
	{
		result += L"\t";
		result += std::get<0>(handler);
		result += L"\t";
		result += std::get<1>(handler);
		result += L"\n";
	}
	result += L"\n";
	return result;
}

std::wstring SocketGameToMain::OnCommand_print(MAYBE_UNUSED ArgList& argList)
{
	gGameChannelManager->RunToAllGameChannel(&GameChannel::PrintDebugInfo);

	return L"./client/Client/Saved/debug_step_count\nOK\n";
}

std::wstring SocketGameToMain::OnCommand_pause(MAYBE_UNUSED ArgList& argList)
{
	if (Engine::GetDebugStepCount() != -1)
		return L"FAILED\n";

	Engine::SetDebugStepCount(0);
	return L"OK\n";
}

std::wstring SocketGameToMain::OnCommand_step(MAYBE_UNUSED ArgList& argList)
{
	if (Engine::GetDebugStepCount() == -1)
		return L"FAILED\n";

	const int32_t step = argList.GetInt<int32_t>(1, int32_t(1));
	
	Engine::SetDebugStepCount(step);
	return L"OK\n";
}

std::wstring SocketGameToMain::OnCommand_resume(MAYBE_UNUSED ArgList& argList)
{
	if (Engine::GetDebugStepCount() == -1)
		return L"FAILED\n";

	Engine::SetDebugStepCount(-1);
	return L"OK\n";
}

std::wstring SocketGameToMain::OnCommand_save(MAYBE_UNUSED ArgList& argList)
{
	if (Engine::GetDebugStepCount() == -1)
		return L"FAILED\n";

	gGameChannelManager->RunToAllGameChannel(&GameChannel::SaveDebugWorldData);
	return L"OK\n";
}

std::wstring SocketGameToMain::OnCommand_rewind(MAYBE_UNUSED ArgList& argList)
{
	if (Engine::GetDebugStepCount() == -1)
		return L"FAILED\n";

	gGameChannelManager->RunToAllGameChannel(&GameChannel::LoadDebugWorldData);
	return L"OK\n";
}

std::wstring SocketGameToMain::OnCommand_reloaded(MAYBE_UNUSED ArgList& argList)
{
	//if (Engine::GetDebugStepCount() == -1)
	//	return L"FAILED\n";

	// DB 정적 데이터를 다시 읽어 온다
	auto [appInfo, lock] = gAppListManager->PickDbServerAppInfo();
	if (appInfo)
	{
		SocketBasePtr socket = appInfo->GetSocket();
		auto* socketGameToDb = static_cast<SocketGameToDb*>(*socket);
		SocketPtr<SocketGameToDb> ptr(socketGameToDb, L"SendReqStaticDataChecksum_timer");
		WorkerManager::RunOnTimer(1000ms, gTimerWorker->GetTimerWorker(), &*gGamePacketHandlerStaticData, &GamePacketHandlerStaticData::SendReqStaticDataChecksum_timer, ptr);
	}

	return L"OK\n";
}

bool SocketGameToMain::OnDispatchPacket(NetworkPacket& rp)
{
	DispatchPacket(rp);
	return true;
}

void SocketGameToMain::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case INVALID_PACKET_TYPE:
		break;
	default:
		const GameId gameId = rp.GetHeader().GetGameId();

		const GameChannelIndex gameChannelIndex = rp.GetHeader().GetGameChannelIndex();
		if (INVALID_GAME_CHANNEL_INDEX != gameChannelIndex)
		{
			if (GameChannelPtr gameChannel = gGameChannelManager->GetGameChannel(gameChannelIndex))
			{
				if (gameChannel->GetGameId() == gameId)
					PacketUtil::DispatchPacketOnWorker(gameChannel, this, &SocketGameToMain::DispatchPacket_async, rp);
				else
					_DEBUG_RED;
			}
			else
				_DEBUG_RED;
		}
		else if (INVALID_UUID != gameId)
		{
			if (GameChannelPtr gameChannel = gGameChannelManager->FindGameChannel(gameId))
			{
				rp.GetHeader().SetGameChannelIndex(gameChannel->GetGameChannelIndex());

				PacketUtil::DispatchPacketOnWorker(gameChannel, this, &SocketGameToMain::DispatchPacket_async, rp);
			}
			else
				_DEBUG_RED;
		}
		else if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketGameToMain::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;
	}
}

void SocketGameToMain::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketGameToMain::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketMG(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gGamePacketHandlerGame.get(), rp)) return;
	if (DispatchPacketToHandler(gGamePacketHandlerSystem.get(), rp)) return;

	if (DispatchPacketMG<MainPeerSocket>(mSocket, this, rp) != HandleResult::NOT_EXISTS) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

