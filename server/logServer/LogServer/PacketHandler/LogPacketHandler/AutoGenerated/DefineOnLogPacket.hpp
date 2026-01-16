// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

HandleResult LogPacketHandler::OnPacket(INSERT_LOG_ACHIEVEMENT& rp, MAYBE_UNUSED LogPacketWorkerPtr worker)
{
   LogDbSession dbSession(CommitType::AUTO);

	SP_LOG_ACHIEVEMENT_INSERT sp(
       dbSession,
       rp.Get_c_id(),
       rp.Get_c_log_id(),
       rp.Get_c_user_id(),
       rp.Get_c_achievement_sid(),
       rp.Get_c_achievement_step_sid(),
       TimeUtil::TS_FROM_TT(rp.Get_c_date_created()),
       TimeUtil::TS_FROM_TT(rp.Get_c_date_log())
	);

	DbUtil::ExecuteSpWithoutResult(sp);

	return HandleResult::OK;
}

