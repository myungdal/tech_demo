// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Util/Base64/Base64.h"


namespace Base64
{
	static constexpr char encodingTable[] =
	{
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
		'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
		'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
		'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
	};

	static constexpr int8_t decodingTable[256] =
	{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
		52,53,54,55,56,57,58,59,60,61,-1,-1,-1, 0,-1,-1,
		-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
		15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
		-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
		41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	};

	size_t Encode(OUT uint8_t* buf, const uint8_t* src, size_t srcSize)
	{
		size_t outLen = 0;
		size_t i = 0;

		// [WHY] 3바이트씩 처리하여 4바이트 Base64 문자 생성
		for (; i + 2 < srcSize; i += 3)
		{
			uint32_t triple = (uint32_t(src[i]) << 16) |
				(uint32_t(src[i + 1]) << 8) |
				uint32_t(src[i + 2]);
			buf[outLen++] = encodingTable[(triple >> 18) & 0x3F];
			buf[outLen++] = encodingTable[(triple >> 12) & 0x3F];
			buf[outLen++] = encodingTable[(triple >> 6) & 0x3F];
			buf[outLen++] = encodingTable[triple & 0x3F];
		}

		// [WHY] 남은 바이트 처리 (패딩 추가)
		if (i < srcSize)
		{
			uint32_t triple = 0;
			size_t rem = srcSize - i;
			triple |= uint32_t(src[i]) << 16;
			if (rem > 1)
				triple |= uint32_t(src[i + 1]) << 8;

			buf[outLen++] = encodingTable[(triple >> 18) & 0x3F];
			buf[outLen++] = encodingTable[(triple >> 12) & 0x3F];
			buf[outLen++] = (rem > 1) ? encodingTable[(triple >> 6) & 0x3F] : '=';
			buf[outLen++] = '=';
		}

		buf[outLen] = '\0';
		return outLen;
	}

	size_t Decode(OUT uint8_t* buf, const uint8_t* src, size_t srcSize)
	{
		size_t outLen = 0;
		// [EDGE-CASE] Base64는 4바이트 단위이므로 크기가 4의 배수가 아니면 실패
		if (srcSize % 4 != 0)
			return 0;

		// [WHY] 패딩 개수 계산
		size_t padding = 0;
		if (srcSize >= 1 && src[srcSize - 1] == '=')
			++padding;
		if (srcSize >= 2 && src[srcSize - 2] == '=')
			++padding;

		// [WHY] 4바이트씩 처리하여 3바이트 원본 데이터 복원
		for (size_t i = 0; i < srcSize; i += 4)
		{
			int8_t vals[4];
			for (int j = 0; j < 4; ++j)
			{
				char c = src[i + j];
				int8_t d = decodingTable[static_cast<uint8_t>(c)];
				// [EDGE-CASE] 유효하지 않은 문자면 실패
				if (d < 0)
					return 0;
				vals[j] = d;
			}

			uint32_t triple = (uint32_t(vals[0]) << 18) |
				(uint32_t(vals[1]) << 12) |
				(uint32_t(vals[2]) << 6) |
				uint32_t(vals[3]);

			buf[outLen++] = (triple >> 16) & 0xFF;
			// [WHY] 패딩이 아닌 경우에만 바이트 출력
			if (src[i + 2] != '=')
				buf[outLen++] = (triple >> 8) & 0xFF;
			if (src[i + 3] != '=')
				buf[outLen++] = triple & 0xFF;
		}

		buf[outLen] = '\0';
		return outLen;
	}

} // namespace Base64
