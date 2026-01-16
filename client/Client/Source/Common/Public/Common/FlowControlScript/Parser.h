// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Types.h"


namespace FlowControlScript
{
	/**
	 * Parser (구문 분석기)
	 * 
	 * Lexer로부터 생성된 토큰 스트림을 받아서 AST(Abstract Syntax Tree, 추상 구문 트리)를 생성합니다.
	 * 
	 * 주요 기능:
	 *  - 토큰 스트림을 AST 노드로 변환
	 *  - 문법 규칙 검증
	 *  - 연산자 우선순위 처리
	 *  - 재귀 하향 파싱(Recursive Descent Parsing) 방식 사용
	 * 
	 * 지원하는 문법 구조:
	 *  - 함수 정의: def function_name(params):
	 *  - 조건문: if condition: ... else: ...
	 *  - 표현식: 산술, 논리, 비교 연산
	 *  - 변수 할당: x = value
	 *  - 함수 호출: function_name(args)
	 *  - return 문
	 * 
	 * 연산자 우선순위 (낮음 → 높음):
	 *  - 논리 OR (||)
	 *  - 논리 AND (&&)
	 *  - 동등 비교 (==, !=)
	 *  - 관계 비교 (>, <, >=, <=)
	 *  - 덧셈/뺄셈 (+, -)
	 *  - 곱셈/나눗셈 (*, /)
	 *  - 단항 연산 (!, -)
	 */
	class Parser
	{
	private:
		const std::deque<Token>& mTokens;
		size_t mCurrent;
		size_t mCurrentIndent;

	public:
		explicit Parser(const std::deque<Token>& tokens);
		virtual ~Parser() = default;

	public:
		std::unique_ptr<Node_root> Parse();

	private:
		std::unique_ptr<Node_statement> ParseStatement();
		std::unique_ptr<Node_statement> ParseFunction();
		std::unique_ptr<Node_statement> ParseIfStatement();
		std::unique_ptr<Node_statement> ParseReturnStatement();
		std::unique_ptr<Node_statement> ParseExpressionStatement();
		std::unique_ptr<Node_expression> ParseExpression();
		std::unique_ptr<Node_expression> ParseAssignment();
		std::unique_ptr<Node_expression> ParseOr();
		std::unique_ptr<Node_expression> ParseAnd();
		std::unique_ptr<Node_expression> ParseEquality();
		std::unique_ptr<Node_expression> ParseComparison();
		std::unique_ptr<Node_expression> ParseTerm();
		std::unique_ptr<Node_expression> ParseFactor();
		std::unique_ptr<Node_expression> ParseUnary();
		std::unique_ptr<Node_expression> ParsePrimary();

		bool Match(TokenType mType);
		bool Match(TokenType type1, TokenType type2);
		bool Match(TokenType type1, TokenType type2, TokenType type3);
		bool Match(TokenType type1, TokenType type2, TokenType type3, TokenType type4);
		const Token& Consume(TokenType mType, const std::wstring& message);
		bool Check(TokenType mType) const;
		const Token& Advance();
		bool IsAtEnd() const;
		const Token& Peek() const;
		const Token& Previous() const;

		bool IsFunctionKeyword() const;
		bool IsIfKeyword() const;
		bool IsReturnKeyword() const;

	private:
		void ReportError(const Token& token, const std::wstring& message);
		void PrintAstNodeDebug(const Node* node, int indent) const;
	};
}
