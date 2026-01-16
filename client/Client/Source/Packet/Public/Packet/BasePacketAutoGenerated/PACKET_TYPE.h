// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/PacketFundamentals/PacketFundamentals.h"

namespace PacketTypes
{
inline constexpr PacketType START = 0;

inline constexpr PacketType COMMON_PACKET_START = 1000;
inline constexpr PacketType LOG_ACHIEVEMENT = 1001;
inline constexpr PacketType INSERT_LOG_ACHIEVEMENT = 1002;
inline constexpr PacketType LOG_ACHIEVEMENT_ALL = 1003;
inline constexpr PacketType STATIC_ACHIEVEMENT = 1004;
inline constexpr PacketType STATIC_ACHIEVEMENT_ALL = 1005;
inline constexpr PacketType STATIC_ACHIEVEMENT_STEP = 1006;
inline constexpr PacketType STATIC_ACHIEVEMENT_STEP_ALL = 1007;
inline constexpr PacketType STATIC_BUILDING = 1008;
inline constexpr PacketType STATIC_BUILDING_ALL = 1009;
inline constexpr PacketType STATIC_CHARACTER = 1010;
inline constexpr PacketType STATIC_CHARACTER_ALL = 1011;
inline constexpr PacketType STATIC_CHARACTER_SKILL = 1012;
inline constexpr PacketType STATIC_CHARACTER_SKILL_ALL = 1013;
inline constexpr PacketType STATIC_GAME = 1014;
inline constexpr PacketType STATIC_GAME_ALL = 1015;
inline constexpr PacketType STATIC_ITEM = 1016;
inline constexpr PacketType STATIC_ITEM_ALL = 1017;
inline constexpr PacketType STATIC_ITEM_STAT = 1018;
inline constexpr PacketType STATIC_ITEM_STAT_ALL = 1019;
inline constexpr PacketType STATIC_MAIL = 1020;
inline constexpr PacketType STATIC_MAIL_ALL = 1021;
inline constexpr PacketType STATIC_MAP = 1022;
inline constexpr PacketType STATIC_MAP_ALL = 1023;
inline constexpr PacketType STATIC_MISSION = 1024;
inline constexpr PacketType STATIC_MISSION_ALL = 1025;
inline constexpr PacketType STATIC_PRODUCT = 1026;
inline constexpr PacketType STATIC_PRODUCT_ALL = 1027;
inline constexpr PacketType STATIC_QUEST = 1028;
inline constexpr PacketType STATIC_QUEST_ALL = 1029;
inline constexpr PacketType STATIC_REWARD_GROUP = 1030;
inline constexpr PacketType STATIC_REWARD_GROUP_ALL = 1031;
inline constexpr PacketType STATIC_REWARD_RATIO = 1032;
inline constexpr PacketType STATIC_REWARD_RATIO_ALL = 1033;
inline constexpr PacketType STATIC_REWARD = 1034;
inline constexpr PacketType STATIC_REWARD_ALL = 1035;
inline constexpr PacketType STATIC_SCHEDULE = 1036;
inline constexpr PacketType STATIC_SCHEDULE_ALL = 1037;
inline constexpr PacketType STATIC_STAT = 1038;
inline constexpr PacketType STATIC_STAT_ALL = 1039;
inline constexpr PacketType STATIC_STOCK = 1040;
inline constexpr PacketType STATIC_STOCK_ALL = 1041;
inline constexpr PacketType STATIC_TRANSLATION = 1042;
inline constexpr PacketType STATIC_TRANSLATION_ALL = 1043;
inline constexpr PacketType STATIC_USER_EXP = 1044;
inline constexpr PacketType STATIC_USER_EXP_ALL = 1045;
inline constexpr PacketType ACCOUNT_DELETED = 1046;
inline constexpr PacketType ACCOUNT_DELETED_ALL = 1047;
inline constexpr PacketType ACCOUNT = 1048;
inline constexpr PacketType ACCOUNT_ALL = 1049;
inline constexpr PacketType ACCOUNT_USER_DELETED = 1050;
inline constexpr PacketType ACCOUNT_USER_DELETED_ALL = 1051;
inline constexpr PacketType ACCOUNT_USER_DELETED_BY_ACCOUNT = 1052;
inline constexpr PacketType ACCOUNT_USER_GAME = 1053;
inline constexpr PacketType ACCOUNT_USER_GAME_ALL = 1054;
inline constexpr PacketType ACCOUNT_USER = 1055;
inline constexpr PacketType ACCOUNT_USER_ALL = 1056;
inline constexpr PacketType ACCOUNT_USER_BY_ACCOUNT = 1057;
inline constexpr PacketType GAME_DELETED = 1058;
inline constexpr PacketType GAME_DELETED_ALL = 1059;
inline constexpr PacketType GAME = 1060;
inline constexpr PacketType GAME_ALL = 1061;
inline constexpr PacketType PURCHASE_DELETED = 1062;
inline constexpr PacketType PURCHASE_DELETED_ALL = 1063;
inline constexpr PacketType PURCHASE = 1064;
inline constexpr PacketType PURCHASE_ALL = 1065;
inline constexpr PacketType SERVER = 1066;
inline constexpr PacketType SERVER_ALL = 1067;
inline constexpr PacketType SERVER_STATISTICS = 1068;
inline constexpr PacketType SERVER_STATISTICS_ALL = 1069;
inline constexpr PacketType ACHIEVEMENT = 1070;
inline constexpr PacketType ACHIEVEMENT_ALL = 1071;
inline constexpr PacketType ACHIEVEMENT_BY_USER = 1072;
inline constexpr PacketType ACTIVITY = 1073;
inline constexpr PacketType ACTIVITY_ALL = 1074;
inline constexpr PacketType ACTIVITY_BY_USER = 1075;
inline constexpr PacketType ITEM_DELETED = 1076;
inline constexpr PacketType ITEM_DELETED_ALL = 1077;
inline constexpr PacketType ITEM = 1078;
inline constexpr PacketType ITEM_ALL = 1079;
inline constexpr PacketType ITEM_BY_USER = 1080;
inline constexpr PacketType ITEM_STAT = 1081;
inline constexpr PacketType ITEM_STAT_ALL = 1082;
inline constexpr PacketType ITEM_STAT_BY_USER = 1083;
inline constexpr PacketType MAIL_DELETED = 1084;
inline constexpr PacketType MAIL_DELETED_ALL = 1085;
inline constexpr PacketType MAIL = 1086;
inline constexpr PacketType MAIL_ALL = 1087;
inline constexpr PacketType MAIL_BY_USER = 1088;
inline constexpr PacketType MISSION_DELETED = 1089;
inline constexpr PacketType MISSION_DELETED_ALL = 1090;
inline constexpr PacketType MISSION = 1091;
inline constexpr PacketType MISSION_ALL = 1092;
inline constexpr PacketType MISSION_BY_USER = 1093;
inline constexpr PacketType QUEST_DELETED = 1094;
inline constexpr PacketType QUEST_DELETED_ALL = 1095;
inline constexpr PacketType QUEST = 1096;
inline constexpr PacketType QUEST_ALL = 1097;
inline constexpr PacketType QUEST_BY_USER = 1098;
inline constexpr PacketType USER_POS = 1099;
inline constexpr PacketType USER_POS_ALL = 1100;
inline constexpr PacketType USER = 1101;
inline constexpr PacketType USER_ALL = 1102;
inline constexpr PacketType USER_BY_ACCOUNT = 1103;
inline constexpr PacketType HANDSHAKE_TO_PEER = 1104;
inline constexpr PacketType HANDSHAKE_TO_HOST = 1105;
inline constexpr PacketType APP_DATA = 1106;
inline constexpr PacketType USER_DATA = 1107;
inline constexpr PacketType PACKET_DEV_DATA = 1108;
inline constexpr PacketType SYNC_TIME = 1109;
inline constexpr PacketType REQ_GLOBAL_NOW = 1110;
inline constexpr PacketType ACK_GLOBAL_NOW = 1111;
inline constexpr PacketType REQ_APP_JOIN = 1112;
inline constexpr PacketType ACK_APP_JOIN = 1113;
inline constexpr PacketType APP_ADD = 1114;
inline constexpr PacketType APP_REMOVE = 1115;
inline constexpr PacketType REQ_SHELL_COMMAND = 1116;
inline constexpr PacketType ACK_SHELL_COMMAND = 1117;
inline constexpr PacketType SHELL_NOTIFY = 1118;
inline constexpr PacketType STRING_ELEM = 1119;
inline constexpr PacketType BIN_ELEM = 1120;
inline constexpr PacketType USER_CACHE_DIFF = 1121;
inline constexpr PacketType WORLD_USER_COMMAND = 1122;
inline constexpr PacketType WORLD_USER_STAT_ALL = 1123;
inline constexpr PacketType WORLD_USER_STAT_DIFF = 1124;
inline constexpr PacketType COMMON_PACKET_END = 1125;

inline constexpr PacketType DL_PACKET_START = 2000;
inline constexpr PacketType DL = 2001;
inline constexpr PacketType DL_PACKET_END = 2002;

inline constexpr PacketType CB_PACKET_START = 3000;
inline constexpr PacketType CB = 3001;
inline constexpr PacketType CB_PACKET_END = 3002;

inline constexpr PacketType BC_PACKET_START = 4000;
inline constexpr PacketType BC = 4001;
inline constexpr PacketType BC_ERROR = 4002;
inline constexpr PacketType BC_PACKET_END = 4003;

inline constexpr PacketType FM_PACKET_START = 5000;
inline constexpr PacketType FM = 5001;
inline constexpr PacketType FM_REQ_CM_BYPASS = 5002;
inline constexpr PacketType FM_ROOM_USER_ENTER = 5003;
inline constexpr PacketType FM_ROOM_USER_LEAVE = 5004;
inline constexpr PacketType FM_CHAT = 5005;
inline constexpr PacketType FM_PACKET_END = 5006;

inline constexpr PacketType BM_PACKET_START = 6000;
inline constexpr PacketType BM = 6001;
inline constexpr PacketType BM_PACKET_END = 6002;

inline constexpr PacketType DM_PACKET_START = 7000;
inline constexpr PacketType DM = 7001;
inline constexpr PacketType DM_ACK_AUTH_TICKET = 7002;
inline constexpr PacketType DM_REQ_GAME_CREATE = 7003;
inline constexpr PacketType DM_REQ_GAME_USER_ENTER = 7004;
inline constexpr PacketType DM_REQ_GAME_USER_LEAVE = 7005;
inline constexpr PacketType DM_REQ_GAME_FINISH = 7006;
inline constexpr PacketType DM_PACKET_END = 7007;

inline constexpr PacketType GM_PACKET_START = 8000;
inline constexpr PacketType GM = 8001;
inline constexpr PacketType GM_GAME_SERVER_READY = 8002;
inline constexpr PacketType GM_ACK_GAME_LOAD = 8003;
inline constexpr PacketType GM_ACK_GAME_CREATE = 8004;
inline constexpr PacketType GM_ACK_GAME_USER_ENTER = 8005;
inline constexpr PacketType GM_ACK_GAME_USER_LEAVE = 8006;
inline constexpr PacketType GM_PACKET_END = 8007;

inline constexpr PacketType LM_PACKET_START = 9000;
inline constexpr PacketType LM = 9001;
inline constexpr PacketType LM_PACKET_END = 9002;

inline constexpr PacketType SM_PACKET_START = 10000;
inline constexpr PacketType SM = 10001;
inline constexpr PacketType SM_PACKET_END = 10002;

inline constexpr PacketType MF_PACKET_START = 11000;
inline constexpr PacketType MF = 11001;
inline constexpr PacketType MF_ACK_MC_BYPASS = 11002;
inline constexpr PacketType MF_ROOM_USER_ENTER = 11003;
inline constexpr PacketType MF_ROOM_USER_LEAVE = 11004;
inline constexpr PacketType MF_CHAT = 11005;
inline constexpr PacketType MF_PACKET_END = 11006;

inline constexpr PacketType MB_PACKET_START = 12000;
inline constexpr PacketType MB = 12001;
inline constexpr PacketType MB_PACKET_END = 12002;

inline constexpr PacketType MD_PACKET_START = 13000;
inline constexpr PacketType MD = 13001;
inline constexpr PacketType MD_REQ_AUTH_TICKET = 13002;
inline constexpr PacketType MD_ACK_GAME_CREATE = 13003;
inline constexpr PacketType MD_ACK_GAME_USER_ENTER = 13004;
inline constexpr PacketType MD_ACK_GAME_USER_LEAVE = 13005;
inline constexpr PacketType MD_ACK_GAME_FINISH = 13006;
inline constexpr PacketType MD_PACKET_END = 13007;

inline constexpr PacketType MG_PACKET_START = 14000;
inline constexpr PacketType MG = 14001;
inline constexpr PacketType MG_REQ_GAME_LOAD = 14002;
inline constexpr PacketType MG_REQ_GAME_CREATE = 14003;
inline constexpr PacketType MG_REQ_GAME_USER_ENTER = 14004;
inline constexpr PacketType MG_REQ_GAME_USER_LEAVE = 14005;
inline constexpr PacketType MG_PACKET_END = 14006;

inline constexpr PacketType ML_PACKET_START = 15000;
inline constexpr PacketType ML = 15001;
inline constexpr PacketType ML_PACKET_END = 15002;

inline constexpr PacketType MS_PACKET_START = 16000;
inline constexpr PacketType MS = 16001;
inline constexpr PacketType MS_PACKET_END = 16002;

inline constexpr PacketType CF_PACKET_START = 17000;
inline constexpr PacketType CF_REQ_PACKET_LIST = 17001;
inline constexpr PacketType CF_REQ_USER_LOGIN = 17002;
inline constexpr PacketType CF_REQ_GAME_CHANNEL_USER_ENTER = 17003;
inline constexpr PacketType CF_REQ_CHAT = 17004;
inline constexpr PacketType CF_PACKET_END = 17005;

inline constexpr PacketType FC_PACKET_START = 18000;
inline constexpr PacketType FC_ACK_PACKET_LIST = 18001;
inline constexpr PacketType FC_KICK = 18002;
inline constexpr PacketType FC_NOTIFY = 18003;
inline constexpr PacketType FC_ERROR = 18004;
inline constexpr PacketType FC_ACK_USER_LOGIN = 18005;
inline constexpr PacketType FC_ACK_GAME_CHANNEL_USER_ENTER = 18006;
inline constexpr PacketType FC_GAME_FINISHED = 18007;
inline constexpr PacketType FC_ACK_CHAT = 18008;
inline constexpr PacketType FC_CHAT = 18009;
inline constexpr PacketType FC_PACKET_END = 18010;

inline constexpr PacketType FD_PACKET_START = 19000;
inline constexpr PacketType FD_REQ_CD_BYPASS = 19001;
inline constexpr PacketType FD_REQ_USER_LOGIN = 19002;
inline constexpr PacketType FD_USER_LOGOUT = 19003;
inline constexpr PacketType FD_PACKET_END = 19004;

inline constexpr PacketType DF_PACKET_START = 20000;
inline constexpr PacketType DF_ACK_DC_BYPASS = 20001;
inline constexpr PacketType DF_ACK_USER_LOGIN = 20002;
inline constexpr PacketType DF_USER_DISCONNECT = 20003;
inline constexpr PacketType DF_PACKET_END = 20004;

inline constexpr PacketType FG_PACKET_START = 21000;
inline constexpr PacketType FG_CG_BYPASS = 21001;
inline constexpr PacketType FG_REQ_GAME_CHANNEL_USER_ENTER = 21002;
inline constexpr PacketType FG_PACKET_END = 21003;

inline constexpr PacketType GF_PACKET_START = 22000;
inline constexpr PacketType GF_GC_BYPASS = 22001;
inline constexpr PacketType GF_GC_BYPASS_GAME = 22002;
inline constexpr PacketType GF_ACK_GAME_CHANNEL_USER_ENTER = 22003;
inline constexpr PacketType GF_GAME_CHANNEL_USER_LEAVE = 22004;
inline constexpr PacketType GF_GAME_FINISHED = 22005;
inline constexpr PacketType GF_PACKET_END = 22006;

inline constexpr PacketType FB_PACKET_START = 23000;
inline constexpr PacketType FB_REQ_CB_BYPASS = 23001;
inline constexpr PacketType FB_PACKET_END = 23002;

inline constexpr PacketType BF_PACKET_START = 24000;
inline constexpr PacketType BF_ACK_BC_BYPASS = 24001;
inline constexpr PacketType BF_PACKET_END = 24002;

inline constexpr PacketType CM_PACKET_START = 25000;
inline constexpr PacketType CM_SHELL_COMMAND = 25001;
inline constexpr PacketType CM_REQ_SERVER_LIST = 25002;
inline constexpr PacketType CM_REQ_ACCOUNT_USER_LIST = 25003;
inline constexpr PacketType CM_REQ_ACCOUNT_USER_CREATE = 25004;
inline constexpr PacketType CM_REQ_ACCOUNT_USER_DELETE = 25005;
inline constexpr PacketType CM_REQ_AUTH_TICKET = 25006;
inline constexpr PacketType CM_REQ_AUTH_MODIFY = 25007;
inline constexpr PacketType CM_REQ_GAME_LIST_OVER = 25008;
inline constexpr PacketType CM_REQ_GAME_LIST_UNDER = 25009;
inline constexpr PacketType CM_PACKET_END = 25010;

inline constexpr PacketType DC_PACKET_START = 26000;
inline constexpr PacketType DC_ERROR = 26001;
inline constexpr PacketType DC_ACK_CHEAT = 26002;
inline constexpr PacketType DC_ACK_STATIC_DATA_CHECKSUM = 26003;
inline constexpr PacketType DC_ACK_STATIC_DATA = 26004;
inline constexpr PacketType DC_ACK_USER_DATA = 26005;
inline constexpr PacketType DC_ACK_USER_LEVEL_UP = 26006;
inline constexpr PacketType DC_ACK_ITEM_USE = 26007;
inline constexpr PacketType DC_ACK_GAME_CREATE = 26008;
inline constexpr PacketType DC_ACK_GAME_USER_ENTER = 26009;
inline constexpr PacketType DC_ACK_GAME_USER_LEAVE = 26010;
inline constexpr PacketType DC_ACK_MAIL_LIST = 26011;
inline constexpr PacketType DC_ACK_MAIL_READ = 26012;
inline constexpr PacketType DC_PACKET_END = 26013;

inline constexpr PacketType GC_PACKET_START = 27000;
inline constexpr PacketType GC_ERROR = 27001;
inline constexpr PacketType GC_GAME_CHANNEL_USER_ENTER = 27002;
inline constexpr PacketType GC_GAME_CHANNEL_USER_LEAVE = 27003;
inline constexpr PacketType GC_WORLD_DATA = 27004;
inline constexpr PacketType GC_GAME_UPDATE = 27005;
inline constexpr PacketType GC_PACKET_END = 27006;

inline constexpr PacketType MC_PACKET_START = 28000;
inline constexpr PacketType MC_ERROR = 28001;
inline constexpr PacketType MC_ACK_SERVER_LIST = 28002;
inline constexpr PacketType MC_ACK_ACCOUNT_USER_LIST = 28003;
inline constexpr PacketType MC_ACK_ACCOUNT_USER_CREATE = 28004;
inline constexpr PacketType MC_ACK_ACCOUNT_USER_DELETE = 28005;
inline constexpr PacketType MC_ACK_AUTH_TICKET = 28006;
inline constexpr PacketType MC_ACK_AUTH_MODIFY = 28007;
inline constexpr PacketType MC_ACK_GAME_LIST_OVER = 28008;
inline constexpr PacketType MC_ACK_GAME_LIST_UNDER = 28009;
inline constexpr PacketType MC_PACKET_END = 28010;

inline constexpr PacketType CD_PACKET_START = 29000;
inline constexpr PacketType CD_REQ_CHEAT = 29001;
inline constexpr PacketType CD_REQ_STATIC_DATA_CHECKSUM = 29002;
inline constexpr PacketType CD_REQ_STATIC_DATA = 29003;
inline constexpr PacketType CD_REQ_USER_DATA = 29004;
inline constexpr PacketType CD_REQ_USER_LEVEL_UP = 29005;
inline constexpr PacketType CD_REQ_ITEM_USE = 29006;
inline constexpr PacketType CD_REQ_GAME_CREATE = 29007;
inline constexpr PacketType CD_REQ_GAME_USER_ENTER = 29008;
inline constexpr PacketType CD_REQ_GAME_USER_LEAVE = 29009;
inline constexpr PacketType CD_REQ_MAIL_LIST = 29010;
inline constexpr PacketType CD_REQ_MAIL_READ = 29011;
inline constexpr PacketType CD_PACKET_END = 29012;

inline constexpr PacketType GD_PACKET_START = 30000;
inline constexpr PacketType GD_REQ_STATIC_DATA_CHECKSUM = 30001;
inline constexpr PacketType GD_REQ_STATIC_DATA = 30002;
inline constexpr PacketType GD_ACK_ITEM_USE = 30003;
inline constexpr PacketType GD_ACK_GAME_USER_DATA_LOAD = 30004;
inline constexpr PacketType GD_GAME_USER_ITEM_UPDATE = 30005;
inline constexpr PacketType GD_REQ_GAME_FINISH = 30006;
inline constexpr PacketType GD_PACKET_END = 30007;

inline constexpr PacketType DG_PACKET_START = 31000;
inline constexpr PacketType DG_ACK_STATIC_DATA_CHECKSUM = 31001;
inline constexpr PacketType DG_ACK_STATIC_DATA = 31002;
inline constexpr PacketType DG_REQ_ITEM_USE = 31003;
inline constexpr PacketType DG_REQ_GAME_USER_DATA_LOAD = 31004;
inline constexpr PacketType DG_GAME_USER_STAT_ALL = 31005;
inline constexpr PacketType DG_GAME_USER_STAT_DIFF = 31006;
inline constexpr PacketType DG_ACK_GAME_FINISH = 31007;
inline constexpr PacketType DG_PACKET_END = 31008;

inline constexpr PacketType CG_PACKET_START = 32000;
inline constexpr PacketType CG_WORLD_USER_COMMAND = 32001;
inline constexpr PacketType CG_REWIND_WORLD_DATA = 32002;
inline constexpr PacketType CG_PACKET_END = 32003;

inline constexpr PacketType END = 33000;
}
PACKET_API const wchar_t* GetPacketTypeString(PacketType packetType);
PACKET_API PacketType GetPacketTypeEnum(const wchar_t* packetTypeString);
#include "Packet/PacketFuncs/PacketTypeFormatter.h"
