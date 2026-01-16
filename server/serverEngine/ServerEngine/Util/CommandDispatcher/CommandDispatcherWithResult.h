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
template<typename _ResultType, typename... _Args>
class CommandDispatcherWithResult
{
public:
	using Handler = std::function<_ResultType(ArgList&, _Args...)>;
	using HandlerMap = std::unordered_map<std::wstring, std::tuple<std::wstring, std::wstring, Handler>>;

private:
	HandlerMap mHandlerMap;

public:
	void Register(const wchar_t* key, const wchar_t* description, Handler handler)
	{
		std::wstring lowerKey = key;
		StringUtil::ToLower(lowerKey);
		mHandlerMap.emplace(lowerKey, std::make_tuple(key, description, handler));
	}

	_ResultType Dispatch(const std::wstring& command, _Args&&... args)
	{
		ArgList argList(command);

		std::wstring key = argList.GetStr(0);
		StringUtil::ToLower(key);
		auto it = mHandlerMap.find(key);
		if (mHandlerMap.end() == it)
			return _ResultType{};

		auto& [_key, description, handler] = it->second;
		return handler(argList, args...);
	}

public:
	const HandlerMap& GetHandlerMap() const noexcept { return mHandlerMap; }
};

