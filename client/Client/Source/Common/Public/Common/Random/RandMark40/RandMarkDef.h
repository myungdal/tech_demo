// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <array>
#include <cstdint>
#include <string>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RandMark 공통 정의 (RandMark Common Definitions)
// RandMark40과 CvcValidator에서 공통으로 사용하는 상수와 타입 정의
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace RandMark
{
	// 자음 집합 (16개) - 인지적으로 구별하기 쉬운 자음들
	static constexpr std::array<wchar_t, 16> CONSONANTS = {
		L'B',L'C',L'D',L'F',L'G',L'H',L'J',L'K',
		L'L',L'M',L'N',L'P',L'R',L'S',L'T',L'W'
	};
	
	// 모음 집합 (4개) - 가장 기본적이고 구별하기 쉬운 모음들
	static constexpr std::array<wchar_t, 4> VOWELS = { 
		L'A',L'E',L'I',L'O' 
	};
	
	// 10bit 구조 상수 정의
	static constexpr size_t CONSONANT_COUNT = CONSONANTS.size();  // 16개
	static constexpr size_t VOWEL_COUNT = VOWELS.size();          // 4개
	static constexpr size_t MAX_CVC_COUNT = CONSONANT_COUNT * VOWEL_COUNT * CONSONANT_COUNT; // 16 * 4 * 16 = 1024 (10bit)
	
	// 10bit DDD 상수 (0-999)
	static constexpr uint32_t DDD_COUNT = 1000;        // 000..999 (10bit)
	static constexpr uint32_t DDD_MIN = 0;
	static constexpr uint32_t DDD_MAX = 999;
	
	// 40bit 구조 상수 (10bit × 4개 순서)
	static constexpr size_t SEGMENT_COUNT = 4;         // 4개 세그먼트
	static constexpr size_t BITS_PER_SEGMENT = 10;     // 각 세그먼트당 10bit
	static constexpr size_t TOTAL_BITS = 40;           // 전체 40bit (10bit × 4)
	
	// 세그먼트 타입 정의
	static constexpr size_t CVC_SEGMENT_1 = 0;         // 1번째 CVC (10bit)
	static constexpr size_t DDD_SEGMENT_2 = 1;         // 2번째 DDD (10bit)
	static constexpr size_t CVC_SEGMENT_3 = 2;         // 3번째 CVC (10bit)
	static constexpr size_t DDD_SEGMENT_4 = 3;         // 4번째 DDD (10bit)
	
	// 레거시 호환성을 위한 별칭
	static constexpr uint32_t NUM_COUNT = DDD_COUNT;
	static constexpr uint32_t MIN_NUMBER = DDD_MIN;
	static constexpr uint32_t MAX_NUMBER = DDD_MAX;
	
	// CVC 패턴 검증을 위한 헬퍼 함수들
	namespace Utils
	{
		// 문자가 자음인지 확인
		constexpr bool IsConsonant(wchar_t ch) noexcept
		{
			for (wchar_t consonant : CONSONANTS)
			{
				if (consonant == ch)
					return true;
			}
			return false;
		}
		
		// 문자가 모음인지 확인
		constexpr bool IsVowel(wchar_t ch) noexcept
		{
			for (wchar_t vowel : VOWELS)
			{
				if (vowel == ch)
					return true;
			}
			return false;
		}
		
		// CVC 패턴인지 확인 (3글자 문자열)
		constexpr bool IsCvcPattern(const wchar_t* str, size_t length) noexcept
		{
			if (length != 3)
				return false;
			
			return IsConsonant(str[0]) && IsVowel(str[1]) && IsConsonant(str[2]);
		}
		
		// 문자를 대문자로 변환
		constexpr wchar_t ToUpper(wchar_t ch) noexcept
		{
			return (ch >= L'a' && ch <= L'z') ? static_cast<wchar_t>(ch - L'a' + L'A') : ch;
		}
		
		// 문자를 소문자로 변환
		constexpr wchar_t ToLower(wchar_t ch) noexcept
		{
			return (ch >= L'A' && ch <= L'Z') ? static_cast<wchar_t>(ch - L'A' + L'a') : ch;
		}
	}
}
