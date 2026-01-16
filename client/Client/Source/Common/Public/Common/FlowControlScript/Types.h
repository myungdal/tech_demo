// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

/**
 * FlowControlScript - Python-like Embedded Scripting DSL
 * 
 * FlowControlScript는 두 가지 주요 특성을 가진 DSL(Domain-Specific Language, 도메인 특화 언어)입니다:
 * 
 * 1. Python-like Scripting DSL (파이썬 스타일의 스크립트형 DSL, Domain-Specific Language)
 *    - Python과 유사한 문법 (def, if/else, return 키워드)
 *    - 들여쓰기 기반 블록 구조
 *    - 재귀 함수 지원
 *    - 직관적이고 배우기 쉬운 문법
 * 
 * 2. Embedded Flow-Control DSL (임베디드 흐름 제어 DSL, Domain-Specific Language)
 *    - C++ 게임 엔진에 임베디드
 *    - 스택 기반 가상 머신(VM) 실행
 *    - C++ 네이티브 함수 바인딩 지원
 *    - 게임 로직의 흐름 제어에 특화
 * 
 * 주요 구성 요소:
 *    - Lexer: 소스 코드를 토큰으로 변환
 *    - Parser: 토큰을 AST(추상 구문 트리)로 변환
 *    - Compiler: AST를 바이트코드로 컴파일
 *    - VirtualMachine: 바이트코드를 실행
 * 
 * 사용 예시:
 *    def OnUse(count0, count1):
 *        if count0 + Add(count0, count1) < count1 * 14:
 *            return Add(count0, count1)
 *        else:
 *            return Sub(count0 + 100, count1)
 */

namespace FlowControlScript
{
	class Node;
	class Node_statement;
	class Node_expression;
	class Node_root;
	class Node_function;
	class Node_ifStatement;
	class Node_returnStatement;
	class Node_binaryOperation;
	class Node_unaryOperation;
	class Node_variable;
	class Node_literal;
	class Node_expressionStatement;
	class Node_grouping;
	class Node_assignment;
	class Node_call;

	// FlowControlScript의 기본 값 타입 (64비트 정수)
	// 모든 스크립트 변수와 함수 반환값은 이 타입을 사용
	using Value = int64_t;
	inline constexpr Value INVALID_VALUE = INT64_MAX;

	inline constexpr size_t INVALID_OFFSET = SIZE_MAX;

	/**
	 * TokenType - Lexer가 인식하는 토큰 타입
	 * 
	 * 소스 코드를 구성하는 기본 단위들을 정의합니다.
	 * Lexer는 문자열을 이 토큰들의 시퀀스로 변환합니다.
	 */
	enum class TokenType : uint8_t
	{
		TK_LEFT_PAREN,         // Left parenthesis '('
		TK_RIGHT_PAREN,        // Right parenthesis ')'
		TK_MINUS,              // Minus '-'
		TK_PLUS,               // Plus '+'
		TK_DIVISION,           // Division '/'
		TK_MULTIPLY,           // Multiply '*'
		TK_NOT,                // Not '!'
		TK_NOT_EQUAL,          // Not equal '!='
		TK_ASSIGN,             // Assignment '='
		TK_EQUAL,              // Equal '=='
		TK_GREATER,            // Greater than '>'
		TK_GREATER_EQUAL,      // Greater than or equal '>='
		TK_LESS,               // Less than '<'
		TK_LESS_EQUAL,         // Less than or equal '<='
		TK_IDENTIFIER,         // Identifier
		TK_STRING,             // String
		TK_NUMBER,             // Number
		TK_DEF,                // Function keyword
		TK_IF,                 // If keyword
		TK_ELSE,               // Else keyword
		TK_RETURN,             // Return keyword
		TK_AND,                // Logical AND '&&'
		TK_OR,                 // Logical OR '||'
		TK_EOF                 // End of file
	};

	inline std::wstring TokenTypeToString(TokenType type)
	{
		switch (type)
		{
			case TokenType::TK_LEFT_PAREN: return L"TK_LEFT_PAREN";
			case TokenType::TK_RIGHT_PAREN: return L"TK_RIGHT_PAREN";
			case TokenType::TK_MINUS: return L"TK_MINUS";
			case TokenType::TK_PLUS: return L"TK_PLUS";
			case TokenType::TK_DIVISION: return L"TK_DIVISION";
			case TokenType::TK_MULTIPLY: return L"TK_MULTIPLY";
			case TokenType::TK_NOT: return L"TK_NOT";
			case TokenType::TK_NOT_EQUAL: return L"TK_NOT_EQUAL";
			case TokenType::TK_ASSIGN: return L"TK_ASSIGN";
			case TokenType::TK_EQUAL: return L"TK_EQUAL";
			case TokenType::TK_GREATER: return L"TK_GREATER";
			case TokenType::TK_GREATER_EQUAL: return L"TK_GREATER_EQUAL";
			case TokenType::TK_LESS: return L"TK_LESS";
			case TokenType::TK_LESS_EQUAL: return L"TK_LESS_EQUAL";
			case TokenType::TK_IDENTIFIER: return L"TK_IDENTIFIER";
			case TokenType::TK_STRING: return L"TK_STRING";
			case TokenType::TK_NUMBER: return L"TK_NUMBER";
			case TokenType::TK_DEF: return L"TK_DEF";
			case TokenType::TK_IF: return L"TK_IF";
			case TokenType::TK_ELSE: return L"TK_ELSE";
			case TokenType::TK_RETURN: return L"TK_RETURN";
			case TokenType::TK_AND: return L"TK_AND";
			case TokenType::TK_OR: return L"TK_OR";
			case TokenType::TK_EOF: return L"TK_EOF";
			default: return L"UNKNOWN";
		}
	}

	/**
	 * NodeType - AST 노드 타입
	 * 
	 * Parser가 생성하는 추상 구문 트리(AST)의 노드 종류를 정의합니다.
	 * 각 노드는 스크립트의 문법적 구조를 나타냅니다.
	 */
	enum class NodeType : uint8_t
	{
		NT_ROOT,                  // Root node
		NT_FUNC,                  // Function node
		NT_IF_STATEMENT,          // If statement node
		NT_RETURN_STATEMENT,      // Return statement node
		NT_EXPRESSION_STATEMENT,  // Expression statement node
		NT_BINARY_OPERATION,      // Binary operation node
		NT_UNARY_OPERATION,       // Unary operation node
		NT_VARIABLE,              // Variable node
		NT_LITERAL,               // Literal node
		NT_GROUPING,              // Grouping node
		NT_ASSIGNMENT,            // Assignment node
		NT_CALL                   // Function call node
	};

	/**
	 * OpCode - 가상 머신 명령어 코드
	 * 
	 * Compiler가 생성하고 VirtualMachine이 실행하는 바이트코드 명령어입니다.
	 * 스택 기반 VM 아키텍처를 따릅니다.
	 * 
	 * 명령어 카테고리:
	 *  - 스택 관리: OP_CONSTANT, OP_POP
	 *  - 변수 접근: OP_GET_LOCAL, OP_SET_LOCAL
	 *  - 산술 연산: OP_ADD, OP_SUBTRACT, OP_MULTIPLY, OP_DIVIDE
	 *  - 비교 연산: OP_EQUAL, OP_GREATER, OP_LESS, OP_GREATER_EQUAL, OP_LESS_EQUAL
	 *  - 논리 연산: OP_NOT, OP_AND, OP_OR
	 *  - 제어 흐름: OP_JUMP, OP_JUMP_IF_FALSE, OP_RETURN
	 *  - 함수 호출: OP_CALL_SCRIPT, OP_CALL_NATIVE
	 */
	enum class OpCode : uint8_t
	{
		OP_CONSTANT,        // Load constant
		OP_POP,             // Pop stack
		OP_GET_LOCAL,       // Load local variable
		OP_SET_LOCAL,       // Store local variable
		OP_EQUAL,           // Equal comparison
		OP_GREATER,         // Greater than comparison
		OP_LESS,            // Less than comparison
		OP_GREATER_EQUAL,   // Greater than or equal comparison
		OP_LESS_EQUAL,      // Less than or equal comparison
		OP_ADD,             // Addition
		OP_SUBTRACT,        // Subtraction
		OP_MULTIPLY,        // Multiplication
		OP_DIVIDE,          // Division
		OP_NOT,             // Logical NOT
		OP_NEGATE,          // Unary negation
		OP_JUMP,            // Unconditional jump
		OP_JUMP_IF_FALSE,   // Conditional jump if false
		OP_RETURN,          // Function return
		OP_CALL_SCRIPT,     // Call script function
		OP_CALL_NATIVE,     // Call native function
		OP_OR,              // Logical OR
		OP_AND              // Logical AND
	};

	inline std::wstring OpCodeToString(OpCode op)
	{
		switch (op) {
			case OpCode::OP_CONSTANT: return L"OP_CONSTANT";
			case OpCode::OP_POP: return L"OP_POP";
			case OpCode::OP_GET_LOCAL: return L"OP_GET_LOCAL";
			case OpCode::OP_SET_LOCAL: return L"OP_SET_LOCAL";
			case OpCode::OP_EQUAL: return L"OP_EQUAL";
			case OpCode::OP_GREATER: return L"OP_GREATER";
			case OpCode::OP_LESS: return L"OP_LESS";
			case OpCode::OP_GREATER_EQUAL: return L"OP_GREATER_EQUAL";
			case OpCode::OP_LESS_EQUAL: return L"OP_LESS_EQUAL";
			case OpCode::OP_ADD: return L"OP_ADD";
			case OpCode::OP_SUBTRACT: return L"OP_SUBTRACT";
			case OpCode::OP_MULTIPLY: return L"OP_MULTIPLY";
			case OpCode::OP_DIVIDE: return L"OP_DIVIDE";
			case OpCode::OP_NOT: return L"OP_NOT";
			case OpCode::OP_NEGATE: return L"OP_NEGATE";
			case OpCode::OP_JUMP: return L"OP_JUMP";
			case OpCode::OP_JUMP_IF_FALSE: return L"OP_JUMP_IF_FALSE";
			case OpCode::OP_RETURN: return L"OP_RETURN";
			case OpCode::OP_CALL_SCRIPT: return L"OP_CALL_SCRIPT";
			case OpCode::OP_CALL_NATIVE: return L"OP_CALL_NATIVE";
			case OpCode::OP_OR: return L"OP_OR";
			case OpCode::OP_AND: return L"OP_AND";
			default: return L"UNKNOWN_OPCODE";
		}
	}

	class Node
	{
	private:
		NodeType mType;

	public:
		Node(NodeType mType) : mType(mType) {}
		virtual ~Node() = default;
		NodeType GetType() const { return mType; }
	};

	class Node_statement : public Node
	{
	public:
		Node_statement(NodeType mType) : Node(mType) {}
		virtual void Execute() const = 0;
	};

	class Node_expression : public Node
	{
	public:
		Node_expression(NodeType mType) : Node(mType) {}
		virtual Value Evaluate() const = 0;
	};

	class Node_root : public Node
	{
	private:
		std::deque<std::unique_ptr<Node_statement>> mStatements;

	public:
		Node_root() : Node(NodeType::NT_ROOT) {}
		void AddStatement(std::unique_ptr<Node_statement> statement) { mStatements.push_back(std::move(statement)); }
		const std::deque<std::unique_ptr<Node_statement>>& GetStatements() const { return mStatements; }
	};

	class Node_function : public Node_statement
	{
	private:
		std::wstring mName;
		std::deque<std::unique_ptr<Node_variable>> mParameters;
		std::deque<std::unique_ptr<Node_statement>> mBody;

	public:
		Node_function(const std::wstring& name, std::deque<std::unique_ptr<Node_variable>> parameters)
			: Node_statement(NodeType::NT_FUNC), mName(name), mParameters(std::move(parameters)) {}
		void AddStatement(std::unique_ptr<Node_statement> statement) { mBody.push_back(std::move(statement)); }
		const std::wstring& GetName() const { return mName; }
		const std::deque<std::unique_ptr<Node_variable>>& GetParameters() const { return mParameters; }
		const std::deque<std::unique_ptr<Node_statement>>& GetBody() const { return mBody; }
		void Execute() const override {}
	};

	class Node_ifStatement : public Node_statement
	{
	private:
		std::unique_ptr<Node_expression> mCondition;
		std::deque<std::unique_ptr<Node_statement>> mThenBody;
		std::deque<std::pair<std::unique_ptr<Node_expression>, std::deque<std::unique_ptr<Node_statement>>>> mElifBodies;
		std::deque<std::unique_ptr<Node_statement>> mElseBody;

	public:
		Node_ifStatement(std::unique_ptr<Node_expression> condition)
			: Node_statement(NodeType::NT_IF_STATEMENT), mCondition(std::move(condition)) {}
		void AddThenStatement(std::unique_ptr<Node_statement> statement) { mThenBody.push_back(std::move(statement)); }
		void AddElif(std::unique_ptr<Node_expression> condition, std::deque<std::unique_ptr<Node_statement>> body)
		{
			mElifBodies.emplace_back(std::move(condition), std::move(body));
		}
		void SetElseBody(std::deque<std::unique_ptr<Node_statement>> body) { mElseBody = std::move(body); }
		const Node_expression* GetCondition() const { return mCondition.get(); }
		const std::deque<std::unique_ptr<Node_statement>>& GetThenBody() const { return mThenBody; }
		const std::deque<std::pair<std::unique_ptr<Node_expression>, std::deque<std::unique_ptr<Node_statement>>>>& GetElifBodies() const { return mElifBodies; }
		const std::deque<std::unique_ptr<Node_statement>>& GetElseBody() const { return mElseBody; }
		void Execute() const override {}
	};

	class Node_returnStatement : public Node_statement
	{
	private:
		std::unique_ptr<Node_expression> mExpression;

	public:
		Node_returnStatement(std::unique_ptr<Node_expression> expression)
			: Node_statement(NodeType::NT_RETURN_STATEMENT), mExpression(std::move(expression)) {}
		const Node_expression* GetExpression() const { return mExpression.get(); }
		void Execute() const override {}
	};

	class Node_expressionStatement : public Node_statement
	{
	private:
		std::unique_ptr<Node_expression> mExpression;

	public:
		Node_expressionStatement(std::unique_ptr<Node_expression> expression)
			: Node_statement(NodeType::NT_EXPRESSION_STATEMENT), mExpression(std::move(expression)) {}
		const Node_expression* GetExpression() const { return mExpression.get(); }
		void Execute() const override {}
	};

	class Node_binaryOperation : public Node_expression
	{
	private:
		std::unique_ptr<Node_expression> mLeft;
		TokenType mOp;
		std::unique_ptr<Node_expression> mRight;

	public:
		Node_binaryOperation(std::unique_ptr<Node_expression> left, TokenType op, std::unique_ptr<Node_expression> right)
			: Node_expression(NodeType::NT_BINARY_OPERATION), mLeft(std::move(left)), mOp(op), mRight(std::move(right)) {}
		const Node_expression* GetLeft() const { return mLeft.get(); }
		TokenType GetOperator() const { return mOp; }
		const Node_expression* GetRight() const { return mRight.get(); }
		Value Evaluate() const override { return 0; }
	};

	class Node_unaryOperation : public Node_expression
	{
	private:
		TokenType mOp;
		std::unique_ptr<Node_expression> mOperand;

	public:
		Node_unaryOperation(TokenType op, std::unique_ptr<Node_expression> operand)
			: Node_expression(NodeType::NT_UNARY_OPERATION), mOp(op), mOperand(std::move(operand)) {}
		TokenType GetOperator() const { return mOp; }
		const Node_expression* GetOperand() const { return mOperand.get(); }
		Value Evaluate() const override { return 0; }
	};

	class Node_variable : public Node_expression
	{
	private:
		std::wstring mName;

	public:
		Node_variable(const std::wstring& name) : Node_expression(NodeType::NT_VARIABLE), mName(name) {}
		const std::wstring& GetName() const { return mName; }
		Value Evaluate() const override { return 0; }
	};

	class Node_literal : public Node_expression
	{
	private:
		Value mValue;

	public:
		Node_literal(Value value) : Node_expression(NodeType::NT_LITERAL), mValue(value) {}
		Value GetValue() const { return mValue; }
		Value Evaluate() const override { return mValue; }
	};

	class Node_grouping : public Node_expression
	{
	public:
		Node_grouping(std::unique_ptr<Node_expression> expression)
			: Node_expression(NodeType::NT_GROUPING), mExpression(std::move(expression)) {}
		const Node_expression* GetExpression() const { return mExpression.get(); }
		Value Evaluate() const override { return 0; }
	private:
		std::unique_ptr<Node_expression> mExpression;
	};

	class Node_assignment : public Node_expression
	{
	private:
		std::wstring mName;
		std::unique_ptr<Node_expression> mValue;

	public:
		Node_assignment(const std::wstring& name, std::unique_ptr<Node_expression> value)
			: Node_expression(NodeType::NT_ASSIGNMENT), mName(name), mValue(std::move(value)) {}
		const std::wstring& GetName() const { return mName; }
		const Node_expression* GetValue() const { return mValue.get(); }
		Value Evaluate() const override { return 0; }
	};

	class Node_call : public Node_expression
	{
	private:
		std::wstring mCallee;
		std::deque<std::unique_ptr<Node_expression>> mArguments;

	public:
		Node_call(const std::wstring& callee, std::deque<std::unique_ptr<Node_expression>> arguments)
			: Node_expression(NodeType::NT_CALL), mCallee(callee), mArguments(std::move(arguments)) {}
		
		const std::wstring& GetCallee() const { return mCallee; }
		const std::deque<std::unique_ptr<Node_expression>>& GetArguments() const { return mArguments; }
		Value Evaluate() const override { return 0; }
	};

	struct Token
	{
		TokenType mType;
		std::wstring mLexeme;
		int64_t mValue;
		int mIndentLevel;
	};

	using NativeFunction = std::function<Value(const std::deque<Value>&)>;

	struct Bytecode
	{
		std::deque<Value> mCodes;
		std::deque<NativeFunction> mNativeFuncs;
		std::unordered_map<std::wstring, size_t> mScriptFuncOffsets;
		size_t mGlobalOffset = 0; // 전역 실행 코드의 시작 오프셋 (함수 정의 이후)
	};

	struct CallFrame
	{
		size_t mReturnIp = 0;
		size_t mParamCnt = 0;
		std::deque<Value> mVariables;
	};
}
