// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "CommandDispatcherWithPromise.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CommandDispatcherWithPromise::Register(const wchar_t* key, const wchar_t* description, const Handler& handler)
{
	std::wstring lowerKey = key;
	StringUtil::ToLower(lowerKey);
	mHandlerMap.emplace(lowerKey, std::make_tuple(key, description, handler));
}

bool CommandDispatcherWithPromise::Dispatch(std::promise<std::wstring>& promise, const std::wstring& command)
{
	ArgList argList(command);

	std::wstring key = argList.GetStr(0);
	StringUtil::ToLower(key);
	auto it = mHandlerMap.find(key);
	if (mHandlerMap.end() == it)
		return false;

	auto& [_key, description, handler] = it->second;
	handler(promise, argList);

	return true;
}
