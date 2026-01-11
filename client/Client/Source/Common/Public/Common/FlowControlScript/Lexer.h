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
	 * Lexer (어휘 분석기)
	 * 
	 * FlowControlScript DSL(Domain-Specific Language) 소스 코드를 토큰(Token)으로 분해하는 역할을 담당합니다.
	 * 
	 * 주요 기능:
	 *  - 소스 코드 문자열을 토큰 스트림으로 변환
	 *  - 키워드, 식별자, 연산자, 리터럴 등을 인식
	 *  - 들여쓰기(indentation) 처리 (Python-like)
	 *  - 주석 및 공백 처리
	 * 
	 * 인식하는 토큰 타입:
	 *  - 키워드: def, if, else, return
	 *  - 연산자: +, -, *, /, ==, !=, >, <, >=, <=, &&, ||, !
	 *  - 리터럴: 숫자, 문자열
	 *  - 식별자: 변수명, 함수명
	 *  - 구분자: (, ), :
	 */
	class Lexer
	{
	private:
		std::wstring mSource;
		std::deque<Token> mTokens;
		size_t mStart;
		size_t mCurrent;
		int mCurrentIndent;

	public:
		explicit Lexer(const std::wstring& source);
		virtual ~Lexer() = default;

	public:
		std::deque<Token> ScanTokens();
		std::deque<Token> Tokenize();

		void ScanToken();
		void Number();
		void Identifier();
		bool Match(wchar_t expected);
		wchar_t Peek() const;
		wchar_t PeekNext() const;
		bool IsAlpha(wchar_t c) const;
		bool IsDigit(wchar_t c) const;
		bool IsAlphaNumeric(wchar_t c) const;
		bool IsAtEnd() const;
		wchar_t Advance();
		void AddToken(TokenType type, Value value = 0);
		bool IsStartOfExpression() const;
		void ReportError(const std::wstring& message);

	private:
		void PrintLexerDebug() const;
		void PrintTokenDebug(const Token& token, size_t index) const;
	};
}
