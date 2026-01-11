// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;
import ServerEngineCommon;

#include "json.hpp"


namespace BotMonitorUtil
{
	template<typename _CountArray>
	void CountArrayToJson(IN OUT nlohmann::json& json, const char* tag, const _CountArray& countArray, PacketType packetTypeStart)
	{
		auto jsonCountArray = nlohmann::json::array();

		for (size_t i = 0; i < countArray.size(); ++i)
		{
			if (0 == countArray[i])
				continue;

			const PacketType packetType = static_cast<PacketType>(static_cast<PacketType::PrimitiveType>(i + packetTypeStart + 1));
			
			Buf_char packetTypeStr = StringUtil::w_to_a(GetPacketTypeString(packetType));

			nlohmann::json jsonCountElem;			
			
			jsonCountElem["packetType"] = *packetTypeStr;
			jsonCountElem["count"] = countArray[i];
			
			jsonCountArray.push_back(jsonCountElem);
		}

		json[tag] = jsonCountArray;
	}
}

void BotMonitor::Initialize(size_t maxBotCount)
{
	mBotStatisticsList.resize(maxBotCount);

	mCountArray_MC.fill(0);
	mCountArray_CM.fill(0);

	mCountArray_FC.fill(0);
	mCountArray_CF.fill(0);

	mCountArray_DC.fill(0);
	mCountArray_CD.fill(0);

	mCountArray_GC.fill(0);
	mCountArray_CG.fill(0);

	mCountArray_BC.fill(0);
	mCountArray_CB.fill(0);

	WorkerManager::RunOnTimer(PRINT_INTERVAL, gBotMonitor, this, &BotMonitor::Print_timer_async);
}

void BotMonitor::Print_timer_async()
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	//WriteLock lock(gJsonLock);
	nlohmann::json json;

	auto botStatisticsList = nlohmann::json::array();

	for (const BotStatistics& botStatistics : mBotStatisticsList)
	{
		if (INVALID_BOT_ID == botStatistics.botId)
			continue;

		nlohmann::json botStatisticsElem;

		botStatisticsElem["botId"] = botStatistics.botId;
		botStatisticsElem["sceneId"] = botStatistics.sceneName;
		botStatisticsElem["sceneSeq"] = botStatistics.sceneSeq;
		//_DEBUG_LOG(YELLOW, L"[{}, {}-{}]", botStatistics.botId, GetBotSceneIdString(botStatistics.sceneId), botStatistics.sceneSeq);

		auto eventList = nlohmann::json::array();

		for (size_t y = 0; y < _IDX(BotEventType::MAX); ++y)
		{
			size_t hitCount = botStatistics.hitCount[y];

			if (0 == hitCount)
				continue;

			Buf_char eventTypeStr = StringUtil::w_to_a(GetBotEventTypeString(static_cast<BotEventType>(y)));
			Buf_char durationTarget = StringUtil::w_to_a(GetBotEventTypeString(GetDurationTarget(static_cast<BotEventType>(y))));

			nlohmann::json eventElem;

			eventElem["eventType"] = *eventTypeStr;			
			eventElem["durationTarget"] = *durationTarget;
			eventElem["hitCount"] = hitCount;
			eventElem["avrDuration"] = static_cast<float>(botStatistics.totalDuration[y].count()) / static_cast<float>(hitCount);
			
			eventList.push_back(eventElem);

			//_DEBUG_LOG(YELLOW, L" - {}, {}, {}", GetBotEventTypeString((BotEventType)y), botStatistics.hitCount[i], botStatistics.totalDuration[i] / botStatistics.hitCount[i]);
		}

		botStatisticsElem["eventList"] = eventList;

		botStatisticsList.push_back(botStatisticsElem);
	}

	json["botStatisticsList"] = botStatisticsList;

	BotMonitorUtil::CountArrayToJson(json, "countArray_CM", mCountArray_CM, PacketTypes::CM_PACKET_START);
	BotMonitorUtil::CountArrayToJson(json, "countArray_MC", mCountArray_MC, PacketTypes::MC_PACKET_START);

	BotMonitorUtil::CountArrayToJson(json, "countArray_CF", mCountArray_CF, PacketTypes::CF_PACKET_START);
	BotMonitorUtil::CountArrayToJson(json, "countArray_FC", mCountArray_FC, PacketTypes::FC_PACKET_START);

	BotMonitorUtil::CountArrayToJson(json, "countArray_CD", mCountArray_CD, PacketTypes::CD_PACKET_START);
	BotMonitorUtil::CountArrayToJson(json, "countArray_DC", mCountArray_DC, PacketTypes::DC_PACKET_START);

	BotMonitorUtil::CountArrayToJson(json, "countArray_CG", mCountArray_CG, PacketTypes::CG_PACKET_START);
	BotMonitorUtil::CountArrayToJson(json, "countArray_GC", mCountArray_GC, PacketTypes::GC_PACKET_START);

	BotMonitorUtil::CountArrayToJson(json, "countArray_CB", mCountArray_CB, PacketTypes::CB_PACKET_START);
	BotMonitorUtil::CountArrayToJson(json, "countArray_BC", mCountArray_BC, PacketTypes::BC_PACKET_START);

	auto resultCountArray = nlohmann::json::array();

	for (size_t i = 0; i < mResultCountArray.size(); ++i)
	{
		if (0 == mResultCountArray[i])
			continue;

		Buf_char resultType = StringUtil::w_to_a(GetResultString(static_cast<Result>(i)));

		nlohmann::json resultCountElem;

		resultCountElem["resultType"] = *resultType;
		resultCountElem["count"] = mResultCountArray[i];

		resultCountArray.push_back(resultCountElem);
	}
	json["resultCountArray"] = resultCountArray;

	std::ofstream jsonFile = CreateOutputFileStream(MAKE_BOT_DATA_FILE_PATH("botMonitor.json"));
	jsonFile << std::setw(4) << json << std::endl;

	WorkerManager::RunOnTimer(PRINT_INTERVAL, gBotMonitor, this, &BotMonitor::Print_timer_async);
}

void BotMonitor::OnBotEvent_async(BotEventType eventType, BotId botId, const std::wstring& sceneName, BotSceneSeq sceneSeq, ClockPoint clockPoint)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	BotStatistics& botStatistics = mBotStatisticsList[botId];

	botStatistics.botId = botId;
	botStatistics.sceneName = sceneName;
	botStatistics.sceneSeq = (INVALID_BOT_SCENE_SEQ == sceneSeq) ? botStatistics.sceneSeq : sceneSeq;
	++botStatistics.hitCount[_IDX(eventType)];
	botStatistics.clockPoint[_IDX(eventType)] = clockPoint;

	BotEventType targetType = GetDurationTarget(eventType);
	
	if (BotEventType::NONE != targetType)
	{
		const ClockPoint oldClockPoint = botStatistics.clockPoint[_IDX(targetType)];

		if (ClockPoint::min() != oldClockPoint)
		{
			const ClockDuration duration = clockPoint - oldClockPoint;

			botStatistics.totalDuration[_IDX(eventType)] += duration;
		}
	}
}

void BotMonitor::OnPacketRecv_async(PacketType packetType)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	if (PacketTypes::MC_PACKET_START < packetType && PacketTypes::MC_PACKET_END > packetType)
	{
		const size_t index = _IDX(packetType) - PacketTypes::MC_PACKET_START - 1;

		++mCountArray_MC[index];
	}
	else if (PacketTypes::FC_PACKET_START < packetType && PacketTypes::FC_PACKET_END > packetType)
	{
		const size_t index = _IDX(packetType) - PacketTypes::FC_PACKET_START - 1;

		++mCountArray_FC[index];
	}
	else if (PacketTypes::DC_PACKET_START < packetType && PacketTypes::DC_PACKET_END > packetType)
	{
		const size_t index = _IDX(packetType) - PacketTypes::DC_PACKET_START - 1;
		
		++mCountArray_DC[index];
	}
	else if (PacketTypes::GC_PACKET_START < packetType && PacketTypes::GC_PACKET_END > packetType)
	{
		const size_t index = _IDX(packetType) - PacketTypes::GC_PACKET_START - 1;

		++mCountArray_GC[index];
	}
	else if (PacketTypes::BC_PACKET_START < packetType && PacketTypes::BC_PACKET_END > packetType)
	{
		const size_t index = _IDX(packetType) - PacketTypes::BC_PACKET_START - 1;

		++mCountArray_BC[index];
	}
}

void BotMonitor::OnPacketSend_async(PacketType packetType)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	if (PacketTypes::CM_PACKET_START < packetType && PacketTypes::CM_PACKET_END > packetType)
	{
		const size_t index = _IDX(packetType) - PacketTypes::CM_PACKET_START - 1;

		++mCountArray_CM[index];
	}
	else if (PacketTypes::CF_PACKET_START < packetType && PacketTypes::CF_PACKET_END > packetType)
	{
		const size_t index = _IDX(packetType) - PacketTypes::CF_PACKET_START - 1;

		++mCountArray_CF[index];
	}
	else if (PacketTypes::CD_PACKET_START < packetType && PacketTypes::CD_PACKET_END > packetType)
	{
		const size_t index = _IDX(packetType) - PacketTypes::CD_PACKET_START - 1;

		++mCountArray_CD[index];
	}
	else if (PacketTypes::CG_PACKET_START < packetType && PacketTypes::CG_PACKET_END > packetType)
	{
		const size_t index = _IDX(packetType) - PacketTypes::CG_PACKET_START - 1;

		++mCountArray_CG[index];
	}
	else if (PacketTypes::CB_PACKET_START < packetType && PacketTypes::CB_PACKET_END > packetType)
	{
		const size_t index = _IDX(packetType) - PacketTypes::CB_PACKET_START - 1;

		++mCountArray_CB[index];
	}
}

void BotMonitor::OnPacketResult_async(Result result)
{
	_ASSERT_CRASH(tWorkingWorker == shared_from_this());

	if (Result::SUCCEEDED <= result && Result::MAX > result)
	{
		++mResultCountArray[_IDX(result)];
	}
	else
	{
		_ASSERT_CRASH(false);
	}
}

BotEventType BotMonitor::GetDurationTarget(BotEventType eventType)
{
	if (BotEventType::DESTROYED == eventType) return BotEventType::CREATED;
	if (BotEventType::DISCONNECTED_FRONT == eventType) return BotEventType::CONNECTED_FRONT;
	if (BotEventType::DISCONNECTED_GAME == eventType) return BotEventType::CONNECTED_GAME;
	if (BotEventType::SCENARIO_FINISHED == eventType) return BotEventType::SCENARIO_STARTED;
	if (BotEventType::SCENE_FINISHED == eventType) return BotEventType::SCENE_STARTED;
	if (BotEventType::SEQ_FINISHED == eventType) return BotEventType::SEQ_STARTED;

	return BotEventType::NONE;
}

