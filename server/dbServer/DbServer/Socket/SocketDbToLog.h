// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/PeerSocket.h"
#include "ServerEngine/Socket/Socket.h"


class SocketDbToLog final : public Socket<SocketDbToLog, MAX_POOL_SIZE_FACTOR_SOCKET_DB_TO_LOG, PeerSocket>
{
};

inline std::shared_ptr<SocketDbToLog> gSocketDbToLog = nullptr;
