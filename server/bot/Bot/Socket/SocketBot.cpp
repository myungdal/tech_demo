// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;

void SocketBot::OnEncrypt(uint8_t* buf, int32_t len, EncryptionKey encryptionKey)
{
	NetworkUtil::Encrypt(buf, len, encryptionKey);
}

