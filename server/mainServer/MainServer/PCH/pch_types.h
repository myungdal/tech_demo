// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// pch_types.h - MainServer 전용 타입 정의
// pch.h와 pch_module.h에서 공통으로 include

#pragma once


class Server;
using ServerPtr = std::shared_ptr<Server>;
using ServerMap = std::map<ServerId, ServerPtr>;
using ServerHomeGameList = std::list<std::tuple<ServerId, GameSid>>;


class Game;
using GamePtr = std::shared_ptr<Game>;
using GameMap = std::map<GameId, GamePtr>;
constexpr int64_t MAX_COUNT_PER_GAME_LIST_PGAE = 50;


class Room;
using RoomPtr = std::shared_ptr<Room>;


class Account;
using AccountPtr = std::shared_ptr<Account>;
using AccountList = std::list<AccountPtr>;
using AccountMapByAccountId = std::unordered_map<AccountId, AccountPtr>;
using AccountMapByToken = std::unordered_map<std::wstring, AccountPtr>;
