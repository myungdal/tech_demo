// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;

#include "BotSceneFactory.h"


template <typename Scene>
std::pair<std::string, std::function<std::shared_ptr<BotScene>(BotPtr)>> RegisterFunc()
{
	return
	{
		std::string(typeid(Scene).name()),
		[](BotPtr bot) -> std::shared_ptr<BotScene>
		{
			return std::make_shared<Scene>(bot);
		}
	};
}

std::shared_ptr<BotScene> BotSceneFactory::CreateScene(BotPtr bot, const std::wstring& sceneName)
{
	static std::unordered_map<std::string, std::function<std::shared_ptr<BotScene>(BotPtr)>> funcs
	{
		RegisterFunc<BotSceneReconnect>(),
		RegisterFunc<BotSceneLogin>(),
		RegisterFunc<BotSceneGame>(),
		RegisterFunc<BotSceneMail>(),
		RegisterFunc<BotSceneItem>(),
		RegisterFunc<BotSceneDisconnect>()
	};
	
	std::string sceneClassName = std::format("class {}", *StringUtil::w_to_a(sceneName.data()));

	auto it = funcs.find(sceneClassName);
	if (funcs.end() != it)
	{
		return it->second(bot);
	}
	else
	{
		_ASSERT_CRASH(false);
	}

	return nullptr;
}
