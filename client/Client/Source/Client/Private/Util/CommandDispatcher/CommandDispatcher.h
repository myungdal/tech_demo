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

template<typename... _Args>
class CommandDispatcher
{
public:
	using Handler = std::function<void(ArgList&, _Args...)>;
	using HandlerMap = std::unordered_map<std::wstring, std::tuple<std::wstring, std::wstring, Handler>>;

private:
	HandlerMap mHandlerMap;

public:
	void Register(const wchar_t* key, const wchar_t* description, Handler handler)
	{
		std::wstring lowerKey = key;
		std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(),
			[](wchar_t ch) {
				return static_cast<wchar_t>(std::tolower(ch));
			});
		mHandlerMap.emplace(lowerKey, std::make_tuple(key, description, handler));
	}

	void Dispatch(const std::wstring& command, _Args&&... args)
	{
		ArgList argList(command);

		std::wstring key = argList.GetStr(0);
		std::transform(key.begin(), key.end(), key.begin(), 
			[](wchar_t ch) {
				return static_cast<wchar_t>(std::tolower(ch));
			});

		auto it = mHandlerMap.find(key);
		if (mHandlerMap.end() == it)
			return;

		auto& [_key, description, handler] = it->second;
		handler(argList, args...);
	}

public:
	const HandlerMap& GetHandlerMap() const noexcept { return mHandlerMap; }
};

