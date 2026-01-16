// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "json.hpp"

import Shell;

using json = nlohmann::json;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MCP 서버 설정 상수
namespace ServerConfig
{
	constexpr const char* SERVER_NAME = "mmo_mcp";
	constexpr const char* SERVER_VERSION = "0.1.0";
	constexpr const char* SERVER_COMMAND = "C:\\dev\\nearest3\\Build\\Debug\\16_shell\\16_shell.exe";
	constexpr const char* SERVER_CWD = "C:\\dev\\nearest3\\Build\\Debug\\16_shell";
	constexpr const char* SERVER_ARG_PATH = "C:/dev/nearest3/Build/Debug/16_shell";
	constexpr const char* PROTOCOL_VERSION = "2024-11-05";
}

// MCP 도구 설정 상수
namespace ToolConfig
{
	constexpr const char* TOOL_ID = "mmo_tool_id";
	constexpr const char* TOOL_NAME = "mmo_tool";
	constexpr const char* TOOL_DESCRIPTION = "mmo tool";
	constexpr const char* TOOL_VERSION = "1.0.0";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ShellMcpManager::ShellMcpManager()
{
}

ShellMcpManager::~ShellMcpManager()
{
}

json ShellMcpManager::HandleRequest(const json& request)
{
	json response;
	response["jsonrpc"] = "2.0";

	if (request.contains("id") && !request["id"].is_null())
		response["id"] = request["id"];
	else
		response["id"] = "";

	std::string method = request["method"];

	if (method == "initialize")
	{
		response["result"] = {
			{"protocolVersion", ServerConfig::PROTOCOL_VERSION},
			{"capabilities", {
				{"statusProvider", true},
				{"tools", json::object()},
				{"resources", json::object()}
			}},
			{"serverInfo", {
				{"name", ServerConfig::SERVER_NAME},
				{"version", ServerConfig::SERVER_VERSION},
				{"command", ServerConfig::SERVER_COMMAND},
				{"args", {"0", ServerConfig::SERVER_ARG_PATH}},
				{"cwd", ServerConfig::SERVER_CWD}
			}}
		};
	}
	else if (method == "notifications/initialized")
	{
		response["result"] = nullptr;
	}
	else if (method == "notifications/exit")
	{
		return json();
	}
	else if (method == "ping")
	{
		response["result"] = "pong";
	}
	else if (method == "ReloadClient")
	{
		response["result"] = {
			{"protocolVersion", ServerConfig::PROTOCOL_VERSION},
			{"serverInfo", {
				{"name", ServerConfig::SERVER_NAME},
				{"command", ServerConfig::SERVER_COMMAND},
				{"args", {"0", ServerConfig::SERVER_ARG_PATH}},
				{"cwd", ServerConfig::SERVER_CWD}
			}}
		};
	}
	else if (method == "ListOfferings")
	{
		response["result"] = {
			{"offerings", {{
				{"id", "offerings"},
				{"name", ServerConfig::SERVER_NAME},
				{"command", ServerConfig::SERVER_COMMAND},
				{"args", {"0", ServerConfig::SERVER_ARG_PATH}},
				{"cwd", ServerConfig::SERVER_CWD}
			}}}
		};
	}
	else if (method == "tools/list")
	{
		// [FIX] outputSchema 제거 - MCP 프로토콜에서 outputSchema가 있으면 structuredContent 필드가 필요하므로
		response["result"] = {
			{"tools", {{
				{"name", ToolConfig::TOOL_NAME},
				{"description", ToolConfig::TOOL_DESCRIPTION},
				{"inputSchema", {
					{"type", "object"},
					{"properties", {
						{"command", {
							{"type", "string"},
							{"description", "The command to execute"}
						}}
					}},
					{"required", {"command"}}
				}}
			}}}
		};
	}
	else if (method == "tools/call")
	{
		if (!request.contains("params") || !request["params"].is_object())
		{
			return {
				{"jsonrpc", "2.0"},
				{"error", {{"code", -32602}, {"message", "Invalid params"}}},
				{"id", request.contains("id") && !request["id"].is_null() ? request["id"] : ""}
			};
		}

		const auto& params = request["params"];
		if (!params.contains("arguments") || !params["arguments"].is_object())
		{
			return {
				{"jsonrpc", "2.0"},
				{"error", {{"code", -32602}, {"message", "Invalid arguments"}}},
				{"id", request.contains("id") && !request["id"].is_null() ? request["id"] : ""}
			};
		}

		const auto& arguments = params["arguments"];
		if (!arguments.contains("command") || !arguments["command"].is_string())
		{
			return {
				{"jsonrpc", "2.0"},
				{"error", {{"code", -32602}, {"message", "Missing or invalid command parameter"}}},
				{"id", request.contains("id") && !request["id"].is_null() ? request["id"] : ""}
			};
		}

		std::string commandA = arguments["command"];
		Buf_wchar_t commandW = StringUtil::utf8_to_w(commandA.c_str());
		std::wstring resultW = gShellCommandManager->Dispatch(*commandW);
		Buf_char resultA = StringUtil::w_to_utf8(resultW.c_str());

		response["result"]["content"] = {{
			{"type", "text"},
			{"text", *resultA}
		}};
	}
	else
	{
		response = {
			{"jsonrpc", "2.0"},
			{"error", {{"code", -32601}, {"message", "Method not found: " + method}}},
			{"id", request.contains("id") && !request["id"].is_null() ? request["id"] : ""}
		};
	}

	return response;
}

void ShellMcpManager::SendHttpResponse(SOCKET client, const json& response)
{
	std::string body = response.dump();
	std::string header = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n";
	std::string data = header + body;

	send(client, data.c_str(), (int)data.size(), 0);
}

void ShellMcpManager::Run(std::stop_token stopToken)
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return;

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8080);

	if (SOCKET_ERROR == bind(serverSocket, (sockaddr*)&addr, sizeof(addr)))
		return;

	if (SOCKET_ERROR == listen(serverSocket, SOMAXCONN))
		return;

	// [WHY] Non-blocking 모드로 설정하여 stop_requested() 체크 가능하게 함
	u_long nonBlocking = 1;
	ioctlsocket(serverSocket, FIONBIO, &nonBlocking);

	while (!stopToken.stop_requested())
	{
		SOCKET client = accept(serverSocket, nullptr, nullptr);
		if (client == INVALID_SOCKET)
		{
			// Non-blocking이라 WSAEWOULDBLOCK이면 대기
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			continue;
		}

		std::string req;
		char buf[1024];
		int recvSize;

		while (req.find("\r\n\r\n") == std::string::npos)
		{
			recvSize = recv(client, buf, sizeof(buf), 0);
			if (recvSize <= 0)
				break;
			req.append(buf, recvSize);
		}

		size_t headerEnd = req.find("\r\n\r\n");
		if (headerEnd == std::string::npos)
		{
			closesocket(client);
			continue;
		}

		std::string header = req.substr(0, headerEnd);
		std::string body = req.substr(headerEnd + 4);

		size_t contentLength = 0;
		std::istringstream hs(header);
		std::string requestLine;

		if (!std::getline(hs, requestLine))
		{
			closesocket(client);
			continue;
		}

		std::string method, path;
		std::istringstream rl(requestLine);
		rl >> method >> path;

		std::string line;
		while (std::getline(hs, line))
		{
			if (line.rfind("Content-Length:", 0) == 0)
			{
				contentLength = std::stoul(line.substr(15));
			}
		}

		// [WHY] SSE 연결 요청 처리 - Cursor 등 외부 도구의 연결 허용
		if (method == "GET" && path.rfind("/sse", 0) == 0)
		{
			std::string header2 =
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/event-stream\r\n"
				"Cache-Control: no-cache\r\n"
				"Connection: keep-alive\r\n\r\n";
			send(client, header2.c_str(), (int)header2.size(), 0);
			std::string event = "data: connected\n\n";
			send(client, event.c_str(), (int)event.size(), 0);
			closesocket(client);
			continue;
		}

		while (body.size() < contentLength)
		{
			recvSize = recv(client, buf, sizeof(buf), 0);
			if (recvSize <= 0)
				break;
			body.append(buf, recvSize);
		}

		json request;
		json response;

		try
		{
			request = json::parse(body);

			if (!request.contains("jsonrpc") || !request["jsonrpc"].is_string() || request["jsonrpc"] != "2.0" || !request.contains("method") || !request["method"].is_string())
			{
				response = {
					{"jsonrpc", "2.0"},
					{"error", {{"code", -32600}, {"message", "Invalid Request"}}},
					{"id", request.contains("id") && !request["id"].is_null() ? request["id"] : ""}
				};
			}
			else
			{
				response = HandleRequest(request);
			}
		}
		catch (const std::exception& e)
		{
			response = {
				{"jsonrpc", "2.0"},
				{"error", {{"code", -1}, {"message", std::string("Parse error: ") + e.what()}}},
				{"id", nullptr}
			};
		}

		if (!response.is_null())
			SendHttpResponse(client, response);

		closesocket(client);
		_YIELD_APP;
	}

	// [WHY] Graceful shutdown 시 서버 소켓 정리
	closesocket(serverSocket);
	WSACleanup();
}
