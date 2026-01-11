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
	 * VirtualMachine (가상 머신)
	 * 
	 * 컴파일된 바이트코드를 실행하는 스택 기반 VM(Virtual Machine, 가상 머신)입니다.
	 * 
	 * 주요 기능:
	 *  - 바이트코드 명령어 실행
	 *  - 스택 기반 연산 (값 스택)
	 *  - 호출 스택 관리 (CallFrame)
	 *  - 함수 호출 및 반환 처리
	 *  - 네이티브 함수 호출
	 * 
	 * 실행 모델:
	 *  - 스택 기반 아키텍처 (Stack-based VM)
	 *  - Instruction Pointer (IP)를 통한 순차 실행
	 *  - 점프 명령어로 제어 흐름 변경
	 *  - 호출 스택을 통한 함수 호출 및 지역 변수 관리
	 * 
	 * 사용 방법:
	 *  1. Execute(): 스크립트 전체 실행 (최상위 레벨)
	 *  2. GetFunc(): 함수 이름으로 함수 오프셋 조회
	 *  3. CallFunc(): 특정 함수를 인자와 함께 호출
	 * 
	 * 예시:
	 *  VirtualMachine vm(bytecode);
	 *  size_t funcOffset = vm.GetFunc(L"Calculate");
	 *  Value result = vm.CallFunc(funcOffset, {10, 20});
	 */
	class VirtualMachine
	{
	private:
		std::deque<Value> mStack;
		std::deque<CallFrame> mCallStack;
		size_t mIp = 0;
		const Bytecode& mBytecode;

	public:
		explicit VirtualMachine(const Bytecode& bytecode);
		virtual ~VirtualMachine() = default;

	public:
		Value Execute();
		size_t GetFunc(const std::wstring& name) const;
		Value CallFunc(size_t offset, const std::deque<Value>& args);

	private:
		void ExecuteInstruction();
		void Push(const Value& value);
		Value Pop();
		Value ReadValue();
		Value ReadOffset();
		Value Peek() const;

	private:
		void ReportError(const std::wstring& message) const;
		void PrintVirtualMachineDebug(OpCode opcode) const;
		void PrintReturnDebug(Value returnValue) const;
		void PrintCallScriptDebug(size_t functionOffset, size_t argCount) const;
	};
}
