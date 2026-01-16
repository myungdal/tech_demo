// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compiler.cpp - 컴파일러 구현
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Common/FlowControlScript/Compiler.h"

#include "Common/FlowControlScript/Types.h"
#include "Common/FlowControlScript/GlobalEnvironment.h"


namespace FlowControlScript
{
	Compiler::Compiler(OUT Bytecode& bytecode)
		:
		mBytecode(bytecode)
	{
	}

	void Compiler::Compile(const std::unique_ptr<Node_root>& root)
	{
		GenerateRoot(root.get());

		// 바이트코드 디버그 정보 출력
		//PrintBytecodeDebug();
	}

	void Compiler::GenerateRoot(const Node_root* root)
	{
		// 1단계: 함수별 로컬 변수(매개변수) 초기화
		for (const auto& statement : root->GetStatements())
		{
			if (statement->GetType() == NodeType::NT_FUNC)
			{
				const Node_function* function = static_cast<const Node_function*>(statement.get());

				mScriptFuncVarOffsets[function->GetName()] = std::unordered_map<std::wstring, size_t>();
				auto& functionVars = mScriptFuncVarOffsets[function->GetName()];

				for (const auto& param : function->GetParameters())
				{
					if (functionVars.count(param->GetName()))
					{
						ReportError(L"Duplicate parameter in function " + function->GetName());
						return;
					}

					functionVars[param->GetName()] = functionVars.size();
				}
			}
		}

		// 2단계: 모든 함수의 본문 생성
		for (const auto& statement : root->GetStatements())
		{
			if (statement->GetType() == NodeType::NT_FUNC)
			{
				const Node_function* function = static_cast<const Node_function*>(statement.get());
			
				// 스크립트함수 오프셋 셋팅
				mBytecode.mScriptFuncOffsets[function->GetName()] = mCurrentOffset;

				GenerateFunction(function);
			}
		}

		// 3단계: 전역 코드 시작점 기록
		mBytecode.mGlobalOffset = mCurrentOffset;  // 전역 코드 시작점 기록

		// 4단계: 전역 실행문 생성
		for (const auto& statement : root->GetStatements())
		{
			if (statement->GetType() != NodeType::NT_FUNC)
			{
				GenerateStatement(statement.get());
			}
		}

		// 5단계: mCallScriptFuncOffsets 스크립트 함수 오프셋 처리
		for (auto& [offset, scriptFuncName] : mCallScriptFuncOffsets)
		{
			mBytecode.mCodes[offset] = mBytecode.mScriptFuncOffsets[scriptFuncName];
		}
	}

	void Compiler::GenerateStatement(const Node_statement* statement)
	{
		switch (statement->GetType())
		{
		case NodeType::NT_FUNC:
			GenerateFunction(static_cast<const Node_function*>(statement));
			break;

		case NodeType::NT_IF_STATEMENT:
			GenerateIfStatement(static_cast<const Node_ifStatement*>(statement));
			break;

		case NodeType::NT_RETURN_STATEMENT:
			GenerateReturnStatement(static_cast<const Node_returnStatement*>(statement));
			break;

		case NodeType::NT_EXPRESSION_STATEMENT:
			GenerateExpressionStatement(static_cast<const Node_expressionStatement*>(statement));
			break;

		default:
			ReportError(L"Unknown statement type");
		}
	}

	void Compiler::GenerateFunction(const Node_function* function)
	{
		mCurrScriptFuncName = function->GetName();
		mCurrScriptFuncVars = mScriptFuncVarOffsets[function->GetName()];

		for (const auto& statement : function->GetBody())
		{
			GenerateStatement(statement.get());
		}

		// 마지막 명령어가 OP_RETURN이 아닌 경우에만 추가
		if (mBytecode.mCodes.empty() ||
			static_cast<OpCode>(mBytecode.mCodes.back()) != OpCode::OP_RETURN)
		{
			EmitOpCode(OpCode::OP_RETURN);
		}

		mCurrScriptFuncName.clear();
		mCurrScriptFuncVars.clear();
	}

	void Compiler::GenerateIfStatement(const Node_ifStatement* ifStatement)
	{
		std::deque<size_t> jumpsToEnd;

		GenerateExpression(ifStatement->GetCondition());
		size_t thenJump = EmitJump(OpCode::OP_JUMP_IF_FALSE);

		for (const auto& statement : ifStatement->GetThenBody())
		{
			GenerateStatement(statement.get());
		}

		size_t thenEnd = EmitJump(OpCode::OP_JUMP);
		jumpsToEnd.push_back(thenEnd);
		PatchJump(thenJump);

		for (const auto& [condition, body] : ifStatement->GetElifBodies())
		{
			GenerateExpression(condition.get());
			size_t elifJump = EmitJump(OpCode::OP_JUMP_IF_FALSE);

			for (const auto& statement : body)
			{
				GenerateStatement(statement.get());
			}

			size_t elifEnd = EmitJump(OpCode::OP_JUMP);
			jumpsToEnd.push_back(elifEnd);
			PatchJump(elifJump);
		}

		if (!ifStatement->GetElseBody().empty())
		{
			for (const auto& statement : ifStatement->GetElseBody())
			{
				GenerateStatement(statement.get());
			}
		}

		for (size_t jump : jumpsToEnd)
		{
			PatchJump(jump);
		}
	}

	void Compiler::GenerateReturnStatement(const Node_returnStatement* returnStatement)
	{
		// 먼저 반환할 식의 코드를 생성합니다.
		GenerateExpression(returnStatement->GetExpression());
		// 그 후, 현재 함수(또는 전역 스크립트)를 종료하는 OP_RETURN을 emit합니다.
		EmitOpCode(OpCode::OP_RETURN);
	}

	void Compiler::GenerateExpressionStatement(const Node_expressionStatement* statement)
	{
		const Node_expression* expr = statement->GetExpression();
		GenerateExpression(expr);

		// 표현식문인 경우 OP_POP을 실행하여 스택에서 값을 꺼냄
		// 표현식문이 OP_SET_LOCAL이면 값을 꺼냄
		if (expr->GetType() != NodeType::NT_ASSIGNMENT) 
		{
			EmitOpCode(OpCode::OP_POP);
		}
	}

	void Compiler::GenerateExpression(const Node_expression* expression)
	{
		switch (expression->GetType())
		{
		case NodeType::NT_VARIABLE:
			GenerateVariable(static_cast<const Node_variable*>(expression));
			break;

		case NodeType::NT_LITERAL:
			GenerateLiteral(static_cast<const Node_literal*>(expression));
			break;

		case NodeType::NT_GROUPING:
			GenerateGrouping(static_cast<const Node_grouping*>(expression));
			break;

		case NodeType::NT_ASSIGNMENT:
			GenerateAssignment(static_cast<const Node_assignment*>(expression));
			break;

		case NodeType::NT_CALL:
			GenerateCall(static_cast<const Node_call*>(expression));
			break;

		case NodeType::NT_BINARY_OPERATION:
			GenerateBinary(static_cast<const Node_binaryOperation*>(expression));
			break;

		case NodeType::NT_UNARY_OPERATION:
			GenerateUnary(static_cast<const Node_unaryOperation*>(expression));
			break;

		default:
			ReportError(L"Unknown expression type");
		}
	}

	void Compiler::GenerateBinary(const Node_binaryOperation* binary)
	{
		GenerateExpression(binary->GetLeft());
		GenerateExpression(binary->GetRight());

		switch (binary->GetOperator())
		{
		case TokenType::TK_PLUS:
			EmitOpCode(OpCode::OP_ADD);
			break;

		case TokenType::TK_MINUS:
			EmitOpCode(OpCode::OP_SUBTRACT);
			break;

		case TokenType::TK_MULTIPLY:
			EmitOpCode(OpCode::OP_MULTIPLY);
			break;

		case TokenType::TK_DIVISION:
			EmitOpCode(OpCode::OP_DIVIDE);
			break;

		case TokenType::TK_EQUAL:
			EmitOpCode(OpCode::OP_EQUAL);
			break;

		case TokenType::TK_NOT_EQUAL:
			EmitOpCode(OpCode::OP_EQUAL);
			EmitOpCode(OpCode::OP_NOT);
			break;

		case TokenType::TK_GREATER:
			EmitOpCode(OpCode::OP_GREATER);
			break;

		case TokenType::TK_GREATER_EQUAL:
			EmitOpCode(OpCode::OP_GREATER_EQUAL);
			break;

		case TokenType::TK_LESS:
			EmitOpCode(OpCode::OP_LESS);
			break;

		case TokenType::TK_LESS_EQUAL:
			EmitOpCode(OpCode::OP_LESS_EQUAL);
			break;

		case TokenType::TK_OR:
			EmitOpCode(OpCode::OP_OR);
			break;

		case TokenType::TK_AND:
			EmitOpCode(OpCode::OP_AND);
			break;

		default:
			ReportError(L"Unknown binary operator");
		}
	}

	void Compiler::GenerateUnary(const Node_unaryOperation* unary)
	{
		GenerateExpression(unary->GetOperand());

		switch (unary->GetOperator())
		{
		case TokenType::TK_MINUS:
			EmitOpCode(OpCode::OP_NEGATE);
			break;

		case TokenType::TK_NOT:
			EmitOpCode(OpCode::OP_NOT);
			break;

		case TokenType::TK_PLUS:
			// 아무것도 하지 않아도 됨 (양수 값이 그대로 유지되므로)
			break;

		default:
			ReportError(L"Unknown unary operator");
		}
	}

	void Compiler::GenerateVariable(const Node_variable* variable)
	{
		if (gGlobalEnvironment->HasConstant(variable->GetName()))
		{
			const Value value = gGlobalEnvironment->FindConstant(variable->GetName());
			EmitOpCode(OpCode::OP_CONSTANT);
			EmitValue(value);
		}
		else
		{
			if (mCurrScriptFuncVars.count(variable->GetName()) == 0)
				mCurrScriptFuncVars[variable->GetName()] = mCurrScriptFuncVars.size();

			const size_t offset = mCurrScriptFuncVars[variable->GetName()];

			EmitOpCode(OpCode::OP_GET_LOCAL);
			EmitValue(offset);
		}
	}

	void Compiler::GenerateLiteral(const Node_literal* literal)
	{
		EmitOpCode(OpCode::OP_CONSTANT);
		EmitValue(literal->GetValue());
	}

	void Compiler::GenerateGrouping(const Node_grouping* grouping)
	{
		GenerateExpression(grouping->GetExpression());
	}

	void Compiler::GenerateAssignment(const Node_assignment* assignment)
	{
		GenerateExpression(assignment->GetValue());

		if (mCurrScriptFuncVars.count(assignment->GetName()) == 0)
			mCurrScriptFuncVars[assignment->GetName()] = mCurrScriptFuncVars.size();

		const size_t offset = mCurrScriptFuncVars[assignment->GetName()];

		EmitOpCode(OpCode::OP_SET_LOCAL);
		EmitValue(offset);
	}

	void Compiler::GenerateCall(const Node_call* call)
	{
		// 먼저 인자들을 생성
		for (const auto& arg : call->GetArguments())
		{
			GenerateExpression(arg.get());
		}

		// Native function call
		auto nativeIt = mNativeFuncOffsets.find(call->GetCallee());
		if (nativeIt != mNativeFuncOffsets.end())
		{
			EmitOpCode(OpCode::OP_CALL_NATIVE);
			EmitValue(nativeIt->second);
			EmitValue(call->GetArguments().size());
			return;
		}

		// Script function call
		EmitOpCode(OpCode::OP_CALL_SCRIPT);

		// 아직 함수를 찾을 수 없기 때문에 mCallScriptFuncOffsets 에 기록해 두고...
		mCallScriptFuncOffsets.emplace_back(mCurrentOffset, call->GetCallee());
		// 아직 함수를 찾을 수 없기 때문에 빈 값을 셋팅
		EmitValue(0);

		EmitValue(call->GetArguments().size());
	}

	void Compiler::EmitOpCode(OpCode op)
	{
		EmitValue(static_cast<Value>(op));
	}

	void Compiler::EmitValue(Value value)
	{
		mBytecode.mCodes.push_back(value);
		mCurrentOffset++;
	}

	size_t Compiler::EmitJump(OpCode instruction)
	{
		EmitOpCode(instruction);
		EmitValue(0xFF);
		return mCurrentOffset - 1;
	}

	void Compiler::PatchJump(size_t offset)
	{
		size_t jump = mCurrentOffset - offset - 1;
		mBytecode.mCodes[offset] = jump;
	}

	void Compiler::ReportError([[maybe_unused]] const std::wstring& message)
	{
		throw std::runtime_error("Bytecode generation error");
	}

	void Compiler::PrintBytecodeDebug()
	{
		std::wcout << L"\n*** Bytecode Dump ***\n";
		std::wcout << L"Size: " << mBytecode.mCodes.size() << L" bytes\n";

		// 전역 상수들을 출력 (추가)
		std::wcout << L"\n-- Global Constants: --\n";
		if (gGlobalEnvironment) 
		{
			for (const auto& pair : gGlobalEnvironment->GetAllConstants()) 
			{
				std::wcout << L"'" << pair.first << L"' = " << pair.second << L"\n";
			}
		}
		else 
		{
			std::wcout << L"(Global environment not initialized)\n";
		}

		// 스크립트 함수들을 출력
		std::wcout << L"\n-- Script Functions: --\n";
		for (const auto& [name, offset] : mBytecode.mScriptFuncOffsets) 
		{
			std::wcout << L"Function '" << name << L"' at offset: " << offset << L"\n";
		}

		// 네이티브 함수들을 출력
		std::wcout << L"\n-- Native Functions: --\n";
		for (const auto& [name, offset] : mNativeFuncOffsets) 
		{
			std::wcout << L"Native function '" << name << L"' at index: " << offset << L"\n";
		}

		// 변수들을 출력
		std::wcout << L"\n-- Variables: --\n";
		// 함수별로 변수들을 출력
		for (const auto& [funcName, varMap] : mScriptFuncVarOffsets) 
		{
			std::wcout << L"Function '" << funcName << L"' variables:\n";
			
			// 각 함수의 변수들을 오프셋 기준으로 정렬
			std::map<size_t, std::wstring> sortedVars;
			for (const auto& [varName, varOffset] : varMap) 
			{
				sortedVars[varOffset] = varName;
			}
			
			// 정렬된 변수들을 출력
			for (const auto& [offset, name] : sortedVars) 
			{
				std::wcout << L"  [" << offset << L"] '" << name << L"'\n";
			}
		}

		// 바이트코드 명령들을 출력
		std::wcout << L"\n-- Bytecode: --\n";
		for (size_t i = 0; i < mBytecode.mCodes.size(); i++) 
		{
			std::wcout << std::setw(4) << i << L": ";

			// OpCode를 출력
			if (i < mBytecode.mCodes.size()) 
			{
				OpCode op = static_cast<OpCode>(mBytecode.mCodes[i]);
				std::wcout << OpCodeToString(op);

				// 명령어 추가 정보 출력
				switch (op) 
				{
				case OpCode::OP_CONSTANT:
					if (i + 1 < mBytecode.mCodes.size())
						std::wcout << L" " << mBytecode.mCodes[i + 1];
					i++;
					break;

				case OpCode::OP_GET_LOCAL:
				case OpCode::OP_SET_LOCAL:
					if (i + 1 < mBytecode.mCodes.size()) 
					{
						size_t varIndex = static_cast<size_t>(mBytecode.mCodes[i + 1]);
						std::wcout << L" (index: " << varIndex << L")";
					}
					i++;
					break;

				case OpCode::OP_JUMP:
				case OpCode::OP_JUMP_IF_FALSE:
					if (i + 1 < mBytecode.mCodes.size()) 
					{
						size_t jumpOffset = static_cast<size_t>(mBytecode.mCodes[i + 1]);
						std::wcout << L" (offset: " << jumpOffset << L", target: "
							<< (i + 1 + jumpOffset) << L")";
					}
					i++;
					break;

				case OpCode::OP_CALL_SCRIPT:
					if (i + 2 < mBytecode.mCodes.size()) 
					{
						size_t funcOffset = static_cast<size_t>(mBytecode.mCodes[i + 1]);
						std::wcout << L" (offset: " << funcOffset << L", args: "
							<< mBytecode.mCodes[i + 2] << L")";

						// 함수를 찾아서 출력
						for (const auto& [name, offset] : mBytecode.mScriptFuncOffsets) 
						{
							if (offset == funcOffset) 
							{
								std::wcout << L" '" << name << L"'";
								break;
							}
						}
					}
					i += 2;
					break;

				case OpCode::OP_CALL_NATIVE:
					if (i + 2 < mBytecode.mCodes.size()) 
					{
						size_t funcIndex = static_cast<size_t>(mBytecode.mCodes[i + 1]);
						std::wcout << L" (index: " << funcIndex << L", args: "
							<< mBytecode.mCodes[i + 2] << L")";

						// 함수를 찾아서 출력
						for (const auto& [name, index] : mNativeFuncOffsets) 
						{
							if (index == funcIndex) 
							{
								std::wcout << L" '" << name << L"'";
								break;
							}
						}
					}
					i += 2;
					break;
				}
			}
			std::wcout << L"\n";
		}
		std::wcout << L"\n";
	}
}
