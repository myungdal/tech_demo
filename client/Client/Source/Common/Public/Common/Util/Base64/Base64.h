// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base64 인코딩/디코딩 네임스페이스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Base64
{
	// 바이너리 데이터를 Base64로 인코딩한다.
	// - buf: 출력 버퍼
	// - src: 원본 데이터
	// - srcSize: 원본 데이터 크기 (bytes)
	// - return: 인코딩된 데이터 크기 (bytes)
	size_t Encode(OUT uint8_t* buf, const uint8_t* src, size_t srcSize);

	// Base64 데이터를 디코딩한다.
	// - buf: 출력 버퍼
	// - src: Base64 인코딩된 데이터
	// - srcSize: 인코딩된 데이터 크기 (bytes)
	// - return: 디코딩된 데이터 크기 (bytes)
	size_t Decode(OUT uint8_t* buf, const uint8_t* src, size_t srcSize);
};

#if 0
#include "Packet/Util/Base64.h"
uint8_t buf0[1024];
uint8_t buf1[1024];
std::string src("안녕하세요");
size_t size0 = Base64::Encode(buf0, (uint8_t*)src.data(), (src.size() + 1) * sizeof(char));
size_t size1 = Base64::Decode(buf1, buf0, size0);
size1;
#endif
