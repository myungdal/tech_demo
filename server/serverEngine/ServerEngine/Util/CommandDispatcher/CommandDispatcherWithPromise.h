// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Invoker/ArgList.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CommandDispatcherWithPromise
{
public:
	using Handler = std::function<void(std::promise<std::wstring>&, ArgList&)>;
	using Key = std::wstring;
	using Description = std::wstring;
	using HandlerMap = std::unordered_map<Key, std::tuple<Key, Description, Handler>>;

private:
	HandlerMap mHandlerMap;

public:
	void Register(const wchar_t* key, const wchar_t* description, const Handler& handler);
	bool Dispatch(std::promise<std::wstring>& promise, const std::wstring& command);

public:
	const HandlerMap& GetHandlerMap() const noexcept { return mHandlerMap; }
};
