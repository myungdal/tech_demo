// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

/**
 * Lexer.cpp - 어휘 분석기 구현
 * 
 * FlowControlScript 소스 코드를 토큰으로 분해합니다.
 * Python과 유사한 들여쓰기 기반 문법을 지원합니다.
 */

#include "Common/FlowControlScript/Lexer.h"

#include "Common/FlowControlScript/Types.h"


namespace FlowControlScript
{
	Lexer::Lexer(const std::wstring& source)
		: 
		mSource(source), 
		mStart(0), 
		mCurrent(0), 
		mCurrentIndent(0)
	{
	}

	std::deque<Token> Lexer::ScanTokens()
	{
		while (!IsAtEnd())
		{
			mStart = mCurrent;
			ScanToken();
		}
		mTokens.emplace_back(Token{ TokenType::TK_EOF, L"", 0, mCurrentIndent });

		// 디버그 정보 출력
		//PrintLexerDebug();

		return mTokens;
	}

	// Alias for ScanTokens to match test file usage
	std::deque<Token> Lexer::Tokenize()
	{
		return ScanTokens();
	}

	void Lexer::ScanToken()
	{
		// Check if we're at the start of a new line
		bool isNewLine = (mCurrent == 0 || mSource[mCurrent - 1] == L'\n');

		// Only reset and count indentation if we're at the start of a new line
		if (isNewLine) 
		{
			mCurrentIndent = 0;
			while (Peek() == L' ' || Peek() == L'\t')
			{
				Advance();
				mCurrentIndent++;
			}
		}
		else 
		{
			// Skip spaces without affecting indentation
			while (Peek() == L' ' || Peek() == L'\t')
			{
				Advance();
			}
		}

		// Update token start position
		mStart = mCurrent;

		wchar_t c = Advance();
		switch (c)
		{
		case L'(': AddToken(TokenType::TK_LEFT_PAREN); break;
		case L')': AddToken(TokenType::TK_RIGHT_PAREN); break;
		case L'*': AddToken(TokenType::TK_MULTIPLY); break;
		case L'-':
			if (Match(L'-')) 
			{
				// Skip comments until end of line
				while (Peek() != L'\n' && !IsAtEnd()) 
					Advance();
			}
			else if (IsDigit(Peek()) && IsStartOfExpression()) 
				Number(); // 식의 시작이면서 다음이 숫자면 부호
			else 
				AddToken(TokenType::TK_MINUS);
			break;
		case L'+':
			if (IsDigit(Peek()) && IsStartOfExpression()) 
				Number(); // 식의 시작이면서 다음이 숫자면 부호
			else 
				AddToken(TokenType::TK_PLUS);
			break;
		case L'!':
			AddToken(Match(L'=') ? TokenType::TK_NOT_EQUAL : TokenType::TK_NOT);
			break;
		case L'=':
			AddToken(Match(L'=') ? TokenType::TK_EQUAL : TokenType::TK_ASSIGN);
			break;
		case L'<':
			AddToken(Match(L'=') ? TokenType::TK_LESS_EQUAL : TokenType::TK_LESS);
			break;
		case L'>':
			AddToken(Match(L'=') ? TokenType::TK_GREATER_EQUAL : TokenType::TK_GREATER);
			break;
		case L'/':
			if (Match(L'/'))
			{
				// Skip comments until end of line
				while (Peek() != L'\n' && !IsAtEnd()) 
					Advance();
			}
			else
				AddToken(TokenType::TK_DIVISION);
			break;
		case L'&':
			if (Match(L'&'))
				AddToken(TokenType::TK_AND);
			else
				ReportError(L"Expected '&' after '&'.");
			break;
		case L'|':
			if (Match(L'|'))
				AddToken(TokenType::TK_OR);
			else
				ReportError(L"Expected '|' after '|'.");
			break;
		case L'\n':
			mCurrentIndent = 0; // Reset indentation at each new line
			break;
		case L'\r':
		case L'\t':
		case L' ':
		case L',':
		case L':':
			break; // Skip whitespace within a line
		default:
			if (IsDigit(c))
				Number();
			else if (IsAlpha(c))
				Identifier();
			else
				ReportError(L"Unexpected character.");
			break;
		}
	}

	void Lexer::Number()
	{
		// 이미 부호를 읽은 상태라면 그대로 진행
		if (mSource[mStart] != L'+' && mSource[mStart] != L'-')
			mCurrent--; // 숫자 시작 위치로 되돌림

		while (IsDigit(Peek()))
			Advance();

		const std::wstring lexeme = mSource.substr(mStart, mCurrent - mStart);
		const Value value = std::stoll(lexeme);

		AddToken(TokenType::TK_NUMBER, value);
	}

	void Lexer::Identifier()
	{
		while (true) {
			wchar_t c = Peek();
			if (IsAlphaNumeric(c)) {
				Advance();
			}
			else if (c == L':') {
				// 다음 문자가 ':'인지 확인하여 "::"이면 식별자에 포함시킴
				if (PeekNext() == L':') {
					Advance(); // 첫번째 ':'
					Advance(); // 두번째 ':'
				}
				else {
					// 단일 ':'이면 식별자에 포함시키지 않고 종료
					break;
				}
			}
			else {
				break;
			}
		}

		const std::wstring lexeme = mSource.substr(mStart, mCurrent - mStart);

		TokenType mType = TokenType::TK_IDENTIFIER;
		if (lexeme == L"def")
			mType = TokenType::TK_DEF;
		else if (lexeme == L"if")
			mType = TokenType::TK_IF;
		else if (lexeme == L"else")
			mType = TokenType::TK_ELSE;
		else if (lexeme == L"return")
			mType = TokenType::TK_RETURN;

		AddToken(mType);
	}

	bool Lexer::Match(wchar_t expected)
	{
		if (IsAtEnd()) return false;
		if (mSource[mCurrent] != expected) return false;
		mCurrent++;
		return true;
	}

	wchar_t Lexer::Peek() const
	{
		if (IsAtEnd()) return L'\0';
		return mSource[mCurrent];
	}

	wchar_t Lexer::PeekNext() const
	{
		if (mCurrent + 1 >= mSource.length()) return L'\0';
		return mSource[mCurrent + 1];
	}

	bool Lexer::IsAlpha(wchar_t c) const
	{
		return (c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || c == L'_';
	}

	bool Lexer::IsDigit(wchar_t c) const
	{
		return c >= L'0' && c <= L'9';
	}

	bool Lexer::IsAlphaNumeric(wchar_t c) const
	{
		return IsAlpha(c) || IsDigit(c);
	}

	bool Lexer::IsAtEnd() const
	{
		return mCurrent >= mSource.length();
	}

	wchar_t Lexer::Advance()
	{
		mCurrent++;
		return mSource[mCurrent - 1];
	}

	void Lexer::AddToken(TokenType type, Value value)
	{
		const std::wstring lexeme = mSource.substr(mStart, mCurrent - mStart);
		mTokens.emplace_back(Token{ type, lexeme, value, mCurrentIndent });
	}

	bool Lexer::IsStartOfExpression() const
	{
		// 토큰이 없거나 마지막 토큰이 연산자/여는 괄호면 식의 시작으로 간주
		if (mTokens.empty()) return true;

		const Token& lastToken = mTokens.back();
		return lastToken.mType == TokenType::TK_LEFT_PAREN ||
			lastToken.mType == TokenType::TK_PLUS ||
			lastToken.mType == TokenType::TK_MINUS ||
			lastToken.mType == TokenType::TK_MULTIPLY ||
			lastToken.mType == TokenType::TK_DIVISION ||
			lastToken.mType == TokenType::TK_ASSIGN;
	}

	void Lexer::ReportError([[maybe_unused]] const std::wstring& message)
	{
		throw std::runtime_error("Lexer generation error");
	}

	void Lexer::PrintLexerDebug() const
	{
		std::wcout << L"\n*** Lexer Debug Output ***\n";
		std::wcout << L"Source length: " << mSource.length() << L" characters\n";
		
		// 소스 코드 출력
		std::wcout << L"\n-- Source Code: --\n";
		std::wstring lineNumber = L"1";
		std::wcout << lineNumber << L" | ";
		for (size_t i = 0; i < mSource.length(); i++) {
			std::wcout << mSource[i];
			if (mSource[i] == L'\n' && i < mSource.length() - 1) {
				lineNumber = std::to_wstring(std::stoi(lineNumber) + 1);
				std::wcout << lineNumber << L" | ";
			}
		}
		std::wcout << L"\n";

		// 토큰 목록 출력
		std::wcout << L"\n-- Tokens: --\n";
		for (size_t i = 0; i < mTokens.size(); i++) {
			PrintTokenDebug(mTokens[i], i);
		}
		std::wcout << L"\n";

		// 통계 정보 출력
		std::wcout << L"-- Statistics: --\n";
		std::wcout << L"Total tokens: " << mTokens.size() << L"\n";
		
		// 토큰 타입별 개수 카운트
		std::map<TokenType, size_t> tokenCounts;
		for (const auto& token : mTokens) {
			tokenCounts[token.mType]++;
		}
		
		std::wcout << L"Token type distribution:\n";
		for (const auto& [type, count] : tokenCounts) {
			std::wcout << L"  " << TokenTypeToString(type) << L": " << count << L"\n";
		}
		std::wcout << L"\n";
	}

	void Lexer::PrintTokenDebug(const Token& token, size_t index) const
	{
		std::wcout << std::setw(4) << index << L": ";
		std::wcout << L"Type: " << std::setw(15) << TokenTypeToString(token.mType);
		
		// 토큰 타입별로 다른 정보 출력
		switch (token.mType) {
		case TokenType::TK_NUMBER:
			std::wcout << L" Value: " << token.mValue;
			break;
		case TokenType::TK_IDENTIFIER:
		case TokenType::TK_STRING:
			std::wcout << L" Lexeme: '" << token.mLexeme << L"'";
			break;
		}
		
		std::wcout << L" Indent: " << token.mIndentLevel << L"\n";
	}
}
