// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PCH/pch_serverEngine.h"


constexpr ClockMs DEFAULT_CHATTING_DELAY = 1000ms;


class FrontUser;
using FrontUserPtr = std::shared_ptr<FrontUser>;
using FrontUserWeakPtr = std::weak_ptr<FrontUser>;
