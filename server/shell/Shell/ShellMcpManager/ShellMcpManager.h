// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <memory>
#include <stop_token>
#include <winsock2.h>
#include "json.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MCP(Model Context Protocol) 서버 관리자
// HTTP 기반 JSON-RPC 요청을 처리하여 외부 도구(Cursor 등)와 통신한다.
class ShellMcpManager
{
public:
	ShellMcpManager();
	~ShellMcpManager();

	// MCP 서버 메인 루프 실행 (포트 8080에서 대기)
	// [WHY] stop_token으로 graceful shutdown 지원
	void Run(std::stop_token stopToken);

private:
	// JSON-RPC 요청 처리
	// request: 파싱된 JSON-RPC 요청
	// return: JSON-RPC 응답
	nlohmann::json HandleRequest(const nlohmann::json& request);

	// HTTP 응답 전송
	// client: 클라이언트 소켓
	// response: 전송할 JSON 응답
	void SendHttpResponse(SOCKET client, const nlohmann::json& response);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::shared_ptr<ShellMcpManager> gShellMcpManager = nullptr;
