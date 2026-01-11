// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Shell;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ShellCommandManager::ShellCommandManager()
{
	mCommandDispatcher.Register(
		L"help", L"도움말",
		std::bind(&ShellCommandManager::OnCommand_help, this, std::placeholders::_1)
	);
}
std::wstring ShellCommandManager::Dispatch(const std::wstring& command)
{
	std::wstring result = mCommandDispatcher.Dispatch(command);
	SocketUtil::Request<REQ_SHELL_COMMAND::Writer> wp(*gSocketShellToMain, REQ);
	wp.SetValues(command.data());
	ACK_SHELL_COMMAND* ack = nullptr;
	if (wp.Wait(wp.GetHeader(), OUT ack))
		result += ack->Get_result();
	// [WHY] help 명령의 경우 여러 서버에서 중복된 결과가 올 수 있으므로 중복 제거
	if (command == L"help")
	{
		std::set<std::wstring> commandSet;
		std::list<std::wstring> commandList;
		std::wstringstream ss(result);
		std::wstring line;

		while (std::getline(ss, line))
		{
			auto [it, newly] = commandSet.insert(line);
			if (newly)
				commandList.emplace_back(line);
		}

		result.clear();
		bool first = true;
		for (const auto& cmd : commandList)
		{
			if (!first)
				result += L"\n";
			result += cmd;
			first = false;
		}
	}

	return result;
}

std::wstring ShellCommandManager::OnCommand_help(MAYBE_UNUSED ArgList& argList)
{
	std::wstring result;

	result += L"\n";
	result += L"* Shell 명령:\n";

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
