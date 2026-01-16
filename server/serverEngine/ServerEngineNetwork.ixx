// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ServerEngineNetwork.ixx - Network Module
// Network, Socket 제공

module;

// GMF - Network
#include "ServerEngine/Network/SendBuffer/SendBuffer.h"
#include "ServerEngine/Network/NetworkManager.h"
#include "ServerEngine/Network/Iocp/Iocp.h"
#include "ServerEngine/Network/RemoteSocketPool/RemoteSocketPool.h"
#include "ServerEngine/Network/RemoteRequestPool/RemoteRequestPool.h"

// GMF - Socket
#include "ServerEngine/Socket/Socket.h"
#include "ServerEngine/Socket/SocketBase/SocketBase.h"
#include "ServerEngine/Socket/SocketImpl/HostSocket.h"
#include "ServerEngine/Socket/SocketImpl/PeerSocket.h"
#include "ServerEngine/Socket/SocketPtr/SocketPtr.h"

export module ServerEngineNetwork;

// ============= Network =============
export using ::NetworkManager;
export using ::Iocp;
export using ::RemoteSocketPool;
export using ::RemoteRequestPool;
export using ::gNetworkManager;

export namespace SendBuffer {
	using ::SendBuffer::Pop;
	using ::SendBuffer::Push;
}

// ============= Socket =============
export using ::SocketBase;
export using ::HostSocket;
export using ::PeerSocket;
export using ::SocketPtr;

