// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import BridgeServer;

// [WHY] BridgePacketHandlerAuth는 import BridgeServer를 통해 BridgeServerPacket 모듈에서 이미 export됨

// [TODO] 외부 인증 서비스 연동 구현 예정
// 예시:
// bool SocketBridgeFromFront::OnPacket(어떤 요청)
// {
//     do
//     {
//         HttpClient httpClient;
//
//         if (false == httpClient.PostStart(true, L"postman-echo.com", L"post"))
//             break;
//
//         if (false == httpClient.PostPushHeader(L"Content-Type:application/json"))
//             break;
//
//         std::string body("{\"email\":\"a@b.c\"}");
//         if (false == httpClient.PostEnd((uint8_t*)body.c_str(), body.length()))
//             break;
//
//         uint8_t* buf = TEMP_BUF;
//         if (false == httpClient.GetResponse(buf, MAX_PACKET_BUFFER_SIZE))
//             break;
//
//     } while (false);
// }
