// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

/*
static STATIC_ACHIEVEMENT::Writer SpToPacket(const class SP_STATIC_ACHIEVEMENT_SELECT& sp);
static STATIC_ACHIEVEMENT::Writer SpToPacket(const class SP_STATIC_ACHIEVEMENT_SELECT_ALL& sp);
static STATIC_ACHIEVEMENT_STEP::Writer SpToPacket(const class SP_STATIC_ACHIEVEMENT_STEP_SELECT& sp);
static STATIC_ACHIEVEMENT_STEP::Writer SpToPacket(const class SP_STATIC_ACHIEVEMENT_STEP_SELECT_ALL& sp);
static STATIC_BUILDING::Writer SpToPacket(const class SP_STATIC_BUILDING_SELECT& sp);
static STATIC_BUILDING::Writer SpToPacket(const class SP_STATIC_BUILDING_SELECT_ALL& sp);
static STATIC_CHARACTER::Writer SpToPacket(const class SP_STATIC_CHARACTER_SELECT& sp);
static STATIC_CHARACTER::Writer SpToPacket(const class SP_STATIC_CHARACTER_SELECT_ALL& sp);
static STATIC_CHARACTER_SKILL::Writer SpToPacket(const class SP_STATIC_CHARACTER_SKILL_SELECT& sp);
static STATIC_CHARACTER_SKILL::Writer SpToPacket(const class SP_STATIC_CHARACTER_SKILL_SELECT_ALL& sp);
static STATIC_GAME::Writer SpToPacket(const class SP_STATIC_GAME_SELECT& sp);
static STATIC_GAME::Writer SpToPacket(const class SP_STATIC_GAME_SELECT_ALL& sp);
static STATIC_ITEM::Writer SpToPacket(const class SP_STATIC_ITEM_SELECT& sp);
static STATIC_ITEM::Writer SpToPacket(const class SP_STATIC_ITEM_SELECT_ALL& sp);
static STATIC_ITEM_STAT::Writer SpToPacket(const class SP_STATIC_ITEM_STAT_SELECT& sp);
static STATIC_ITEM_STAT::Writer SpToPacket(const class SP_STATIC_ITEM_STAT_SELECT_ALL& sp);
static STATIC_MAIL::Writer SpToPacket(const class SP_STATIC_MAIL_SELECT& sp);
static STATIC_MAIL::Writer SpToPacket(const class SP_STATIC_MAIL_SELECT_ALL& sp);
static STATIC_MAP::Writer SpToPacket(const class SP_STATIC_MAP_SELECT& sp);
static STATIC_MAP::Writer SpToPacket(const class SP_STATIC_MAP_SELECT_ALL& sp);
static STATIC_MISSION::Writer SpToPacket(const class SP_STATIC_MISSION_SELECT& sp);
static STATIC_MISSION::Writer SpToPacket(const class SP_STATIC_MISSION_SELECT_ALL& sp);
static STATIC_PRODUCT::Writer SpToPacket(const class SP_STATIC_PRODUCT_SELECT& sp);
static STATIC_PRODUCT::Writer SpToPacket(const class SP_STATIC_PRODUCT_SELECT_ALL& sp);
static STATIC_QUEST::Writer SpToPacket(const class SP_STATIC_QUEST_SELECT& sp);
static STATIC_QUEST::Writer SpToPacket(const class SP_STATIC_QUEST_SELECT_ALL& sp);
static STATIC_REWARD_GROUP::Writer SpToPacket(const class SP_STATIC_REWARD_GROUP_SELECT& sp);
static STATIC_REWARD_GROUP::Writer SpToPacket(const class SP_STATIC_REWARD_GROUP_SELECT_ALL& sp);
static STATIC_REWARD_RATIO::Writer SpToPacket(const class SP_STATIC_REWARD_RATIO_SELECT& sp);
static STATIC_REWARD_RATIO::Writer SpToPacket(const class SP_STATIC_REWARD_RATIO_SELECT_ALL& sp);
static STATIC_REWARD::Writer SpToPacket(const class SP_STATIC_REWARD_SELECT& sp);
static STATIC_REWARD::Writer SpToPacket(const class SP_STATIC_REWARD_SELECT_ALL& sp);
static STATIC_SCHEDULE::Writer SpToPacket(const class SP_STATIC_SCHEDULE_SELECT& sp);
static STATIC_SCHEDULE::Writer SpToPacket(const class SP_STATIC_SCHEDULE_SELECT_ALL& sp);
static STATIC_STAT::Writer SpToPacket(const class SP_STATIC_STAT_SELECT& sp);
static STATIC_STAT::Writer SpToPacket(const class SP_STATIC_STAT_SELECT_ALL& sp);
static STATIC_STOCK::Writer SpToPacket(const class SP_STATIC_STOCK_SELECT& sp);
static STATIC_STOCK::Writer SpToPacket(const class SP_STATIC_STOCK_SELECT_ALL& sp);
static STATIC_TRANSLATION::Writer SpToPacket(const class SP_STATIC_TRANSLATION_SELECT& sp);
static STATIC_TRANSLATION::Writer SpToPacket(const class SP_STATIC_TRANSLATION_SELECT_ALL& sp);
static STATIC_USER_EXP::Writer SpToPacket(const class SP_STATIC_USER_EXP_SELECT& sp);
static STATIC_USER_EXP::Writer SpToPacket(const class SP_STATIC_USER_EXP_SELECT_ALL& sp);
static LOG_ACHIEVEMENT::Writer SpToPacket(const class SP_LOG_ACHIEVEMENT_SELECT& sp);
static LOG_ACHIEVEMENT::Writer SpToPacket(const class SP_LOG_ACHIEVEMENT_SELECT_ALL& sp);
static ACCOUNT_DELETED::Writer SpToPacket(const class SP_ACCOUNT_DELETED_SELECT& sp);
static ACCOUNT_DELETED::Writer SpToPacket(const class SP_ACCOUNT_DELETED_SELECT_ALL& sp);
static ACCOUNT::Writer SpToPacket(const class SP_ACCOUNT_SELECT& sp);
static ACCOUNT::Writer SpToPacket(const class SP_ACCOUNT_SELECT_ALL& sp);
static ACCOUNT_USER_DELETED::Writer SpToPacket(const class SP_ACCOUNT_USER_DELETED_SELECT& sp);
static ACCOUNT_USER_DELETED::Writer SpToPacket(const class SP_ACCOUNT_USER_DELETED_SELECT_ALL& sp);
static ACCOUNT_USER_DELETED_BY_ACCOUNT::Writer SpToPacket(const class SP_ACCOUNT_USER_DELETED_SELECT_BY_ACCOUNT& sp);
static ACCOUNT_USER_GAME::Writer SpToPacket(const class SP_ACCOUNT_USER_GAME_SELECT& sp);
static ACCOUNT_USER_GAME::Writer SpToPacket(const class SP_ACCOUNT_USER_GAME_SELECT_ALL& sp);
static ACCOUNT_USER::Writer SpToPacket(const class SP_ACCOUNT_USER_SELECT& sp);
static ACCOUNT_USER::Writer SpToPacket(const class SP_ACCOUNT_USER_SELECT_ALL& sp);
static ACCOUNT_USER_BY_ACCOUNT::Writer SpToPacket(const class SP_ACCOUNT_USER_SELECT_BY_ACCOUNT& sp);
static GAME_DELETED::Writer SpToPacket(const class SP_GAME_DELETED_SELECT& sp);
static GAME_DELETED::Writer SpToPacket(const class SP_GAME_DELETED_SELECT_ALL& sp);
static GAME::Writer SpToPacket(const class SP_GAME_SELECT& sp);
static GAME::Writer SpToPacket(const class SP_GAME_SELECT_ALL& sp);
static PURCHASE_DELETED::Writer SpToPacket(const class SP_PURCHASE_DELETED_SELECT& sp);
static PURCHASE_DELETED::Writer SpToPacket(const class SP_PURCHASE_DELETED_SELECT_ALL& sp);
static PURCHASE::Writer SpToPacket(const class SP_PURCHASE_SELECT& sp);
static PURCHASE::Writer SpToPacket(const class SP_PURCHASE_SELECT_ALL& sp);
static SERVER::Writer SpToPacket(const class SP_SERVER_SELECT& sp);
static SERVER::Writer SpToPacket(const class SP_SERVER_SELECT_ALL& sp);
static SERVER_STATISTICS::Writer SpToPacket(const class SP_SERVER_STATISTICS_SELECT& sp);
static SERVER_STATISTICS::Writer SpToPacket(const class SP_SERVER_STATISTICS_SELECT_ALL& sp);
static ACHIEVEMENT::Writer SpToPacket(const class SP_ACHIEVEMENT_SELECT& sp);
static ACHIEVEMENT::Writer SpToPacket(const class SP_ACHIEVEMENT_SELECT_ALL& sp);
static ACHIEVEMENT_BY_USER::Writer SpToPacket(const class SP_ACHIEVEMENT_SELECT_BY_USER& sp);
static ACTIVITY::Writer SpToPacket(const class SP_ACTIVITY_SELECT& sp);
static ACTIVITY::Writer SpToPacket(const class SP_ACTIVITY_SELECT_ALL& sp);
static ACTIVITY_BY_USER::Writer SpToPacket(const class SP_ACTIVITY_SELECT_BY_USER& sp);
static ITEM_DELETED::Writer SpToPacket(const class SP_ITEM_DELETED_SELECT& sp);
static ITEM_DELETED::Writer SpToPacket(const class SP_ITEM_DELETED_SELECT_ALL& sp);
static ITEM::Writer SpToPacket(const class SP_ITEM_SELECT& sp);
static ITEM::Writer SpToPacket(const class SP_ITEM_SELECT_ALL& sp);
static ITEM_BY_USER::Writer SpToPacket(const class SP_ITEM_SELECT_BY_USER& sp);
static ITEM_STAT::Writer SpToPacket(const class SP_ITEM_STAT_SELECT& sp);
static ITEM_STAT::Writer SpToPacket(const class SP_ITEM_STAT_SELECT_ALL& sp);
static ITEM_STAT_BY_USER::Writer SpToPacket(const class SP_ITEM_STAT_SELECT_BY_USER& sp);
static MAIL_DELETED::Writer SpToPacket(const class SP_MAIL_DELETED_SELECT& sp);
static MAIL_DELETED::Writer SpToPacket(const class SP_MAIL_DELETED_SELECT_ALL& sp);
static MAIL::Writer SpToPacket(const class SP_MAIL_SELECT& sp);
static MAIL::Writer SpToPacket(const class SP_MAIL_SELECT_ALL& sp);
static MAIL_BY_USER::Writer SpToPacket(const class SP_MAIL_SELECT_BY_USER& sp);
static MISSION_DELETED::Writer SpToPacket(const class SP_MISSION_DELETED_SELECT& sp);
static MISSION_DELETED::Writer SpToPacket(const class SP_MISSION_DELETED_SELECT_ALL& sp);
static MISSION::Writer SpToPacket(const class SP_MISSION_SELECT& sp);
static MISSION::Writer SpToPacket(const class SP_MISSION_SELECT_ALL& sp);
static MISSION_BY_USER::Writer SpToPacket(const class SP_MISSION_SELECT_BY_USER& sp);
static QUEST_DELETED::Writer SpToPacket(const class SP_QUEST_DELETED_SELECT& sp);
static QUEST_DELETED::Writer SpToPacket(const class SP_QUEST_DELETED_SELECT_ALL& sp);
static QUEST::Writer SpToPacket(const class SP_QUEST_SELECT& sp);
static QUEST::Writer SpToPacket(const class SP_QUEST_SELECT_ALL& sp);
static QUEST_BY_USER::Writer SpToPacket(const class SP_QUEST_SELECT_BY_USER& sp);
static USER_POS::Writer SpToPacket(const class SP_USER_POS_SELECT& sp);
static USER_POS::Writer SpToPacket(const class SP_USER_POS_SELECT_ALL& sp);
static USER::Writer SpToPacket(const class SP_USER_SELECT& sp);
static USER::Writer SpToPacket(const class SP_USER_SELECT_ALL& sp);
static USER_BY_ACCOUNT::Writer SpToPacket(const class SP_USER_SELECT_BY_ACCOUNT& sp);



STATIC_ACHIEVEMENT::Writer DbUtil::SpToPacket(const SP_STATIC_ACHIEVEMENT_SELECT& sp)
{
	return STATIC_ACHIEVEMENT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_achievement_sid,
		sp.mOut.c_name,
		sp.mOut.c_condition_script
	);
}

STATIC_ACHIEVEMENT::Writer DbUtil::SpToPacket(const SP_STATIC_ACHIEVEMENT_SELECT_ALL& sp)
{
	return STATIC_ACHIEVEMENT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_achievement_sid,
		sp.mOut.c_name,
		sp.mOut.c_condition_script
	);
}

STATIC_ACHIEVEMENT_STEP::Writer DbUtil::SpToPacket(const SP_STATIC_ACHIEVEMENT_STEP_SELECT& sp)
{
	return STATIC_ACHIEVEMENT_STEP::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_achievement_sid,
		sp.mOut.c_achievement_step,
		sp.mOut.c_name,
		sp.mOut.c_condition_script,
		sp.mOut.c_action_script
	);
}

STATIC_ACHIEVEMENT_STEP::Writer DbUtil::SpToPacket(const SP_STATIC_ACHIEVEMENT_STEP_SELECT_ALL& sp)
{
	return STATIC_ACHIEVEMENT_STEP::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_achievement_sid,
		sp.mOut.c_achievement_step,
		sp.mOut.c_name,
		sp.mOut.c_condition_script,
		sp.mOut.c_action_script
	);
}

STATIC_BUILDING::Writer DbUtil::SpToPacket(const SP_STATIC_BUILDING_SELECT& sp)
{
	return STATIC_BUILDING::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_building_item_sid,
		GetBuildingTypeEnum(sp.mOut.c_building_type)
	);
}

STATIC_BUILDING::Writer DbUtil::SpToPacket(const SP_STATIC_BUILDING_SELECT_ALL& sp)
{
	return STATIC_BUILDING::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_building_item_sid,
		GetBuildingTypeEnum(sp.mOut.c_building_type)
	);
}

STATIC_CHARACTER::Writer DbUtil::SpToPacket(const SP_STATIC_CHARACTER_SELECT& sp)
{
	return STATIC_CHARACTER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_character_item_sid,
		sp.mOut.c_name,
		GetCharacterTypeEnum(sp.mOut.c_character_type),
		sp.mOut.c_character_color,
		GetTacticsTypeEnum(sp.mOut.c_tactics_type),
		GetGridMoverTypeEnum(sp.mOut.c_grid_mover_type),
		sp.mOut.c_physics_size,
		sp.mOut.c_skill_distance_best_min,
		sp.mOut.c_skill_distance_best_max,
		sp.mOut.c_hp_threshold_flee,
		sp.mOut.c_hp_threshold_combat,
		sp.mOut.c_hp_threshold_last_stand
	);
}

STATIC_CHARACTER::Writer DbUtil::SpToPacket(const SP_STATIC_CHARACTER_SELECT_ALL& sp)
{
	return STATIC_CHARACTER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_character_item_sid,
		sp.mOut.c_name,
		GetCharacterTypeEnum(sp.mOut.c_character_type),
		sp.mOut.c_character_color,
		GetTacticsTypeEnum(sp.mOut.c_tactics_type),
		GetGridMoverTypeEnum(sp.mOut.c_grid_mover_type),
		sp.mOut.c_physics_size,
		sp.mOut.c_skill_distance_best_min,
		sp.mOut.c_skill_distance_best_max,
		sp.mOut.c_hp_threshold_flee,
		sp.mOut.c_hp_threshold_combat,
		sp.mOut.c_hp_threshold_last_stand
	);
}

STATIC_CHARACTER_SKILL::Writer DbUtil::SpToPacket(const SP_STATIC_CHARACTER_SKILL_SELECT& sp)
{
	return STATIC_CHARACTER_SKILL::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_character_item_sid,
		sp.mOut.c_name,
		GetSkillComponentTypeEnum(sp.mOut.c_skill_component_type),
		sp.mOut.c_skill_cost_sp,
		sp.mOut.c_skill_cooltime,
		sp.mOut.c_skill_distance_min,
		sp.mOut.c_skill_distance_max,
		sp.mOut.c_skill_motion_duration,
		sp.mOut.c_skill_power
	);
}

STATIC_CHARACTER_SKILL::Writer DbUtil::SpToPacket(const SP_STATIC_CHARACTER_SKILL_SELECT_ALL& sp)
{
	return STATIC_CHARACTER_SKILL::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_character_item_sid,
		sp.mOut.c_name,
		GetSkillComponentTypeEnum(sp.mOut.c_skill_component_type),
		sp.mOut.c_skill_cost_sp,
		sp.mOut.c_skill_cooltime,
		sp.mOut.c_skill_distance_min,
		sp.mOut.c_skill_distance_max,
		sp.mOut.c_skill_motion_duration,
		sp.mOut.c_skill_power
	);
}

STATIC_GAME::Writer DbUtil::SpToPacket(const SP_STATIC_GAME_SELECT& sp)
{
	return STATIC_GAME::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_game_sid,
		sp.mOut.c_name,
		GetGameTypeEnum(sp.mOut.c_game_type),
		sp.mOut.c_map_sid
	);
}

STATIC_GAME::Writer DbUtil::SpToPacket(const SP_STATIC_GAME_SELECT_ALL& sp)
{
	return STATIC_GAME::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_game_sid,
		sp.mOut.c_name,
		GetGameTypeEnum(sp.mOut.c_game_type),
		sp.mOut.c_map_sid
	);
}

STATIC_ITEM::Writer DbUtil::SpToPacket(const SP_STATIC_ITEM_SELECT& sp)
{
	return STATIC_ITEM::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_item_sid,
		sp.mOut.c_name,
		GetItemTypeEnum(sp.mOut.c_item_type),
		sp.mOut.c_item_grade,
		sp.mOut.c_max_quantity,
		sp.mOut.c_item_tag,
		sp.mOut.c_use_condition_script,
		sp.mOut.c_use_action_script,
		sp.mOut.c_uasset_icon_img
	);
}

STATIC_ITEM::Writer DbUtil::SpToPacket(const SP_STATIC_ITEM_SELECT_ALL& sp)
{
	return STATIC_ITEM::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_item_sid,
		sp.mOut.c_name,
		GetItemTypeEnum(sp.mOut.c_item_type),
		sp.mOut.c_item_grade,
		sp.mOut.c_max_quantity,
		sp.mOut.c_item_tag,
		sp.mOut.c_use_condition_script,
		sp.mOut.c_use_action_script,
		sp.mOut.c_uasset_icon_img
	);
}

STATIC_ITEM_STAT::Writer DbUtil::SpToPacket(const SP_STATIC_ITEM_STAT_SELECT& sp)
{
	return STATIC_ITEM_STAT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_item_sid,
		sp.mOut.c_name,
		GetStatTypeEnum(sp.mOut.c_stat_type),
		sp.mOut.c_stat_value
	);
}

STATIC_ITEM_STAT::Writer DbUtil::SpToPacket(const SP_STATIC_ITEM_STAT_SELECT_ALL& sp)
{
	return STATIC_ITEM_STAT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_item_sid,
		sp.mOut.c_name,
		GetStatTypeEnum(sp.mOut.c_stat_type),
		sp.mOut.c_stat_value
	);
}

STATIC_MAIL::Writer DbUtil::SpToPacket(const SP_STATIC_MAIL_SELECT& sp)
{
	return STATIC_MAIL::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_mail_sid,
		GetMailTypeEnum(sp.mOut.c_mail_type),
		sp.mOut.c_title,
		sp.mOut.c_msg,
		sp.mOut.c_action_script
	);
}

STATIC_MAIL::Writer DbUtil::SpToPacket(const SP_STATIC_MAIL_SELECT_ALL& sp)
{
	return STATIC_MAIL::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_mail_sid,
		GetMailTypeEnum(sp.mOut.c_mail_type),
		sp.mOut.c_title,
		sp.mOut.c_msg,
		sp.mOut.c_action_script
	);
}

STATIC_MAP::Writer DbUtil::SpToPacket(const SP_STATIC_MAP_SELECT& sp)
{
	return STATIC_MAP::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_map_sid,
		sp.mOut.c_name,
		sp.mOut.c_tile_theme,
		sp.mOut.c_grid_col_count_factor,
		sp.mOut.c_grid_row_count_factor,
		sp.mOut.c_level_division0,
		sp.mOut.c_level_division1,
		sp.mOut.c_level_division2,
		sp.mOut.c_level_division3,
		sp.mOut.c_level_division4,
		sp.mOut.c_level_division5,
		sp.mOut.c_level_division6,
		sp.mOut.c_fractal_scale_x0,
		sp.mOut.c_fractal_scale_x1,
		sp.mOut.c_fractal_scale_x2,
		sp.mOut.c_fractal_scale_x3,
		sp.mOut.c_fractal_scale_y0,
		sp.mOut.c_fractal_scale_y1,
		sp.mOut.c_fractal_scale_y2,
		sp.mOut.c_fractal_scale_y3,
		sp.mOut.c_fractal_scale_w0,
		sp.mOut.c_fractal_scale_w1,
		sp.mOut.c_fractal_scale_w2,
		sp.mOut.c_fractal_scale_w3,
		sp.mOut.c_random_seed,
		sp.mOut.c_initial_system_group_name,
		sp.mOut.c_initial_system_group_fsm_state_name
	);
}

STATIC_MAP::Writer DbUtil::SpToPacket(const SP_STATIC_MAP_SELECT_ALL& sp)
{
	return STATIC_MAP::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_map_sid,
		sp.mOut.c_name,
		sp.mOut.c_tile_theme,
		sp.mOut.c_grid_col_count_factor,
		sp.mOut.c_grid_row_count_factor,
		sp.mOut.c_level_division0,
		sp.mOut.c_level_division1,
		sp.mOut.c_level_division2,
		sp.mOut.c_level_division3,
		sp.mOut.c_level_division4,
		sp.mOut.c_level_division5,
		sp.mOut.c_level_division6,
		sp.mOut.c_fractal_scale_x0,
		sp.mOut.c_fractal_scale_x1,
		sp.mOut.c_fractal_scale_x2,
		sp.mOut.c_fractal_scale_x3,
		sp.mOut.c_fractal_scale_y0,
		sp.mOut.c_fractal_scale_y1,
		sp.mOut.c_fractal_scale_y2,
		sp.mOut.c_fractal_scale_y3,
		sp.mOut.c_fractal_scale_w0,
		sp.mOut.c_fractal_scale_w1,
		sp.mOut.c_fractal_scale_w2,
		sp.mOut.c_fractal_scale_w3,
		sp.mOut.c_random_seed,
		sp.mOut.c_initial_system_group_name,
		sp.mOut.c_initial_system_group_fsm_state_name
	);
}

STATIC_MISSION::Writer DbUtil::SpToPacket(const SP_STATIC_MISSION_SELECT& sp)
{
	return STATIC_MISSION::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_mission_sid,
		sp.mOut.c_name,
		sp.mOut.c_condition_script,
		sp.mOut.c_ratio,
		sp.mOut.c_action_script
	);
}

STATIC_MISSION::Writer DbUtil::SpToPacket(const SP_STATIC_MISSION_SELECT_ALL& sp)
{
	return STATIC_MISSION::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_mission_sid,
		sp.mOut.c_name,
		sp.mOut.c_condition_script,
		sp.mOut.c_ratio,
		sp.mOut.c_action_script
	);
}

STATIC_PRODUCT::Writer DbUtil::SpToPacket(const SP_STATIC_PRODUCT_SELECT& sp)
{
	return STATIC_PRODUCT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_product_sid,
		sp.mOut.c_name,
		sp.mOut.c_product_id_google,
		sp.mOut.c_product_id_apple,
		sp.mOut.c_price_google,
		sp.mOut.c_price_apple,
		sp.mOut.c_condition_script,
		sp.mOut.c_action_script
	);
}

STATIC_PRODUCT::Writer DbUtil::SpToPacket(const SP_STATIC_PRODUCT_SELECT_ALL& sp)
{
	return STATIC_PRODUCT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_product_sid,
		sp.mOut.c_name,
		sp.mOut.c_product_id_google,
		sp.mOut.c_product_id_apple,
		sp.mOut.c_price_google,
		sp.mOut.c_price_apple,
		sp.mOut.c_condition_script,
		sp.mOut.c_action_script
	);
}

STATIC_QUEST::Writer DbUtil::SpToPacket(const SP_STATIC_QUEST_SELECT& sp)
{
	return STATIC_QUEST::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_quest_sid,
		sp.mOut.c_name,
		sp.mOut.c_quest_chapter,
		sp.mOut.c_quest_index,
		sp.mOut.c_condition_script,
		sp.mOut.c_action_script,
		sp.mOut.c_complete_condition_script,
		sp.mOut.c_complete_action_script
	);
}

STATIC_QUEST::Writer DbUtil::SpToPacket(const SP_STATIC_QUEST_SELECT_ALL& sp)
{
	return STATIC_QUEST::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_quest_sid,
		sp.mOut.c_name,
		sp.mOut.c_quest_chapter,
		sp.mOut.c_quest_index,
		sp.mOut.c_condition_script,
		sp.mOut.c_action_script,
		sp.mOut.c_complete_condition_script,
		sp.mOut.c_complete_action_script
	);
}

STATIC_REWARD_GROUP::Writer DbUtil::SpToPacket(const SP_STATIC_REWARD_GROUP_SELECT& sp)
{
	return STATIC_REWARD_GROUP::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_reward_sid,
		sp.mOut.c_reward_value,
		sp.mOut.c_user_exp,
		sp.mOut.c_item_sid,
		sp.mOut.c_item_quantity
	);
}

STATIC_REWARD_GROUP::Writer DbUtil::SpToPacket(const SP_STATIC_REWARD_GROUP_SELECT_ALL& sp)
{
	return STATIC_REWARD_GROUP::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_reward_sid,
		sp.mOut.c_reward_value,
		sp.mOut.c_user_exp,
		sp.mOut.c_item_sid,
		sp.mOut.c_item_quantity
	);
}

STATIC_REWARD_RATIO::Writer DbUtil::SpToPacket(const SP_STATIC_REWARD_RATIO_SELECT& sp)
{
	return STATIC_REWARD_RATIO::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_reward_sid,
		sp.mOut.c_reward_value,
		sp.mOut.c_user_exp,
		sp.mOut.c_item_sid,
		sp.mOut.c_item_quantity
	);
}

STATIC_REWARD_RATIO::Writer DbUtil::SpToPacket(const SP_STATIC_REWARD_RATIO_SELECT_ALL& sp)
{
	return STATIC_REWARD_RATIO::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_reward_sid,
		sp.mOut.c_reward_value,
		sp.mOut.c_user_exp,
		sp.mOut.c_item_sid,
		sp.mOut.c_item_quantity
	);
}

STATIC_REWARD::Writer DbUtil::SpToPacket(const SP_STATIC_REWARD_SELECT& sp)
{
	return STATIC_REWARD::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_reward_sid,
		GetRewardTypeEnum(sp.mOut.c_reward_type),
		sp.mOut.c_user_exp,
		sp.mOut.c_item_sid,
		sp.mOut.c_item_quantity
	);
}

STATIC_REWARD::Writer DbUtil::SpToPacket(const SP_STATIC_REWARD_SELECT_ALL& sp)
{
	return STATIC_REWARD::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_reward_sid,
		GetRewardTypeEnum(sp.mOut.c_reward_type),
		sp.mOut.c_user_exp,
		sp.mOut.c_item_sid,
		sp.mOut.c_item_quantity
	);
}

STATIC_SCHEDULE::Writer DbUtil::SpToPacket(const SP_STATIC_SCHEDULE_SELECT& sp)
{
	return STATIC_SCHEDULE::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_schedule_sid,
		sp.mOut.c_name,
		sp.mOut.c_condition_script,
		sp.mOut.c_action_script
	);
}

STATIC_SCHEDULE::Writer DbUtil::SpToPacket(const SP_STATIC_SCHEDULE_SELECT_ALL& sp)
{
	return STATIC_SCHEDULE::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_schedule_sid,
		sp.mOut.c_name,
		sp.mOut.c_condition_script,
		sp.mOut.c_action_script
	);
}

STATIC_STAT::Writer DbUtil::SpToPacket(const SP_STATIC_STAT_SELECT& sp)
{
	return STATIC_STAT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		GetStatTypeEnum(sp.mOut.c_stat_type),
		sp.mOut.c_name
	);
}

STATIC_STAT::Writer DbUtil::SpToPacket(const SP_STATIC_STAT_SELECT_ALL& sp)
{
	return STATIC_STAT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		GetStatTypeEnum(sp.mOut.c_stat_type),
		sp.mOut.c_name
	);
}

STATIC_STOCK::Writer DbUtil::SpToPacket(const SP_STATIC_STOCK_SELECT& sp)
{
	return STATIC_STOCK::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_stock_sid,
		sp.mOut.c_name,
		sp.mOut.c_stock_slot,
		sp.mOut.c_condition_script,
		sp.mOut.c_ratio,
		sp.mOut.c_action_script
	);
}

STATIC_STOCK::Writer DbUtil::SpToPacket(const SP_STATIC_STOCK_SELECT_ALL& sp)
{
	return STATIC_STOCK::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_stock_sid,
		sp.mOut.c_name,
		sp.mOut.c_stock_slot,
		sp.mOut.c_condition_script,
		sp.mOut.c_ratio,
		sp.mOut.c_action_script
	);
}

STATIC_TRANSLATION::Writer DbUtil::SpToPacket(const SP_STATIC_TRANSLATION_SELECT& sp)
{
	return STATIC_TRANSLATION::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_display_key,
		GetLanguageTypeEnum(sp.mOut.c_language_type),
		sp.mOut.c_language_text
	);
}

STATIC_TRANSLATION::Writer DbUtil::SpToPacket(const SP_STATIC_TRANSLATION_SELECT_ALL& sp)
{
	return STATIC_TRANSLATION::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_display_key,
		GetLanguageTypeEnum(sp.mOut.c_language_type),
		sp.mOut.c_language_text
	);
}

STATIC_USER_EXP::Writer DbUtil::SpToPacket(const SP_STATIC_USER_EXP_SELECT& sp)
{
	return STATIC_USER_EXP::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_user_exp,
		sp.mOut.c_user_level,
		sp.mOut.c_mail_sid
	);
}

STATIC_USER_EXP::Writer DbUtil::SpToPacket(const SP_STATIC_USER_EXP_SELECT_ALL& sp)
{
	return STATIC_USER_EXP::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_user_exp,
		sp.mOut.c_user_level,
		sp.mOut.c_mail_sid
	);
}

LOG_ACHIEVEMENT::Writer DbUtil::SpToPacket(const SP_LOG_ACHIEVEMENT_SELECT& sp)
{
	return LOG_ACHIEVEMENT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_log_id,
		sp.mOut.c_user_id,
		sp.mOut.c_achievement_sid,
		sp.mOut.c_achievement_step_sid,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_log)
	);
}

LOG_ACHIEVEMENT::Writer DbUtil::SpToPacket(const SP_LOG_ACHIEVEMENT_SELECT_ALL& sp)
{
	return LOG_ACHIEVEMENT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_log_id,
		sp.mOut.c_user_id,
		sp.mOut.c_achievement_sid,
		sp.mOut.c_achievement_step_sid,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_log)
	);
}

ACCOUNT_DELETED::Writer DbUtil::SpToPacket(const SP_ACCOUNT_DELETED_SELECT& sp)
{
	return ACCOUNT_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_account_id,
		sp.mOut.c_device_token,
		sp.mOut.c_google_token,
		sp.mOut.c_apple_token,
		sp.mOut.c_account_state,
		sp.mOut.c_db_shard_idx,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

ACCOUNT_DELETED::Writer DbUtil::SpToPacket(const SP_ACCOUNT_DELETED_SELECT_ALL& sp)
{
	return ACCOUNT_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_account_id,
		sp.mOut.c_device_token,
		sp.mOut.c_google_token,
		sp.mOut.c_apple_token,
		sp.mOut.c_account_state,
		sp.mOut.c_db_shard_idx,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

ACCOUNT::Writer DbUtil::SpToPacket(const SP_ACCOUNT_SELECT& sp)
{
	return ACCOUNT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_account_id,
		sp.mOut.c_device_token,
		sp.mOut.c_google_token,
		sp.mOut.c_apple_token,
		sp.mOut.c_db_shard_idx,
		sp.mOut.c_account_state,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_ban),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ACCOUNT::Writer DbUtil::SpToPacket(const SP_ACCOUNT_SELECT_ALL& sp)
{
	return ACCOUNT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_account_id,
		sp.mOut.c_device_token,
		sp.mOut.c_google_token,
		sp.mOut.c_apple_token,
		sp.mOut.c_db_shard_idx,
		sp.mOut.c_account_state,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_ban),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ACCOUNT_USER_DELETED::Writer DbUtil::SpToPacket(const SP_ACCOUNT_USER_DELETED_SELECT& sp)
{
	return ACCOUNT_USER_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_account_id,
		sp.mOut.c_user_id,
		GetUserTypeEnum(sp.mOut.c_user_type),
		sp.mOut.c_user_state,
		sp.mOut.c_user_name,
		sp.mOut.c_user_level,
		sp.mOut.c_user_level_highest,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_ban),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

ACCOUNT_USER_DELETED::Writer DbUtil::SpToPacket(const SP_ACCOUNT_USER_DELETED_SELECT_ALL& sp)
{
	return ACCOUNT_USER_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_account_id,
		sp.mOut.c_user_id,
		GetUserTypeEnum(sp.mOut.c_user_type),
		sp.mOut.c_user_state,
		sp.mOut.c_user_name,
		sp.mOut.c_user_level,
		sp.mOut.c_user_level_highest,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_ban),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

ACCOUNT_USER_DELETED_BY_ACCOUNT::Writer DbUtil::SpToPacket(const SP_ACCOUNT_USER_DELETED_SELECT_BY_ACCOUNT& sp)
{
	return ACCOUNT_USER_DELETED_BY_ACCOUNT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_account_id,
		sp.mOut.c_user_id,
		GetUserTypeEnum(sp.mOut.c_user_type),
		sp.mOut.c_user_state,
		sp.mOut.c_user_name,
		sp.mOut.c_user_level,
		sp.mOut.c_user_level_highest,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_ban),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

ACCOUNT_USER_GAME::Writer DbUtil::SpToPacket(const SP_ACCOUNT_USER_GAME_SELECT& sp)
{
	return ACCOUNT_USER_GAME::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_game_id,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated)
	);
}

ACCOUNT_USER_GAME::Writer DbUtil::SpToPacket(const SP_ACCOUNT_USER_GAME_SELECT_ALL& sp)
{
	return ACCOUNT_USER_GAME::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_game_id,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated)
	);
}

ACCOUNT_USER::Writer DbUtil::SpToPacket(const SP_ACCOUNT_USER_SELECT& sp)
{
	return ACCOUNT_USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_account_id,
		sp.mOut.c_server_id,
		sp.mOut.c_user_id,
		GetUserTypeEnum(sp.mOut.c_user_type),
		sp.mOut.c_user_state,
		sp.mOut.c_user_name,
		sp.mOut.c_user_level,
		sp.mOut.c_user_level_highest,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_ban),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ACCOUNT_USER::Writer DbUtil::SpToPacket(const SP_ACCOUNT_USER_SELECT_ALL& sp)
{
	return ACCOUNT_USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_account_id,
		sp.mOut.c_server_id,
		sp.mOut.c_user_id,
		GetUserTypeEnum(sp.mOut.c_user_type),
		sp.mOut.c_user_state,
		sp.mOut.c_user_name,
		sp.mOut.c_user_level,
		sp.mOut.c_user_level_highest,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_ban),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ACCOUNT_USER_BY_ACCOUNT::Writer DbUtil::SpToPacket(const SP_ACCOUNT_USER_SELECT_BY_ACCOUNT& sp)
{
	return ACCOUNT_USER_BY_ACCOUNT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_account_id,
		sp.mOut.c_server_id,
		sp.mOut.c_user_id,
		GetUserTypeEnum(sp.mOut.c_user_type),
		sp.mOut.c_user_state,
		sp.mOut.c_user_name,
		sp.mOut.c_user_level,
		sp.mOut.c_user_level_highest,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_ban),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

GAME_DELETED::Writer DbUtil::SpToPacket(const SP_GAME_DELETED_SELECT& sp)
{
	return GAME_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_game_id,
		sp.mOut.c_server_id,
		sp.mOut.c_user_id,
		sp.mOut.c_game_sid,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

GAME_DELETED::Writer DbUtil::SpToPacket(const SP_GAME_DELETED_SELECT_ALL& sp)
{
	return GAME_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_game_id,
		sp.mOut.c_server_id,
		sp.mOut.c_user_id,
		sp.mOut.c_game_sid,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

GAME::Writer DbUtil::SpToPacket(const SP_GAME_SELECT& sp)
{
	return GAME::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_game_id,
		sp.mOut.c_server_id,
		sp.mOut.c_user_id,
		sp.mOut.c_game_sid,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

GAME::Writer DbUtil::SpToPacket(const SP_GAME_SELECT_ALL& sp)
{
	return GAME::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_game_id,
		sp.mOut.c_server_id,
		sp.mOut.c_user_id,
		sp.mOut.c_game_sid,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

PURCHASE_DELETED::Writer DbUtil::SpToPacket(const SP_PURCHASE_DELETED_SELECT& sp)
{
	return PURCHASE_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_purchase_id,
		sp.mOut.c_account_id,
		sp.mOut.c_user_id,
		sp.mOut.c_guild_id,
		sp.mOut.c_product_sid,
		sp.mOut.c_product_receipt,
		sp.mOut.c_purchase_state,
		sp.mOut.c_mail_id,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

PURCHASE_DELETED::Writer DbUtil::SpToPacket(const SP_PURCHASE_DELETED_SELECT_ALL& sp)
{
	return PURCHASE_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_purchase_id,
		sp.mOut.c_account_id,
		sp.mOut.c_user_id,
		sp.mOut.c_guild_id,
		sp.mOut.c_product_sid,
		sp.mOut.c_product_receipt,
		sp.mOut.c_purchase_state,
		sp.mOut.c_mail_id,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

PURCHASE::Writer DbUtil::SpToPacket(const SP_PURCHASE_SELECT& sp)
{
	return PURCHASE::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_purchase_id,
		sp.mOut.c_account_id,
		sp.mOut.c_user_id,
		sp.mOut.c_guild_id,
		sp.mOut.c_product_sid,
		sp.mOut.c_product_receipt,
		sp.mOut.c_purchase_state,
		sp.mOut.c_mail_id,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

PURCHASE::Writer DbUtil::SpToPacket(const SP_PURCHASE_SELECT_ALL& sp)
{
	return PURCHASE::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_purchase_id,
		sp.mOut.c_account_id,
		sp.mOut.c_user_id,
		sp.mOut.c_guild_id,
		sp.mOut.c_product_sid,
		sp.mOut.c_product_receipt,
		sp.mOut.c_purchase_state,
		sp.mOut.c_mail_id,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

SERVER::Writer DbUtil::SpToPacket(const SP_SERVER_SELECT& sp)
{
	return SERVER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_server_id,
		sp.mOut.c_server_name,
		sp.mOut.c_home_game_sid,
		sp.mOut.c_server_state,
		sp.mOut.c_wau_count_max,
		sp.mOut.c_wau_count_busy,
		sp.mOut.c_ccu_count_max,
		sp.mOut.c_ccu_count_busy
	);
}

SERVER::Writer DbUtil::SpToPacket(const SP_SERVER_SELECT_ALL& sp)
{
	return SERVER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_server_id,
		sp.mOut.c_server_name,
		sp.mOut.c_home_game_sid,
		sp.mOut.c_server_state,
		sp.mOut.c_wau_count_max,
		sp.mOut.c_wau_count_busy,
		sp.mOut.c_ccu_count_max,
		sp.mOut.c_ccu_count_busy
	);
}

SERVER_STATISTICS::Writer DbUtil::SpToPacket(const SP_SERVER_STATISTICS_SELECT& sp)
{
	return SERVER_STATISTICS::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_server_id,
		sp.mOut.c_tuc_count,
		sp.mOut.c_mau_count,
		sp.mOut.c_wau_count,
		sp.mOut.c_ccu_count
	);
}

SERVER_STATISTICS::Writer DbUtil::SpToPacket(const SP_SERVER_STATISTICS_SELECT_ALL& sp)
{
	return SERVER_STATISTICS::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_server_id,
		sp.mOut.c_tuc_count,
		sp.mOut.c_mau_count,
		sp.mOut.c_wau_count,
		sp.mOut.c_ccu_count
	);
}

ACHIEVEMENT::Writer DbUtil::SpToPacket(const SP_ACHIEVEMENT_SELECT& sp)
{
	return ACHIEVEMENT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_achievement_id,
		sp.mOut.c_achievement_sid,
		sp.mOut.c_achievement_step,
		sp.mOut.c_reward_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ACHIEVEMENT::Writer DbUtil::SpToPacket(const SP_ACHIEVEMENT_SELECT_ALL& sp)
{
	return ACHIEVEMENT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_achievement_id,
		sp.mOut.c_achievement_sid,
		sp.mOut.c_achievement_step,
		sp.mOut.c_reward_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ACHIEVEMENT_BY_USER::Writer DbUtil::SpToPacket(const SP_ACHIEVEMENT_SELECT_BY_USER& sp)
{
	return ACHIEVEMENT_BY_USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_achievement_id,
		sp.mOut.c_achievement_sid,
		sp.mOut.c_achievement_step,
		sp.mOut.c_reward_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ACTIVITY::Writer DbUtil::SpToPacket(const SP_ACTIVITY_SELECT& sp)
{
	return ACTIVITY::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_activity_id,
		sp.mOut.c_activity_sid,
		sp.mOut.c_activity_point,
		sp.mOut.c_activity_point_increased,
		sp.mOut.c_activity_point_decreased,
		sp.mOut.c_activity_point_min,
		sp.mOut.c_activity_point_max,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ACTIVITY::Writer DbUtil::SpToPacket(const SP_ACTIVITY_SELECT_ALL& sp)
{
	return ACTIVITY::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_activity_id,
		sp.mOut.c_activity_sid,
		sp.mOut.c_activity_point,
		sp.mOut.c_activity_point_increased,
		sp.mOut.c_activity_point_decreased,
		sp.mOut.c_activity_point_min,
		sp.mOut.c_activity_point_max,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ACTIVITY_BY_USER::Writer DbUtil::SpToPacket(const SP_ACTIVITY_SELECT_BY_USER& sp)
{
	return ACTIVITY_BY_USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_activity_id,
		sp.mOut.c_activity_sid,
		sp.mOut.c_activity_point,
		sp.mOut.c_activity_point_increased,
		sp.mOut.c_activity_point_decreased,
		sp.mOut.c_activity_point_min,
		sp.mOut.c_activity_point_max,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ITEM_DELETED::Writer DbUtil::SpToPacket(const SP_ITEM_DELETED_SELECT& sp)
{
	return ITEM_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_user_id,
		sp.mOut.c_item_id,
		sp.mOut.c_parent_item_id,
		sp.mOut.c_parent_mail_id,
		sp.mOut.c_item_sid,
		sp.mOut.c_inventory_slot_index,
		sp.mOut.c_quick_slot_index,
		sp.mOut.c_equip_slot_index,
		sp.mOut.c_item_quantity,
		sp.mOut.c_item_level,
		sp.mOut.c_item_color_union,
		sp.mOut.c_item_stat_union,
		sp.mOut.c_item_enchant_union,
		sp.mOut.c_item_state_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_cooltime),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_expire),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

ITEM_DELETED::Writer DbUtil::SpToPacket(const SP_ITEM_DELETED_SELECT_ALL& sp)
{
	return ITEM_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_user_id,
		sp.mOut.c_item_id,
		sp.mOut.c_parent_item_id,
		sp.mOut.c_parent_mail_id,
		sp.mOut.c_item_sid,
		sp.mOut.c_inventory_slot_index,
		sp.mOut.c_quick_slot_index,
		sp.mOut.c_equip_slot_index,
		sp.mOut.c_item_quantity,
		sp.mOut.c_item_level,
		sp.mOut.c_item_color_union,
		sp.mOut.c_item_stat_union,
		sp.mOut.c_item_enchant_union,
		sp.mOut.c_item_state_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_cooltime),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_expire),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

ITEM::Writer DbUtil::SpToPacket(const SP_ITEM_SELECT& sp)
{
	return ITEM::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_item_id,
		sp.mOut.c_parent_item_id,
		sp.mOut.c_parent_mail_id,
		sp.mOut.c_item_sid,
		sp.mOut.c_inventory_slot_index,
		sp.mOut.c_quick_slot_index,
		sp.mOut.c_equip_slot_index,
		sp.mOut.c_item_quantity,
		sp.mOut.c_item_level,
		sp.mOut.c_item_color_union,
		sp.mOut.c_item_stat_union,
		sp.mOut.c_item_enchant_union,
		sp.mOut.c_item_state_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_cooltime),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_expire),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ITEM::Writer DbUtil::SpToPacket(const SP_ITEM_SELECT_ALL& sp)
{
	return ITEM::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_item_id,
		sp.mOut.c_parent_item_id,
		sp.mOut.c_parent_mail_id,
		sp.mOut.c_item_sid,
		sp.mOut.c_inventory_slot_index,
		sp.mOut.c_quick_slot_index,
		sp.mOut.c_equip_slot_index,
		sp.mOut.c_item_quantity,
		sp.mOut.c_item_level,
		sp.mOut.c_item_color_union,
		sp.mOut.c_item_stat_union,
		sp.mOut.c_item_enchant_union,
		sp.mOut.c_item_state_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_cooltime),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_expire),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ITEM_BY_USER::Writer DbUtil::SpToPacket(const SP_ITEM_SELECT_BY_USER& sp)
{
	return ITEM_BY_USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_item_id,
		sp.mOut.c_parent_item_id,
		sp.mOut.c_parent_mail_id,
		sp.mOut.c_item_sid,
		sp.mOut.c_inventory_slot_index,
		sp.mOut.c_quick_slot_index,
		sp.mOut.c_equip_slot_index,
		sp.mOut.c_item_quantity,
		sp.mOut.c_item_level,
		sp.mOut.c_item_color_union,
		sp.mOut.c_item_stat_union,
		sp.mOut.c_item_enchant_union,
		sp.mOut.c_item_state_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_cooltime),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_expire),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

ITEM_STAT::Writer DbUtil::SpToPacket(const SP_ITEM_STAT_SELECT& sp)
{
	return ITEM_STAT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_item_id,
		GetStatTypeEnum(sp.mOut.c_stat_type),
		sp.mOut.c_stat_value,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated)
	);
}

ITEM_STAT::Writer DbUtil::SpToPacket(const SP_ITEM_STAT_SELECT_ALL& sp)
{
	return ITEM_STAT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_item_id,
		GetStatTypeEnum(sp.mOut.c_stat_type),
		sp.mOut.c_stat_value,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated)
	);
}

ITEM_STAT_BY_USER::Writer DbUtil::SpToPacket(const SP_ITEM_STAT_SELECT_BY_USER& sp)
{
	return ITEM_STAT_BY_USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_item_id,
		GetStatTypeEnum(sp.mOut.c_stat_type),
		sp.mOut.c_stat_value,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated)
	);
}

MAIL_DELETED::Writer DbUtil::SpToPacket(const SP_MAIL_DELETED_SELECT& sp)
{
	return MAIL_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_user_id,
		sp.mOut.c_mail_id,
		sp.mOut.c_mail_sid,
		sp.mOut.c_read_flag,
		sp.mOut.c_reward_flag,
		sp.mOut.c_title,
		sp.mOut.c_msg,
		sp.mOut.c_msg_args,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_expire),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

MAIL_DELETED::Writer DbUtil::SpToPacket(const SP_MAIL_DELETED_SELECT_ALL& sp)
{
	return MAIL_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_user_id,
		sp.mOut.c_mail_id,
		sp.mOut.c_mail_sid,
		sp.mOut.c_read_flag,
		sp.mOut.c_reward_flag,
		sp.mOut.c_title,
		sp.mOut.c_msg,
		sp.mOut.c_msg_args,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_expire),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

MAIL::Writer DbUtil::SpToPacket(const SP_MAIL_SELECT& sp)
{
	return MAIL::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_mail_id,
		sp.mOut.c_mail_sid,
		sp.mOut.c_read_flag,
		sp.mOut.c_reward_flag,
		sp.mOut.c_title,
		sp.mOut.c_msg,
		sp.mOut.c_msg_args,
		sp.mOut.c_reward_sid,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_expire),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

MAIL::Writer DbUtil::SpToPacket(const SP_MAIL_SELECT_ALL& sp)
{
	return MAIL::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_mail_id,
		sp.mOut.c_mail_sid,
		sp.mOut.c_read_flag,
		sp.mOut.c_reward_flag,
		sp.mOut.c_title,
		sp.mOut.c_msg,
		sp.mOut.c_msg_args,
		sp.mOut.c_reward_sid,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_expire),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

MAIL_BY_USER::Writer DbUtil::SpToPacket(const SP_MAIL_SELECT_BY_USER& sp)
{
	return MAIL_BY_USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_mail_id,
		sp.mOut.c_mail_sid,
		sp.mOut.c_read_flag,
		sp.mOut.c_reward_flag,
		sp.mOut.c_title,
		sp.mOut.c_msg,
		sp.mOut.c_msg_args,
		sp.mOut.c_reward_sid,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_expire),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

MISSION_DELETED::Writer DbUtil::SpToPacket(const SP_MISSION_DELETED_SELECT& sp)
{
	return MISSION_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_user_id,
		sp.mOut.c_mission_id,
		sp.mOut.c_mission_sid,
		sp.mOut.c_activity_point_delta,
		sp.mOut.c_reward_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

MISSION_DELETED::Writer DbUtil::SpToPacket(const SP_MISSION_DELETED_SELECT_ALL& sp)
{
	return MISSION_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_user_id,
		sp.mOut.c_mission_id,
		sp.mOut.c_mission_sid,
		sp.mOut.c_activity_point_delta,
		sp.mOut.c_reward_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

MISSION::Writer DbUtil::SpToPacket(const SP_MISSION_SELECT& sp)
{
	return MISSION::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_mission_id,
		sp.mOut.c_mission_sid,
		sp.mOut.c_activity_point_delta,
		sp.mOut.c_reward_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

MISSION::Writer DbUtil::SpToPacket(const SP_MISSION_SELECT_ALL& sp)
{
	return MISSION::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_mission_id,
		sp.mOut.c_mission_sid,
		sp.mOut.c_activity_point_delta,
		sp.mOut.c_reward_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

MISSION_BY_USER::Writer DbUtil::SpToPacket(const SP_MISSION_SELECT_BY_USER& sp)
{
	return MISSION_BY_USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_mission_id,
		sp.mOut.c_mission_sid,
		sp.mOut.c_activity_point_delta,
		sp.mOut.c_reward_flag,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

QUEST_DELETED::Writer DbUtil::SpToPacket(const SP_QUEST_DELETED_SELECT& sp)
{
	return QUEST_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_user_id,
		sp.mOut.c_quest_id,
		sp.mOut.c_quest_sid,
		sp.mOut.c_activity_point_delta,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

QUEST_DELETED::Writer DbUtil::SpToPacket(const SP_QUEST_DELETED_SELECT_ALL& sp)
{
	return QUEST_DELETED::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_id,
		sp.mOut.c_user_id,
		sp.mOut.c_quest_id,
		sp.mOut.c_quest_sid,
		sp.mOut.c_activity_point_delta,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_deleted)
	);
}

QUEST::Writer DbUtil::SpToPacket(const SP_QUEST_SELECT& sp)
{
	return QUEST::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_quest_id,
		sp.mOut.c_quest_sid,
		sp.mOut.c_activity_point_delta,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

QUEST::Writer DbUtil::SpToPacket(const SP_QUEST_SELECT_ALL& sp)
{
	return QUEST::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_quest_id,
		sp.mOut.c_quest_sid,
		sp.mOut.c_activity_point_delta,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

QUEST_BY_USER::Writer DbUtil::SpToPacket(const SP_QUEST_SELECT_BY_USER& sp)
{
	return QUEST_BY_USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		sp.mOut.c_quest_id,
		sp.mOut.c_quest_sid,
		sp.mOut.c_activity_point_delta,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

USER_POS::Writer DbUtil::SpToPacket(const SP_USER_POS_SELECT& sp)
{
	return USER_POS::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		GetGameTypeEnum(sp.mOut.c_game_type),
		sp.mOut.c_game_id,
		sp.mOut.c_x,
		sp.mOut.c_y,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated)
	);
}

USER_POS::Writer DbUtil::SpToPacket(const SP_USER_POS_SELECT_ALL& sp)
{
	return USER_POS::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_user_id,
		GetGameTypeEnum(sp.mOut.c_game_type),
		sp.mOut.c_game_id,
		sp.mOut.c_x,
		sp.mOut.c_y,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated)
	);
}

USER::Writer DbUtil::SpToPacket(const SP_USER_SELECT& sp)
{
	return USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_account_id,
		sp.mOut.c_user_id,
		sp.mOut.c_user_exp,
		sp.mOut.c_user_exp_total,
		sp.mOut.c_play_second_total,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_login),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_logout),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

USER::Writer DbUtil::SpToPacket(const SP_USER_SELECT_ALL& sp)
{
	return USER::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_account_id,
		sp.mOut.c_user_id,
		sp.mOut.c_user_exp,
		sp.mOut.c_user_exp_total,
		sp.mOut.c_play_second_total,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_login),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_logout),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}

USER_BY_ACCOUNT::Writer DbUtil::SpToPacket(const SP_USER_SELECT_BY_ACCOUNT& sp)
{
	return USER_BY_ACCOUNT::Writer(PARAM, TEMP_BUF,
		sp.mOut.c_account_id,
		sp.mOut.c_user_id,
		sp.mOut.c_user_exp,
		sp.mOut.c_user_exp_total,
		sp.mOut.c_play_second_total,
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_login),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_logout),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_updated),
		TimeUtil:: TT_FROM_TS(sp.mOut.c_date_created)
	);
}
*/
