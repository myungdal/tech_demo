// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;
import BotUtil;

// BotScenarioUtil.h는 BotUtil.ixx 모듈에서 이미 include/export됨 - 직접 include 시 중복 정의 오류 발생


bool ValidatePacket(NetworkPacket& rp)
{
	if ((false == ValidatePacketFC(rp)) ||
		(false == ValidatePacketDC(rp)) ||
		(false == ValidatePacketGC(rp)) ||
		(false == ValidatePacketBC(rp)))
	{
		_DEBUG_RED;
		_DEBUG_BREAK;
		return false;
	}

	return true;
}

