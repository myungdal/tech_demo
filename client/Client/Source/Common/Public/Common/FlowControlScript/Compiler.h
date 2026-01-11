// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/FlowControlScript/Types.h"


namespace FlowControlScript
{
	/**
	 * Compiler (컴파일러)
	 * 
	 * Parser가 생성한 AST(Abstract Syntax Tree, 추상 구문 트리)를 바이트코드로 컴파일합니다.
	 * 
	 * 주요 기능:
	 *  - AST 노드를 OpCode 바이트코드로 변환
	 *  - 변수 오프셋 관리 (지역 변수 스택 위치)
	 *  - 함수 오프셋 관리 (함수 진입점)
	 *  - C++ 네이티브 함수 바인딩
	 *  - 점프 명령어 패치 (조건문, 반복문)
	 * 
	 * 생성되는 바이트코드 타입:
	 *  - OP_CONSTANT: 상수 로드
	 *  - OP_GET_LOCAL/OP_SET_LOCAL: 지역 변수 접근
	 *  - OP_ADD/OP_SUBTRACT/OP_MULTIPLY/OP_DIVIDE: 산술 연산
	 *  - OP_EQUAL/OP_GREATER/OP_LESS: 비교 연산
	 *  - OP_JUMP/OP_JUMP_IF_FALSE: 제어 흐름
	 *  - OP_CALL_SCRIPT/OP_CALL_NATIVE: 함수 호출
	 *  - OP_RETURN: 함수 반환
	 * 
	 * C++ 네이티브 함수 바인딩:
	 *  - BindNativeFunction()을 통해 C++ 멤버 함수를 스크립트에서 호출 가능
	 *  - 가변 인자 템플릿을 사용한 타입 안전 바인딩
	 *  - 반환값이 있는 함수와 void 함수 모두 지원
	 */
	class Compiler
	{
	private:
		OUT Bytecode& mBytecode;
		size_t mCurrentOffset = 0;
		std::unordered_map<std::wstring, size_t> mNativeFuncOffsets;

		std::unordered_map<std::wstring, std::unordered_map<std::wstring, size_t>> mScriptFuncVarOffsets;  // 함수별 변수 오프셋
		std::unordered_map<std::wstring, size_t> mCurrScriptFuncVars;  // 현재 함수의 변수 오프셋
		std::wstring mCurrScriptFuncName;  // 현재 처리 중인 함수 이름

		std::deque<std::pair<size_t, std::wstring>> mCallScriptFuncOffsets;

	public:
		explicit Compiler(OUT Bytecode& bytecode);
		virtual ~Compiler() = default;

	public:
		void Compile(const std::unique_ptr<Node_root>& root);

	public:
		template<typename T, typename... Args>
		void BindNativeFunction(const std::wstring& name, T& instance, Value(T::* method)(Args...))
		{
			auto it = mNativeFuncOffsets.find(name);
			if (it != mNativeFuncOffsets.end())
				return;

			mNativeFuncOffsets[name] = mNativeFuncOffsets.size();

			auto&& func = [&instance, method](const std::deque<Value>& args) -> Value
				{
					if (args.size() < sizeof...(Args))
					{
						std::wcerr << L"Arg count error" << std::endl;
						return 0;
					}
					return CallNativeFunction(instance, method, args, std::index_sequence_for<Args...>{});
				};

			mBytecode.mNativeFuncs.emplace_back(std::move(func));
		}

		template<typename T, typename... Args>
		void BindNativeFunction(const std::wstring& name, T& instance, void(T::* method)(Args...))
		{
			auto it = mNativeFuncOffsets.find(name);
			if (it != mNativeFuncOffsets.end())
				return;

			mNativeFuncOffsets[name] = mNativeFuncOffsets.size();

			auto&& func = [&instance, method](const std::deque<Value>& args) -> Value
				{
					if (args.size() < sizeof...(Args))
					{
						std::wcerr << L"Arg count error" << std::endl;
						return 0;
					}
					CallNativeFunctionVoid(instance, method, args, std::index_sequence_for<Args...>{});
					return 0;
				};

			mBytecode.mNativeFuncs.emplace_back(std::move(func));
		}

	private:
		// 네이티브 함수를 호출하는 템플릿 함수
		template<typename T, typename... Args, size_t... I>
		static Value CallNativeFunction(T& instance, Value(T::* method)(Args...), const std::deque<Value>& args, std::index_sequence<I...>)
		{
			return (instance.*method)(args[I]...);
		}

		template<typename T, typename... Args, size_t... I>
		static Value CallNativeFunctionVoid(T& instance, void(T::* method)(Args...), const std::deque<Value>& args, std::index_sequence<I...>)
		{
			(instance.*method)(args[I]...);
			return 0;
		}

	private:
		void GenerateRoot(const Node_root* root);
		void GenerateStatement(const Node_statement* statement);
		void GenerateFunction(const Node_function* function);
		void GenerateIfStatement(const Node_ifStatement* ifStatement);
		void GenerateReturnStatement(const Node_returnStatement* statement);
		void GenerateExpressionStatement(const Node_expressionStatement* statement);
		void GenerateExpression(const Node_expression* expression);
		void GenerateBinary(const Node_binaryOperation* binary);
		void GenerateUnary(const Node_unaryOperation* unary);
		void GenerateVariable(const Node_variable* variable);
		void GenerateLiteral(const Node_literal* literal);
		void GenerateGrouping(const Node_grouping* grouping);
		void GenerateAssignment(const Node_assignment* assignment);
		void GenerateCall(const Node_call* call);

		void EmitOpCode(OpCode op);
		void EmitValue(Value value);
		size_t EmitJump(OpCode instruction);
		void PatchJump(size_t offset);

	private:
		void ReportError(const std::wstring& message);
		void PrintBytecodeDebug();
	};
}
