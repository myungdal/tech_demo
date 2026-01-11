// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;

#include "TestBadWordFilter.h"

#include "Common/BadWordFilter/BadWordFilter.h"


void TestBadWordFilter::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestBadWordFilter::OnStarted()
{
	std::wcout << L"=== BadWordFilter Test Start ===" << std::endl;

	int testCount = 0;
	int passCount = 0;

	// Test 1: Basic bad word detection
	{
		std::wcout << L"Test 1: Basic bad word detection" << std::endl;

		BadWordFilter filter;
		filter.InsertWord(L"badword", BadWordType::BLACK);
		filter.InsertWord(L"curse", BadWordType::BLACK);

		bool test1a = !filter.ValidateText(L"this is a badword here");
		bool test1b = !filter.ValidateText(L"curse detected");
		bool test1c = filter.ValidateText(L"normal sentence");

		std::wcout << L"  'badword' detection: " << (test1a ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  'curse' detection: " << (test1b ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  Normal text pass: " << (test1c ? L"OK" : L"FAIL") << std::endl;

		testCount++;
		if (test1a && test1b && test1c)
		{
			passCount++;
			std::wcout << L"  Basic detection test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Basic detection test: FAIL" << std::endl;
		}
	}

	// Test 2: Exact word matching (bug fix verification)
	{
		std::wcout << L"Test 2: Exact word matching (bug fix verification)" << std::endl;

		BadWordFilter filter;
		filter.InsertWord(L"bad", BadWordType::BLACK);

		bool test2a = !filter.ValidateText(L"bad");
		bool test2b = !filter.ValidateText(L"this is bad");
		bool test2c = !filter.ValidateText(L"badword");
		bool test2d = filter.ValidateText(L"good");

		std::wcout << L"  'bad' exact match: " << (test2a ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  Text ending with 'bad': " << (test2b ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  Text starting with 'bad': " << (test2c ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  Text without bad word: " << (test2d ? L"OK" : L"FAIL") << std::endl;

		testCount++;
		if (test2a && test2b && test2c && test2d)
		{
			passCount++;
			std::wcout << L"  Exact matching test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Exact matching test: FAIL" << std::endl;
		}
	}

	// Test 3: Empty text and empty filter handling
	{
		std::wcout << L"Test 3: Empty text and filter handling" << std::endl;

		BadWordFilter emptyFilter;
		BadWordFilter filter;
		filter.InsertWord(L"test", BadWordType::BLACK);

		bool test3a = emptyFilter.ValidateText(L"any text");
		bool test3b = emptyFilter.ValidateText(L"");
		bool test3c = filter.ValidateText(L"");

		std::wcout << L"  Empty filter, normal text: " << (test3a ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  Empty filter, empty text: " << (test3b ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  Filter with word, empty text: " << (test3c ? L"OK" : L"FAIL") << std::endl;

		testCount++;
		if (test3a && test3b && test3c)
		{
			passCount++;
			std::wcout << L"  Empty handling test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Empty handling test: FAIL" << std::endl;
		}
	}

	// Test 4: Prefix bad words (short word is prefix of long word)
	{
		std::wcout << L"Test 4: Prefix bad words test" << std::endl;

		BadWordFilter filter;
		filter.InsertWord(L"ass", BadWordType::BLACK);
		filter.InsertWord(L"assassin", BadWordType::BLACK);

		bool test4a = !filter.ValidateText(L"ass");
		bool test4b = !filter.ValidateText(L"assassin");
		bool test4c = !filter.ValidateText(L"I am an assassin");
		bool test4d = !filter.ValidateText(L"class");

		std::wcout << L"  'ass' match: " << (test4a ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  'assassin' match: " << (test4b ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  'assassin' in sentence: " << (test4c ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  'ass' in 'class': " << (test4d ? L"OK" : L"FAIL") << std::endl;

		testCount++;
		if (test4a && test4b && test4c && test4d)
		{
			passCount++;
			std::wcout << L"  Prefix bad words test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Prefix bad words test: FAIL" << std::endl;
		}
	}

	// Test 5: Whitelist functionality
	{
		std::wcout << L"Test 5: Whitelist functionality test" << std::endl;

		BadWordFilter filter;
		filter.InsertWord(L"ban", BadWordType::BLACK);
		filter.InsertWord(L"banana", BadWordType::WHITE);

		bool test5a = !filter.ValidateText(L"you are banned");
		bool test5b = filter.ValidateText(L"I like banana");

		std::wcout << L"  'ban' blocked: " << (test5a ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  'banana' whitelisted: " << (test5b ? L"OK" : L"FAIL") << std::endl;

		testCount++;
		if (test5a && test5b)
		{
			passCount++;
			std::wcout << L"  Whitelist test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Whitelist test: FAIL" << std::endl;
		}
	}

	// Test 6: Unicode Korean handling
	{
		std::wcout << L"Test 6: Unicode Korean handling" << std::endl;

		BadWordFilter filter;
		filter.InsertWord(L"\uC695\uC124", BadWordType::BLACK);
		filter.InsertWord(L"\uC88B\uC740\uB9D0", BadWordType::WHITE);

		bool test6a = !filter.ValidateText(L"\uC695\uC124\uD558\uC9C0\uB9C8");
		bool test6b = filter.ValidateText(L"\uC88B\uC740\uB9D0\uB9CC\uD574\uC694");
		bool test6c = filter.ValidateText(L"\uC548\uB155\uD558\uC138\uC694");

		std::wcout << L"  Korean bad word detected: " << (test6a ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  Korean whitelist pass: " << (test6b ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  Korean normal text pass: " << (test6c ? L"OK" : L"FAIL") << std::endl;

		testCount++;
		if (test6a && test6b && test6c)
		{
			passCount++;
			std::wcout << L"  Unicode Korean test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Unicode Korean test: FAIL" << std::endl;
		}
	}

	// Test 7: Multiple bad words detection
	{
		std::wcout << L"Test 7: Multiple bad words detection" << std::endl;

		BadWordFilter filter;
		filter.InsertWord(L"one", BadWordType::BLACK);
		filter.InsertWord(L"two", BadWordType::BLACK);
		filter.InsertWord(L"three", BadWordType::BLACK);

		bool test7a = !filter.ValidateText(L"one");
		bool test7b = !filter.ValidateText(L"two");
		bool test7c = !filter.ValidateText(L"three");
		bool test7d = !filter.ValidateText(L"one two three");
		bool test7e = filter.ValidateText(L"four five six");

		std::wcout << L"  'one' detected: " << (test7a ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  'two' detected: " << (test7b ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  'three' detected: " << (test7c ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  Multiple detection: " << (test7d ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  No bad words: " << (test7e ? L"OK" : L"FAIL") << std::endl;

		testCount++;
		if (test7a && test7b && test7c && test7d && test7e)
		{
			passCount++;
			std::wcout << L"  Multiple bad words test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Multiple bad words test: FAIL" << std::endl;
		}
	}

	// Test 8: Single character bad word
	{
		std::wcout << L"Test 8: Single character bad word" << std::endl;

		BadWordFilter filter;
		filter.InsertWord(L"X", BadWordType::BLACK);

		bool test8a = !filter.ValidateText(L"X");
		bool test8b = !filter.ValidateText(L"XXX");
		bool test8c = !filter.ValidateText(L"aXb");
		bool test8d = filter.ValidateText(L"abc");

		std::wcout << L"  Single 'X' match: " << (test8a ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  'XXX' detected: " << (test8b ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  'aXb' detected: " << (test8c ? L"OK" : L"FAIL") << std::endl;
		std::wcout << L"  No X in text: " << (test8d ? L"OK" : L"FAIL") << std::endl;

		testCount++;
		if (test8a && test8b && test8c && test8d)
		{
			passCount++;
			std::wcout << L"  Single character test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Single character test: FAIL" << std::endl;
		}
	}

	// Result summary
	std::wcout << L"=== Test Results ===" << std::endl;
	std::wcout << L"Passed: " << passCount << L"/" << testCount << L" tests" << std::endl;
	if (passCount == testCount)
	{
		std::wcout << L"Overall: SUCCESS" << std::endl;
	}
	else
	{
		std::wcout << L"Overall: FAILED" << std::endl;
	}
	std::wcout << L"=== BadWordFilter Test Complete ===" << std::endl;
}

void TestBadWordFilter::OnUpdate()
{
}
