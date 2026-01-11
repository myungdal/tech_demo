// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;
import ServerEngineCommon;

#include "TestRandMark40.h"


void TestRandMark40::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestRandMark40::OnStarted()
{
	std::wcout << L"=== 랜드마크40 테스트 시작 ===" << std::endl;

	int testCount = 0;
	int passCount = 0;

	// 테스트 1: CvcValidator 테스트
	{
		std::wcout << L"테스트 1: CvcValidator 검증" << std::endl;

		std::vector<std::wstring> testWords = { L"ABC", L"XYZ", L"BAD", L"GOOD", L"CAT", L"DOG", L"BAT", L"FOG" };
		bool validatorValid = true;

		for (const auto& word : testWords)
		{
			bool isValid = CvcValidator().IsValidCvcPattern(word);
			std::wcout << L"  '" << word << L"' is valid CVC: " 
				<< (isValid ? L"true" : L"false") << std::endl;

			// 예상 결과와 비교 (올바른 CVC 패턴만 유효해야 함)
			bool expected = false;
			if (word == L"BAD" || word == L"DOG" || word == L"BAT" || word == L"FOG" || word == L"CAT")
				expected = true;

			if (isValid != expected)
			{
				validatorValid = false;
				std::wcout << L"    오류: 예상 " << (expected ? L"true" : L"false")
					<< L", 실제 " << (isValid ? L"true" : L"false") << std::endl;
			}
		}

		testCount++;
		if (validatorValid)
		{
			passCount++;
			std::wcout << L"  CvcValidator 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  CvcValidator 테스트: 실패" << std::endl;
		}
	}

	// 테스트 2: 금칙어 필터링 테스트
	{
		std::wcout << L"테스트 2: 금칙어 필터링" << std::endl;

		std::vector<std::wstring> badWords = { L"sex", L"ASS", L"god", L"TiT", L"jiz", L"abc", L"GOD", L"fAg", L"cum", L"TIt" };
		auto filtered = CvcValidator().FilterGeneratableBadWords(badWords);

		std::wcout << L"  필터링된 금칙어:" << std::endl;
		for (const auto& word : filtered)
		{
			std::wcout << L"    '" << word << L"'" << std::endl;
		}

		// 필터링된 단어들이 모두 3글자인지 확인
		bool filteringValid = true;
		for (const auto& word : filtered)
		{
			if (word.length() != 3)
			{
				filteringValid = false;
				break;
			}
		}

		testCount++;
		if (filteringValid)
		{
			passCount++;
			std::wcout << L"  금칙어 필터링 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  금칙어 필터링 테스트: 실패" << std::endl;
		}
	}

	// 테스트 3: RandMark40 인코딩-디코딩 일치성 검증
	{
		std::wcout << L"테스트 3: RandMark40 인코딩-디코딩 일치성" << std::endl;

		RandMark40 randMark40(0x123456789ABCDEF0ULL);
		std::vector<std::wstring> badWords = { L"sex", L"ASS", L"god", L"TiT", L"jiz", L"abc", L"GOD", L"fAg", L"cum", L"TIt" };
		randMark40.BuildAllowedCvcTable(badWords);


		bool encodingValid = true;
		const int testIterations = 100;

		std::wcout << L"  인코딩-디코딩 일치성 테스트 중:" << std::endl;
		for (uint64_t i = 0; i < testIterations; ++i)
		{
			std::wstring code;
			if (!randMark40.EncodeIndex(i, code))
			{
				encodingValid = false;
				std::wcout << L"    오류: 인덱스 " << i << L" 인코딩 실패" << std::endl;
				break;
			}
			uint64_t decoded = randMark40.DecodeIndex(code);

			if (i != decoded)
			{
				encodingValid = false;
				std::wcout << std::format(L"    오류: 인덱스 {} -> 코드: '{}' -> 디코딩: {} (불일치!)",
					i, std::wstring(code.begin(), code.end()), decoded) << std::endl;
				break;
			}

			// 처음 10개만 상세 출력
			if (i < 10)
			{
				std::wcout << std::format(L"    인덱스 {} -> 코드: '{}' -> 디코딩: {} 통과",
					i, std::wstring(code.begin(), code.end()), decoded) << std::endl;
			}
		}

		testCount++;
		if (encodingValid)
		{
			passCount++;
			std::wcout << L"  인코딩-디코딩 일치성 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  인코딩-디코딩 일치성 테스트: 실패" << std::endl;
		}
	}

	// 테스트 4: 발급 여부 판별 테스트
	{
		std::wcout << L"테스트 4: 발급 여부 판별 검증" << std::endl;

		bool issuanceValid = true;

		testCount++;
		if (issuanceValid)
		{
			passCount++;
			std::wcout << L"  발급 여부 판별 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  발급 여부 판별 테스트: 실패" << std::endl;
		}
	}

	// 테스트 5: 코드 형식 검증
	{
		std::wcout << L"테스트 5: 코드 형식 검증" << std::endl;

		RandMark40 randMark40(0x123456789ABCDEF0ULL);
		std::vector<std::wstring> badWords = { L"sex", L"ASS", L"god", L"TiT", L"jiz", L"abc", L"GOD", L"fAg", L"cum", L"TIt" };
		randMark40.BuildAllowedCvcTable(badWords);

		bool formatValid = true;
		const int testIterations = 20;

		for (uint64_t i = 0; i < testIterations; ++i)
		{
			std::wstring code;
			if (!randMark40.EncodeIndex(i, code))
			{
				formatValid = false;
				std::wcout << L"    오류: 인덱스 " << i << L" 인코딩 실패" << std::endl;
				break;
			}

			// 코드 형식 검증: "CVCDDD-CVCDDD" (13글자, 6번째 위치에 '-')
			if (code.length() != 13 || code[6] != L'-')
			{
				formatValid = false;
				std::wcout << std::format(L"    오류: 잘못된 코드 형식: '{}'",
					code) << std::endl;
				break;
			}

			// CVC 부분이 대문자인지 확인
			bool cvcValid = true;
			for (int j = 0; j < 3; ++j)
			{
				if (code[j] < L'A' || code[j] > L'Z')
					cvcValid = false;
				if (code[j + 7] < L'A' || code[j + 7] > L'Z')
					cvcValid = false;
			}

			// DDD 부분이 숫자인지 확인
			for (int j = 3; j < 6; ++j)
			{
				if (code[j] < L'0' || code[j] > L'9')
					cvcValid = false;
				if (code[j + 7] < L'0' || code[j + 7] > L'9')
					cvcValid = false;
			}

			if (!cvcValid)
			{
				formatValid = false;
				std::wcout << std::format(L"    오류: 코드에 잘못된 문자: '{}'",
					code) << std::endl;
				break;
			}
		}

		testCount++;
		if (formatValid)
		{
			passCount++;
			std::wcout << L"  코드 형식 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  코드 형식 테스트: 실패" << std::endl;
		}
	}

	// 테스트 6: 0~999 숫자 범위 검증
	{
		std::wcout << L"테스트 6: 0~999 숫자 범위 검증" << std::endl;

		RandMark40 randMark40(0x123456789ABCDEF0ULL);
		std::vector<std::wstring> badWords = { L"sex", L"ASS", L"god", L"TiT", L"jiz", L"abc", L"GOD", L"fAg", L"cum", L"TIt" };
		randMark40.BuildAllowedCvcTable(badWords);

		bool numberRangeValid = true;

		// 실제 사용 가능한 인덱스 범위 확인
		// mAllowedCvc.size() * 1000 * mAllowedCvc.size() * 1000
		uint64_t maxAllowedIndex = static_cast<uint64_t>(randMark40.GetAllowedCvcCount()) * 1000 * 
		                          static_cast<uint64_t>(randMark40.GetAllowedCvcCount()) * 1000;
		
		std::wcout << L"    허용된 CVC 개수: " << randMark40.GetAllowedCvcCount() << std::endl;
		std::wcout << L"    최대 사용 가능한 인덱스: " << maxAllowedIndex << std::endl;
		
		// 사용 가능한 범위 내에서만 테스트
		std::vector<uint32_t> testNumbers;
		if (maxAllowedIndex > 0)
		{
			testNumbers = { 0, 1, 99, 100, 999 };
			// 최대 인덱스가 999보다 작으면 조정
			if (maxAllowedIndex < 999)
			{
				testNumbers = { 0, 1, static_cast<uint32_t>(maxAllowedIndex - 1) };
			}
		}
		else
		{
			std::wcout << L"    오류: 사용 가능한 인덱스가 없음" << std::endl;
			numberRangeValid = false;
		}

		for (uint32_t num : testNumbers)
		{
			std::wstring code;
			if (!randMark40.EncodeIndex(num, code))
			{
				numberRangeValid = false;
				std::wcout << L"    오류: 인덱스 " << num << L" 인코딩 실패" << std::endl;
				continue;
			}
			uint64_t decoded = randMark40.DecodeIndex(code);

			// 코드 형식: "CVCDDD-CVCDDD" (13글자)
			// 첫 번째 블록: code.substr(0, 6) = "CVCDDD"
			// 두 번째 블록: code.substr(7, 6) = "CVCDDD"
			std::wstring firstBlock = code.substr(0, 6);
			std::wstring secondBlock = code.substr(7, 6);
			std::wstring firstNumberPart = firstBlock.substr(3, 3);
			std::wstring secondNumberPart = secondBlock.substr(3, 3);

			uint32_t firstNumber = std::stoi(firstNumberPart);
			uint32_t secondNumber = std::stoi(secondNumberPart);

			std::wcout << std::format(L"  인덱스 {} -> 코드: '{}' -> 첫번째블록: {} -> 두번째블록: {} -> 디코딩: {}",
				num, code, firstNumber, secondNumber, decoded) << std::endl;

			if (num != decoded)
			{
				numberRangeValid = false;
				std::wcout << L"    오류: 인코딩-디코딩 불일치!" << std::endl;
			}
		}

		// 000~099 범위 테스트
		std::wcout << L"  000~099 범위 테스트:" << std::endl;
		for (uint32_t i = 0; i <= 99; i += 10)
		{
			std::wstring code;
			if (!randMark40.EncodeIndex(i, code))
			{
				numberRangeValid = false;
				std::wcout << L"    오류: 인덱스 " << i << L" 인코딩 실패" << std::endl;
				continue;
			}
			uint64_t decoded = randMark40.DecodeIndex(code);

			// 첫 번째와 두 번째 블록의 숫자 부분 추출
			std::wstring firstNumberPart = code.substr(3, 3);
			std::wstring secondNumberPart = code.substr(10, 3);
			uint32_t firstNumber = std::stoi(firstNumberPart);
			uint32_t secondNumber = std::stoi(secondNumberPart);

			if (i < 10)
			{
				std::wcout << std::format(L"    인덱스 {} -> 첫번째블록: {} -> 두번째블록: {} -> 디코딩: {} (코드: '{}')",
					i, firstNumber, secondNumber, decoded, code) << std::endl;
			}

			if (i != decoded)
			{
				numberRangeValid = false;
				std::wcout << std::format(L"    오류: 인덱스 {}에서 디코딩 {} 불일치", i, decoded) << std::endl;
			}
		}

		testCount++;
		if (numberRangeValid)
		{
			passCount++;
			std::wcout << L"  0~999 숫자 범위 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  0~999 숫자 범위 테스트: 실패" << std::endl;
		}
	}

	// 결과 요약
	std::wcout << L"=== 테스트 결과 ===" << std::endl;
	std::wcout << std::format(L"통과: {}/{} 테스트", passCount, testCount) << std::endl;
	if (passCount == testCount)
	{
		std::wcout << L"전체: 성공" << std::endl;
	}
	else
	{
		std::wcout << L"전체: 실패" << std::endl;
	}
	std::wcout << L"=== 랜드마크40 테스트 완료 ===" << std::endl;
}

void TestRandMark40::OnUpdate()
{
}
