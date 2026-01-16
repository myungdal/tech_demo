// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

struct BotStatistics
{
	BotId botId = INVALID_BOT_ID;
	std::wstring sceneName = L"";
	BotSceneSeq sceneSeq = INVALID_BOT_SCENE_SEQ;

	std::array<size_t, _IDX(BotEventType::MAX)> hitCount = { 0 };
	std::array<ClockPoint, _IDX(BotEventType::MAX)> clockPoint = { ClockPoint::min() };
	std::array<ClockDuration, _IDX(BotEventType::MAX)> totalDuration = { 0s };
};
