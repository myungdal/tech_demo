// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;
import ServerEngineCommon;

#include "TestFlowControlScript.h"

/**
 * FlowControlScript 테스트 파일
 * 
 * FlowControlScript는 Python-like Embedded Scripting DSL(Domain-Specific Language, 도메인 특화 언어)입니다.
 * 
 * 이 테스트는 FlowControlScript의 모든 주요 기능을 검증합니다:
 *  1. 기본 산술 연산 및 변수 할당
 *  2. 함수 정의 및 호출
 *  3. 조건문 (if/else)
 *  4. 논리 연산 (&&, ||)
 *  5. 재귀 함수 (피보나치, GCD)
 *  6. C++ 네이티브 함수 바인딩
 *  7. 전역 상수 사용
 *  8. 중첩된 함수 호출
 * 
 * 실행 흐름:
 *  소스 코드 문자열 → Lexer → Parser → Compiler → VirtualMachine → 결과
 */

// FlowControlScript 타입들은 ServerEngineCommon 모듈에서 전역으로 export됨

// C++ 인스턴스 클래스 정의 (네이티브 함수 바인딩 테스트용)
class CppInstance
{
private:
	std::vector<Value> mPrintedValues;

public:
	CppInstance() {
	}

	Value Add(Value a, Value b)
	{
		std::wcout << L"네이티브 함수 호출 로그: Add(" << a << L", " << b << L")" << std::endl;
		return a + b;
	}

	Value Sub(Value a, Value b)
	{
		std::wcout << L"네이티브 함수 호출 로그: Sub(" << a << L", " << b << L")" << std::endl;
		return a - b;
	}

	Value Mul(Value a, Value b)
	{
		std::wcout << L"네이티브 함수 호출 로그: Mul(" << a << L", " << b << L")" << std::endl;
		return a * b;
	}

	Value Div(Value a, Value b)
	{
		std::wcout << L"네이티브 함수 호출 로그: Div(" << a << L", " << b << L")" << std::endl;
		if (b == 0) return 0; // 0으로 나누기 방지
		return a / b;
	}

	Value Mod(Value a, Value b)
	{
		std::wcout << L"네이티브 함수 호출 로그: Mod(" << a << L", " << b << L")" << std::endl;
		if (b == 0) return 0; // 0으로 나누기 방지
		return a % b;
	}

	void Print(Value value)
	{
		std::wcout << L"네이티브 함수 호출 로그: Print(" << value << L")" << std::endl;
		mPrintedValues.push_back(value);
	}

	// 출력된 값들을 반환
	const std::vector<Value>& GetPrintedValues() const {
		return mPrintedValues;
	}
};

/**
 * RunTest - 개별 스크립트 테스트 실행
 * 
 * @param name 테스트 이름
 * @param source FlowControlScript 소스 코드
 * @param Call VM 실행 방식을 지정하는 람다 (Execute() 또는 CallFunc())
 * @param expectedResult 기대되는 반환값
 * @param expectedPrints Print() 함수로 출력된 값들의 기대값 목록
 * @return 테스트 성공 여부
 * 
 * 실행 단계:
 *  1. Lexer로 소스 코드를 토큰화
 *  2. Parser로 AST 생성
 *  3. Compiler로 바이트코드 생성 (네이티브 함수 바인딩 포함)
 *  4. VirtualMachine으로 실행
 *  5. 결과 검증 (반환값, Print 출력값)
 */
template<typename _Call>
bool RunTest([[maybe_unused]] const std::wstring& name, const std::wstring& source, _Call Call, [[maybe_unused]] Value expectedResult, [[maybe_unused]] std::vector<Value> expectedPrints)
{
	CppInstance cppInstance;

	std::wcout << L"  " << name << L" 테스트:" << std::endl;

	Lexer lexer(source);
	auto tokens = lexer.ScanTokens();

	Parser parser(tokens);
	auto astRoot = parser.Parse();

	Bytecode bytecode;
	Compiler generator(OUT bytecode);
	generator.BindNativeFunction(L"Add", cppInstance, &CppInstance::Add);
	generator.BindNativeFunction(L"Sub", cppInstance, &CppInstance::Sub);
	generator.BindNativeFunction(L"Mul", cppInstance, &CppInstance::Mul);
	generator.BindNativeFunction(L"Div", cppInstance, &CppInstance::Div);
	generator.BindNativeFunction(L"Mod", cppInstance, &CppInstance::Mod);
	generator.BindNativeFunction(L"Print", cppInstance, &CppInstance::Print);
	generator.Compile(astRoot);

	VirtualMachine vm(IN bytecode);
	Value result = Call(vm); //vm.Execute(); // ScriptFunc 호출

	if (result != expectedResult)
	{
		std::wcout << L"    결과 불일치: " << result << L" (예상: " << expectedResult << L")" << std::endl;
		return false;
	}

	// 출력 검증
	const auto& prints = cppInstance.GetPrintedValues();
	if (prints.size() != expectedPrints.size())
	{
		std::wcout << L"    출력 개수 불일치: " << prints.size() << L" (예상: " << expectedPrints.size() << L")" << std::endl;
		return false;
	}

	for (size_t i = 0; i < prints.size(); ++i)
	{
		if (prints[i] != expectedPrints[i])
		{
			std::wcout << L"    출력 불일치 인덱스 " << i
				<< L": " << prints[i] << L" (예상: " << expectedPrints[i] << L")" << std::endl;
			return false;
		}
	}

	std::wcout << L"    " << name << L" 테스트: 통과" << std::endl;
	return true;
}

/**
 * RunTests - 전체 테스트 스위트 실행
 * 
 * FlowControlScript의 모든 기능을 체계적으로 테스트합니다.
 * 각 테스트는 독립적으로 실행되며, 통과/실패 결과를 집계합니다.
 * 
 * 테스트 카테고리:
 *  - 기본 연산: 산술, 변수, 함수 호출
 *  - 제어 흐름: if/else, 중첩 조건문
 *  - 네이티브 바인딩: C++ 함수 호출, 전역 상수
 *  - 고급 기능: 재귀 함수, 상호 재귀, 중첩 호출
 *  - 실용 예제: 게임 로직 시뮬레이션 (아이템 사용, 그리드 계산)
 */
int RunTests()
{
	std::wcout << L"=== FlowControlScript 테스트 시작 ===" << std::endl;

	int testCount = 0;
	int passCount = 0;

	gGlobalEnvironment = std::make_shared<FlowControlScript::GlobalEnvironment>();

	gGlobalEnvironment->RegisterConstant(L"Result::SUCCEEDED", 123);
	gGlobalEnvironment->RegisterConstant(L"Result::DB_ERROR", 321);
	gGlobalEnvironment->RegisterConstant(L"GRID_SIZE", 1024);

	testCount++;
	if (RunTest(
		L"Foreign func execution",
		L"a = +Result::SUCCEEDED\n"
		L"b = -Result::DB_ERROR\n"
		L"c = Add(a, b)\n"
		L"Print(c)\n"
		L"return 54321\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			return vm.Execute();
		},
		54321, { 123 - 321 }
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"Basic arithmetic",
		L"def add(a, b):\n"
		L"    return a + b\n"
		L"\n"
		L"return add(10, 20)\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			return vm.Execute();
		},
		30, {}
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"If statement",
		L"def test(a, b):\n"
		L"    if (a > b || 2 < 3) && 5 < 2:\n"
		L"        return a\n"
		L"    else:\n"
		L"        return b\n"
		L"\n"
		L"return test(30, 20)\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			return vm.Execute();
		},
		20, {}
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"Variable assignment",
		L"def test():\n"
		L"    x = 10\n"
		L"    y = 20\n"
		L"    Print(x - y)\n"
		L"    Print(y - x)\n"
		L"    return x + y\n"
		L"\n"
		L"test()\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			return vm.Execute();
		},
		INVALID_VALUE, { -10, 10 }
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"Rectangle area",
		L"def calculate_area(length, width):\n"
		L"    return length * width\n"
		L"\n"
		L"return calculate_area(5, 4)\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			return vm.Execute();
		},
		20, {}
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"아이템 사용 조건 체크 테스트",
		L"def OnUse(count0, count1):\n"
		L"    if count0 + Add(count0, count1) < count1 * 14:\n"
		L"        return Add(count0, count1)\n"
		L"    else:\n"
		L"        return Sub(count0 + 100, count1)\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			size_t func = vm.GetFunc(L"OnUse");
			return vm.CallFunc(func, { 1, 2 });
		},
		3, {}
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"중첩된 IF-ELSE 테스트",
		L"if 5 > 3:\n"
		L"    if 2 > 3:\n"
		L"        Print(1)\n"
		L"    else:\n"
		L"        Print(2)\n"
		L"else:\n"
		L"    Print(3)\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			return vm.Execute();
		},
		INVALID_VALUE, { 2 }
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"조건 테스트",
		L"if 3 + Add(1, 2) < 5 + Add(1, 0):\n"
		L"    Print(Add(2, 1))\n"
		L"else:\n"
		L"    Print(Sub(4, 5))\n"
		L"Print(123)\n"
		L"return 1\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			return vm.Execute();
		},
		1, { -1, 123 }
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"계산 함수 테스트",
		L"def Calculate(a, b, c):\n"
		L"    if a * b > c + 10:\n"
		L"        return a * b - c\n"
		L"    else:\n"
		L"        return a + b * c\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			size_t func = vm.GetFunc(L"Calculate");
			return vm.CallFunc(func, { 1, 2, 3 });
		},
		7, {}
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"재귀 함수 테스트 (피보나치)",
		L"def Fibonacci(n):\n"
		L"    if n <= 0:\n"
		L"        return 0\n"
		L"    if n == 1:\n"
		L"        return 1\n"
		L"    return Fibonacci(n - 1) + Fibonacci(n - 2)\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			size_t func = vm.GetFunc(L"Fibonacci");
			return vm.CallFunc(func, { 7 });
		},
		13, {}
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"최대공약수(GCD) 테스트 (재귀)",
		L"def GCD(a, b):\n"
		L"    if b == 0:\n"
		L"        return a\n"
		L"    return GCD(b, Mod(a, b))\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			size_t func = vm.GetFunc(L"GCD");
			return vm.CallFunc(func, { 4, 6 });
		},
		2, {}
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"반환없는함수테스트",
		L"def test1():\n"
		L"    Print(123)\n"
		L"def test2():\n"
		L"    Print(234)\n"
		L"def test3():\n"
		L"    Print(456)\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			size_t test2 = vm.GetFunc(L"test2");
			Value a = vm.CallFunc(test2, { });
			return a;
		},
		INVALID_VALUE, { 234 }
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"홀수 짝수 판별 테스트",
		L"def IsEven(n):\n"
		L"    if n == 0:\n"
		L"        return 1\n"
		L"    return IsOdd(n - 1)\n"
		L"\n"
		L"def IsOdd(n):\n"
		L"    if n == 0:\n"
		L"        return 0\n"
		L"    return IsEven(n - 1)\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			size_t IsEven = vm.GetFunc(L"IsEven");
			size_t IsOdd = vm.GetFunc(L"IsOdd");
			Value a = vm.CallFunc(IsEven, { 5 });
			Value b = vm.CallFunc(IsOdd, { 5 });
			return a + b;
		},
		1, {}
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"그리드 위치 계산 테스트",
		L"def GetGridPosition(index):\n"
		L"    x = Mod(index, GRID_SIZE)\n"
		L"    y = Div(index, GRID_SIZE)\n"
		L"    return Add(Mul(x, 100), Mul(y, 100))\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			size_t GetGridPosition = vm.GetFunc(L"GetGridPosition");
			Value a = vm.CallFunc(GetGridPosition, { 5 });
			return a;
		},
		500, {}
	))
		passCount++;

	testCount++;
	if (RunTest(
		L"중첩된 함수 호출 테스트 (제곱, 세제곱)",
		L"def Square(x):\n"
		L"    return Mul(x, x)\n"
		L"\n"
		L"def Cube(x):\n"
		L"    return Mul(x, Mul(x, x))\n"
		L"\n"
		L"def Calculate(a, b, c):\n"
		L"    if a > b:\n"
		L"        return Sub(Add(Square(a), Cube(b)), c)\n"
		L"    else:\n"
		L"        return Sub(Add(Square(b), Cube(a)), c)\n"
		L"Print(Calculate(1, 2, 3))\n",
		[]([[maybe_unused]] VirtualMachine& vm) -> Value
		{
			return vm.Execute();
		},
		INVALID_VALUE, { 2 }
	))
		passCount++;

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
	std::wcout << L"=== FlowControlScript 테스트 완료 ===" << std::endl;

	return 0;
}


void TestFlowControlScript::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestFlowControlScript::OnStarted()
{
	gGlobalEnvironment = std::make_shared<GlobalEnvironment>();
	RunTests();
}

void TestFlowControlScript::OnUpdate()
{
}
