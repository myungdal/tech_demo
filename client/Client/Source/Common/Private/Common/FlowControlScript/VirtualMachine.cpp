// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VirtualMachine.cpp - 가상 머신 구현
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Common/FlowControlScript/VirtualMachine.h"


namespace FlowControlScript
{
	VirtualMachine::VirtualMachine(const Bytecode& bytecode)
		:
		mBytecode(bytecode)
	{
		mCallStack.emplace_back(CallFrame{ 0, 0, {} });
	}

	Value VirtualMachine::Execute()
	{
		mStack.clear();
		mCallStack.clear();
		mCallStack.emplace_back(CallFrame{ 0, 0, {} });

		// 전역 실행 부분부터 시작 (함수 정의 부분은 건너뜀)
		mIp = mBytecode.mGlobalOffset;

		while (mIp < mBytecode.mCodes.size())
		{
			ExecuteInstruction();
		}

		return mStack.empty() ? INVALID_VALUE : mStack.back();
	}

	size_t VirtualMachine::GetFunc(const std::wstring& name) const
	{
		auto it = mBytecode.mScriptFuncOffsets.find(name);
		if (it == mBytecode.mScriptFuncOffsets.end())
		{
			ReportError(L"Function not found: " + name);
			return INVALID_OFFSET;
		}

		return it->second;
	}

	Value VirtualMachine::CallFunc(size_t offset, const std::deque<Value>& arguments)
	{
		mStack.clear();
		mCallStack.clear();
		mCallStack.emplace_back(CallFrame{ 0, arguments.size(), arguments });

		mIp = offset;

		// 함수 실행
		while (mIp < mBytecode.mCodes.size())
		{
			ExecuteInstruction();
		}

		// 반환값 처리
		Value returnValue = mStack.empty() ? INVALID_VALUE : mStack.back();
		return returnValue;
	}

	void VirtualMachine::ExecuteInstruction()
	{
		Value instruction = ReadValue();
		OpCode opcode = static_cast<OpCode>(instruction);
		
		// 디버그 정보 출력
		//PrintVirtualMachineDebug(opcode);
		
		switch (opcode)
		{
		case OpCode::OP_CONSTANT:
		{
			Value val = ReadValue();
			Push(val);
			break;
		}

		case OpCode::OP_POP:
			Pop();
			break;

		case OpCode::OP_GET_LOCAL:
		{
			Value index = ReadValue();
			if (index >= static_cast<Value>(mCallStack.back().mVariables.size()))
			{
				mCallStack.back().mVariables.resize(index + 1, 0);
			}

			Push(mCallStack.back().mVariables[index]);
			break;
		}

		case OpCode::OP_SET_LOCAL:
		{
			Value index = ReadValue();
			if (index >= static_cast<Value>(mCallStack.back().mVariables.size()))
			{
				mCallStack.back().mVariables.resize(index + 1, 0);
			}

			mCallStack.back().mVariables[index] = Pop();
			break;
		}

		case OpCode::OP_EQUAL:
		{
			Value b = Pop();
			Value a = Pop();
			Push(a == b ? 1 : 0);
			break;
		}

		case OpCode::OP_GREATER:
		{
			Value b = Pop();
			Value a = Pop();
			Push(a > b ? 1 : 0);
			break;
		}

		case OpCode::OP_GREATER_EQUAL:
		{
			Value b = Pop();
			Value a = Pop();
			Push(a >= b ? 1 : 0);
			break;
		}

		case OpCode::OP_LESS:
		{
			Value b = Pop();
			Value a = Pop();
			Push(a < b ? 1 : 0);
			break;
		}

		case OpCode::OP_LESS_EQUAL:
		{
			Value b = Pop();
			Value a = Pop();
			Push(a <= b ? 1 : 0);
			break;
		}

		case OpCode::OP_ADD:
		{
			Value b = Pop();
			Value a = Pop();
			Push(a + b);
			break;
		}

		case OpCode::OP_SUBTRACT:
		{
			Value b = Pop();
			Value a = Pop();
			Push(a - b);
			break;
		}

		case OpCode::OP_MULTIPLY:
		{
			Value b = Pop();
			Value a = Pop();
			Push(a * b);
			break;
		}

		case OpCode::OP_DIVIDE:
		{
			Value b = Pop();
			Value a = Pop();
			if (b == 0) ReportError(L"Division by zero");
			
			Push(a / b);
			break;
		}

		case OpCode::OP_NOT:
			Push(Pop() == 0 ? 1 : 0);
			break;

		case OpCode::OP_NEGATE:
			Push(-Pop());
			break;

		case OpCode::OP_JUMP:
			mIp += ReadOffset();
			break;

		case OpCode::OP_JUMP_IF_FALSE:
		{
			size_t offset = ReadOffset();
			if (Pop() == 0)
			{
				mIp += offset;
			}
			break;
		}

		case OpCode::OP_RETURN:
		{
			// 반환값을 스택에서 꺼냄
			Value returnValue = mStack.empty() ? INVALID_VALUE : Pop();
			
			// 디버그 정보 출력
			//PrintReturnDebug(returnValue);

			if (mCallStack.size() > 1)
			{
				CallFrame frame = mCallStack.back();
				mCallStack.pop_back();

				mIp = frame.mReturnIp;
				
				// 호출자의 변수 상태를 유지
				Push(returnValue);
			}
			else
			{
				mIp = mBytecode.mCodes.size();
				
				// main 스크립트의 return의 경우에도 반환값을 남김
				Push(returnValue);
			}
			break;
		}

		case OpCode::OP_CALL_NATIVE:
		{
			size_t index = static_cast<size_t>(ReadValue());
			size_t argCount = static_cast<size_t>(ReadValue());

			if (index >= mBytecode.mNativeFuncs.size()) 
			{
				ReportError(L"Invalid native function index");
				break;
			}

			// 인자들을 스택에서 Pop하여 뒤집어서 전달
			std::deque<Value> args(argCount);
			for (size_t i = 0; i < argCount; ++i)
				args[argCount - 1 - i] = Pop();

			Value result = mBytecode.mNativeFuncs[index](args);
			Push(result);
			break;
		}

		case OpCode::OP_CALL_SCRIPT:
		{
			size_t functionOffset = static_cast<size_t>(ReadValue());
			size_t argCount = static_cast<size_t>(ReadValue());
			
			// 디버그 정보 출력
			//PrintCallScriptDebug(functionOffset, argCount);
			
			// 현재 실행 상태 저장
			CallFrame frame;
			frame.mReturnIp = mIp;
			frame.mParamCnt = argCount;

			// 인자 값들을 새 CallFrame의 로컬 변수로 저장
			frame.mVariables.resize(argCount);
			for (size_t i = 0; i < argCount; ++i)
				frame.mVariables[argCount - 1 - i] = Pop();

			// 콜 스택에 새 프레임 추가
			mCallStack.emplace_back(std::move(frame));

			// 함수 시작 위치로 점프
			mIp = functionOffset;
			break;
		}

		case OpCode::OP_OR:
		{
			Value b = Pop();
			Value a = Pop();
			Push((a != 0 || b != 0) ? 1 : 0);
			break;
		}

		case OpCode::OP_AND:
		{
			Value b = Pop();
			Value a = Pop();
			Push((a != 0 && b != 0) ? 1 : 0);
			break;
		}

		default:
			ReportError(L"Unknown opcode: " + std::to_wstring(instruction));
		}
	}

	void VirtualMachine::Push(const Value& value)
	{
		mStack.push_back(value);
	}

	Value VirtualMachine::Pop()
	{
		if (mStack.empty())
		{
			std::wstring errorMsg = L"Stack underflow at instruction pointer: "
				+ std::to_wstring(mIp) + L", opcode: "
				+ std::to_wstring(static_cast<int>(static_cast<OpCode>(mBytecode.mCodes[mIp - 1])));
			ReportError(errorMsg);
			return INVALID_VALUE;
		}

		Value value = mStack.back();
		mStack.pop_back();
		return value;
	}

	Value VirtualMachine::Peek() const
	{
		if (mStack.empty())
		{
			ReportError(L"Stack underflow");
			return INVALID_VALUE;
		}

		return mStack.back();
	}

	Value VirtualMachine::ReadValue()
	{
		return mBytecode.mCodes[mIp++];
	}

	Value VirtualMachine::ReadOffset()
	{
		return mBytecode.mCodes[mIp++];
	}

	void VirtualMachine::ReportError(const std::wstring& message) const
	{
		_DEBUG_LOG(RED, L"Error: {}", message);
	}

	void VirtualMachine::PrintVirtualMachineDebug(OpCode opcode) const
	{
		std::wcout << L"\n=== Execution Context ===\n";

		// 기본 실행 정보
		std::wcout << L"Call Stack Depth: " << mCallStack.size() << L"\n";
		std::wcout << L"Current IP: " << mIp - 1 << L"\n";
		std::wcout << L"Executing: " << OpCodeToString(opcode) << L"\n";

		// 콜 스택 정보
		std::wcout << L"\n-- Call Stack: --\n";
		for (size_t i = 0; i < mCallStack.size(); ++i) 
		{
			std::wcout << L"Frame " << i << L":\n";
			std::wcout << L"  Return IP: " << mCallStack[i].mReturnIp << L"\n";
			std::wcout << L"  Parameters: " << mCallStack[i].mParamCnt << L"\n";
			std::wcout << L"  Variables: [";
			
			for (size_t j = 0; j < mCallStack[i].mVariables.size(); ++j) 
			{
				if (j > 0) std::wcout << L", ";
				std::wcout << L"var[" << j << L"]=" << mCallStack[i].mVariables[j];
			}
			std::wcout << L"]\n";
		}

		// 스택 상태
		std::wcout << L"\n-- Stack State: --\n";
		std::wcout << L"Before execution: [";
		
		for (size_t i = 0; i < mStack.size(); ++i) 
		{
			if (i > 0) std::wcout << L", ";
			std::wcout << mStack[i];
		}
		std::wcout << L"]\n";

		std::wcout << L"=====================\n\n";
	}

	void VirtualMachine::PrintReturnDebug(Value returnValue) const
	{
		std::wcout << L"\n-- Return Information --\n";
		std::wcout << L"Current call depth: " << mCallStack.size() << L"\n";
		std::wcout << L"Return IP: " << (mCallStack.size() > 1 ? mCallStack.back().mReturnIp : -1) << L"\n";
		std::wcout << L"Return value: " << (returnValue == INVALID_VALUE ? L"<none>" : std::to_wstring(returnValue)) << L"\n";
		std::wcout << L"=====================\n\n";
	}

	void VirtualMachine::PrintCallScriptDebug(size_t functionOffset, size_t argCount) const
	{
		// 함수 이름 찾기
		std::wstring funcName = L"<unknown>";
		for (const auto& [name, offset] : mBytecode.mScriptFuncOffsets) 
		{
			if (offset == functionOffset) 
			{
				funcName = name;
				break;
			}
		}
		
		std::wcout << L"\n-- Script Function Call --\n";
		std::wcout << L"Function name: '" << funcName << L"'\n";
		std::wcout << L"Offset: " << functionOffset << L"\n";
		std::wcout << L"Arguments count: " << argCount << L"\n";
		
		std::wcout << L"\nCall stack trace:\n";
		for (size_t i = 0; i < mCallStack.size(); ++i) 
		{
			std::wcout << L"  " << i << L": IP=" << mCallStack[i].mReturnIp 
					  << L", Vars=" << mCallStack[i].mVariables.size() << L"\n";
		}
		std::wcout << L"=====================\n\n";
	}
}
