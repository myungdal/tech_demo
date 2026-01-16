// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


// 씬 이름으로 BotScene 인스턴스를 생성하는 팩토리
struct BotSceneFactory
{
	// sceneName에 해당하는 BotScene 파생 클래스 인스턴스 생성
	static std::shared_ptr<class BotScene> CreateScene(BotPtr bot, const std::wstring& sceneName);
};
