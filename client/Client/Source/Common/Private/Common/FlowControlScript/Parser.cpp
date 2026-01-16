// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

/**
 * Parser.cpp - 구문 분석기 구현
 * 
 * 토큰 스트림을 받아 AST(추상 구문 트리)를 생성합니다.
 * 재귀 하향 파싱(Recursive Descent Parsing) 방식을 사용합니다.
 */

#include "Common/FlowControlScript/Parser.h"


namespace FlowControlScript
{
	Parser::Parser(const std::deque<Token>& tokens)
		: mTokens(tokens), mCurrent(0), mCurrentIndent(0)
	{
	}

	std::unique_ptr<Node_root> Parser::Parse()
	{
		auto root = std::make_unique<Node_root>();
		while (!IsAtEnd())
		{
			root->AddStatement(ParseStatement());
		}

		// AST 구조를 콘솔에 디버그 정보 출력
		//std::wcout << L"*** AST Structure:\n";
		//PrintAstNodeDebug(root.get(), 0);
		
		return root;
	}

	std::unique_ptr<Node_statement> Parser::ParseStatement()
	{
		if (IsReturnKeyword())
		{
			return ParseReturnStatement();
		}
		else if (IsIfKeyword())
		{
			return ParseIfStatement();
		}        
		else if (IsFunctionKeyword())
		{
			return ParseFunction();
		}
		else
		{
			return ParseExpressionStatement();
		}
	}

	std::unique_ptr<Node_statement> Parser::ParseFunction()
	{
		size_t funcIndent = mTokens[mCurrent].mIndentLevel;
		Consume(TokenType::TK_DEF, L"Expected def");
		Token name = Consume(TokenType::TK_IDENTIFIER, L"Expected function name");
		Consume(TokenType::TK_LEFT_PAREN, L"Expected '(' after function name");

		// 매개변수를 Node_variable의 vector로 변경
		std::deque<std::unique_ptr<Node_variable>> parameters;
		while (!Check(TokenType::TK_RIGHT_PAREN))
		{
			Token paramName = Consume(TokenType::TK_IDENTIFIER, L"Expected parameter name");
			parameters.push_back(std::make_unique<Node_variable>(paramName.mLexeme));
			
			// 콤마 처리 추가
			if (Check(TokenType::TK_RIGHT_PAREN)) break;
			// 여기서 콤마를 소비하는 로직을 추가할 수 있습니다
		}
		Consume(TokenType::TK_RIGHT_PAREN, L"Expected ')' after parameters");

		auto function = std::make_unique<Node_function>(name.mLexeme, std::move(parameters));
		while (!IsAtEnd() && mTokens[mCurrent].mIndentLevel > funcIndent)
		{
			function->AddStatement(ParseStatement());
		}
		return function;
	}

	std::unique_ptr<Node_statement> Parser::ParseIfStatement()
	{
		size_t ifIndent = mTokens[mCurrent].mIndentLevel;
		Consume(TokenType::TK_IF, L"Expected 'if'");
		auto condition = ParseExpression();
		auto ifStatement = std::make_unique<Node_ifStatement>(std::move(condition));

		// Parse then branch
		while (!IsAtEnd() && mTokens[mCurrent].mIndentLevel > ifIndent)
		{
			// 현재 토큰의 들여쓰기 레벨이 if 문의 들여쓰기 레벨보다 큰 경우에만 파싱
			if (mTokens[mCurrent].mIndentLevel > ifIndent) {
				ifStatement->AddThenStatement(ParseStatement());
			} else {
				break;
			}
		}

		// Parse elif/else branches
		if (Match(TokenType::TK_ELSE))
		{
			if (mTokens[mCurrent - 1].mIndentLevel != ifIndent)
			{
				ReportError(mTokens[mCurrent - 1], L"Else must be at the same indentation level as if");
			}
			
			// else 다음에 if가 오는지 확인 (else if 구문)
			if (Check(TokenType::TK_IF)) {
				Advance(); // if 토큰 소비
				auto elifCondition = ParseExpression();
				std::deque<std::unique_ptr<Node_statement>> elifBody;
				while (!IsAtEnd() && mTokens[mCurrent].mIndentLevel > ifIndent) {
					elifBody.push_back(ParseStatement());
				}
				ifStatement->AddElif(std::move(elifCondition), std::move(elifBody));
			} 
			// 단독 else 구문
			else {
				std::deque<std::unique_ptr<Node_statement>> elseBody;
				while (!IsAtEnd() && mTokens[mCurrent].mIndentLevel > ifIndent) {
					elseBody.push_back(ParseStatement());
				}
				ifStatement->SetElseBody(std::move(elseBody));
			}
		}

		return ifStatement;
	}

	std::unique_ptr<Node_statement> Parser::ParseReturnStatement()
	{
		Consume(TokenType::TK_RETURN, L"Expected 'return'");
		auto expression = ParseExpression();
		return std::make_unique<Node_returnStatement>(std::move(expression));
	}

	std::unique_ptr<Node_statement> Parser::ParseExpressionStatement()
	{
		auto expr = ParseExpression();
		return std::make_unique<Node_expressionStatement>(std::move(expr));
	}

	std::unique_ptr<Node_expression> Parser::ParseExpression()
	{
		return ParseAssignment();
	}

	std::unique_ptr<Node_expression> Parser::ParseAssignment()
	{
		auto expr = ParseOr();
		if (Match(TokenType::TK_ASSIGN))
		{
			Token equals = Previous();
			auto value = ParseAssignment();
			if (auto* variable = dynamic_cast<Node_variable*>(expr.get()))
			{
				expr.release(); // Prevent double deletion
				return std::make_unique<Node_assignment>(variable->GetName(), std::move(value));
			}
			ReportError(equals, L"Invalid assignment target");
		}
		return expr;
	}

	std::unique_ptr<Node_expression> Parser::ParseOr()
	{
		auto expr = ParseAnd();
		while (Match(TokenType::TK_OR))
		{
			Token op = Previous();
			auto right = ParseAnd();
			expr = std::make_unique<Node_binaryOperation>(std::move(expr), op.mType, std::move(right));
		}
		return expr;
	}

	std::unique_ptr<Node_expression> Parser::ParseAnd()
	{
		auto expr = ParseEquality();
		while (Match(TokenType::TK_AND))
		{
			Token op = Previous();
			auto right = ParseEquality();
			expr = std::make_unique<Node_binaryOperation>(std::move(expr), op.mType, std::move(right));
		}
		return expr;
	}

	std::unique_ptr<Node_expression> Parser::ParseEquality()
	{
		auto expr = ParseComparison();
		while (Match(TokenType::TK_EQUAL, TokenType::TK_NOT_EQUAL))
		{
			Token op = Previous();
			auto right = ParseComparison();
			expr = std::make_unique<Node_binaryOperation>(std::move(expr), op.mType, std::move(right));
		}
		return expr;
	}

	std::unique_ptr<Node_expression> Parser::ParseComparison()
	{
		auto expr = ParseTerm();
		while (Match(TokenType::TK_GREATER, TokenType::TK_GREATER_EQUAL, TokenType::TK_LESS, TokenType::TK_LESS_EQUAL))
		{
			Token op = Previous();
			auto right = ParseTerm();
			expr = std::make_unique<Node_binaryOperation>(std::move(expr), op.mType, std::move(right));
		}
		return expr;
	}

	std::unique_ptr<Node_expression> Parser::ParseTerm()
	{
		auto expr = ParseFactor();
		while (Match(TokenType::TK_PLUS, TokenType::TK_MINUS))
		{
			Token op = Previous();
			auto right = ParseFactor();
			expr = std::make_unique<Node_binaryOperation>(std::move(expr), op.mType, std::move(right));
		}
		return expr;
	}

	std::unique_ptr<Node_expression> Parser::ParseFactor()
	{
		auto expr = ParseUnary();
		while (Match(TokenType::TK_MULTIPLY, TokenType::TK_DIVISION))
		{
			Token op = Previous();
			auto right = ParseUnary();
			expr = std::make_unique<Node_binaryOperation>(std::move(expr), op.mType, std::move(right));
		}
		return expr;
	}

	std::unique_ptr<Node_expression> Parser::ParseUnary()
	{
		if (Match(TokenType::TK_MINUS, TokenType::TK_NOT, TokenType::TK_PLUS))
		{
			Token op = Previous();
			auto right = ParseUnary();
			return std::make_unique<Node_unaryOperation>(op.mType, std::move(right));
		}
		return ParsePrimary();
	}

	std::unique_ptr<Node_expression> Parser::ParsePrimary()
	{
		if (Match(TokenType::TK_NUMBER))
			return std::make_unique<Node_literal>(std::stoll(Previous().mLexeme));
		
		if (Match(TokenType::TK_IDENTIFIER))
		{
			Token name = Previous();
			if (Match(TokenType::TK_LEFT_PAREN))
			{
				std::deque<std::unique_ptr<Node_expression>> args;
				while (!Check(TokenType::TK_RIGHT_PAREN))
				{
					args.push_back(ParseExpression());
				}
				Consume(TokenType::TK_RIGHT_PAREN, L"Expect ')' after arguments");
				
				return std::make_unique<Node_call>(name.mLexeme, std::move(args));
			}
			return std::make_unique<Node_variable>(name.mLexeme);
		}
		
		if (Match(TokenType::TK_LEFT_PAREN))
		{
			auto expr = ParseExpression();
			Consume(TokenType::TK_RIGHT_PAREN, L"Expect ')' after expression");
			return std::make_unique<Node_grouping>(std::move(expr));
		}
		
		ReportError(Peek(), L"Expect expression");
		return std::make_unique<Node_literal>(0);
	}

	bool Parser::Match(TokenType mType)
	{
		if (Check(mType))
		{
			Advance();
			return true;
		}
		return false;
	}

	bool Parser::Match(TokenType type1, TokenType type2)
	{
		if (Check(type1) || Check(type2))
		{
			Advance();
			return true;
		}
		return false;
	}

	bool Parser::Match(TokenType type1, TokenType type2, TokenType type3)
	{
		if (Check(type1) || Check(type2) || Check(type3))
		{
			Advance();
			return true;
		}
		return false;
	}

	bool Parser::Match(TokenType type1, TokenType type2, TokenType type3, TokenType type4)
	{
		if (Check(type1) || Check(type2) || Check(type3) || Check(type4))
		{
			Advance();
			return true;
		}
		return false;
	}

	const Token& Parser::Consume(TokenType mType, const std::wstring& message)
	{
		if (Check(mType)) return Advance();
		ReportError(Peek(), message);
		return Peek(); // 현재 토큰을 반환하여 파싱을 계속할 수 있게 함
	}

	bool Parser::Check(TokenType mType) const
	{
		if (IsAtEnd()) return false;
		return Peek().mType == mType;
	}

	const Token& Parser::Advance()
	{
		if (!IsAtEnd()) mCurrent++;
		return Previous();
	}

	bool Parser::IsAtEnd() const
	{
		return Peek().mType == TokenType::TK_EOF;
	}

	const Token& Parser::Peek() const
	{
		return mTokens[mCurrent];
	}

	const Token& Parser::Previous() const
	{
		if (mCurrent > 0)
			return mTokens[mCurrent - 1];
		else
			return mTokens[0];
	}

	bool Parser::IsFunctionKeyword() const
	{
		return Check(TokenType::TK_DEF);
	}

	bool Parser::IsIfKeyword() const
	{
		return Check(TokenType::TK_IF);
	}

	bool Parser::IsReturnKeyword() const
	{
		return Check(TokenType::TK_RETURN);
	}

	void Parser::ReportError([[maybe_unused]] const Token& token, [[maybe_unused]] const std::wstring& message)
	{
		throw std::runtime_error("VirtualMachine error");
	}

	// 노드 구조를 콘솔에 출력하는 재귀 함수
	void Parser::PrintAstNodeDebug(const Node* node, int indent) const
	{
		std::wstring indentStr(indent * 2, L' ');
		
		if (!node) {
			std::wcout << indentStr << L"<null>\n";
			return;
		}

		// 노드 타입에 따라 출력
		switch (node->GetType()) {
			case NodeType::NT_ROOT: {
				std::wcout << indentStr << L"Root\n";
				const auto* root = static_cast<const Node_root*>(node);
				for (const auto& stmt : root->GetStatements()) {
					PrintAstNodeDebug(stmt.get(), indent + 1);
				}
				break;
			}
			case NodeType::NT_FUNC: {
				const auto* func = static_cast<const Node_function*>(node);
				std::wcout << indentStr << L"Function: " << func->GetName() << L"(";
				const auto& params = func->GetParameters();
				for (size_t i = 0; i < params.size(); ++i) {
					std::wcout << params[i]->GetName();
					if (i < params.size() - 1) std::wcout << L", ";
				}
				std::wcout << L")\n";
				for (const auto& stmt : func->GetBody()) {
					PrintAstNodeDebug(stmt.get(), indent + 1);
				}
				break;
			}
			case NodeType::NT_IF_STATEMENT: {
				const auto* ifStmt = static_cast<const Node_ifStatement*>(node);
				std::wcout << indentStr << L"If\n";
				std::wcout << indentStr << L"  Condition:\n";
				PrintAstNodeDebug(ifStmt->GetCondition(), indent + 2);
				std::wcout << indentStr << L"  Then:\n";
				for (const auto& stmt : ifStmt->GetThenBody()) {
					PrintAstNodeDebug(stmt.get(), indent + 2);
				}
				
				// Elif 분기 출력
				const auto& elifBodies = ifStmt->GetElifBodies();
				for (size_t i = 0; i < elifBodies.size(); ++i) {
					std::wcout << indentStr << L"  Elif " << (i + 1) << L":\n";
					std::wcout << indentStr << L"    Condition:\n";
					PrintAstNodeDebug(elifBodies[i].first.get(), indent + 3);
					std::wcout << indentStr << L"    Body:\n";
					for (const auto& stmt : elifBodies[i].second) {
						PrintAstNodeDebug(stmt.get(), indent + 3);
					}
				}
				
				// Else 분기 출력
				if (!ifStmt->GetElseBody().empty()) {
					std::wcout << indentStr << L"  Else:\n";
					for (const auto& stmt : ifStmt->GetElseBody()) {
						PrintAstNodeDebug(stmt.get(), indent + 2);
					}
				}
				break;
			}
			case NodeType::NT_RETURN_STATEMENT: {
				const auto* returnStmt = static_cast<const Node_returnStatement*>(node);
				std::wcout << indentStr << L"Return\n";
				PrintAstNodeDebug(returnStmt->GetExpression(), indent + 1);
				break;
			}
			case NodeType::NT_EXPRESSION_STATEMENT: {
				const auto* exprStmt = static_cast<const Node_expressionStatement*>(node);
				std::wcout << indentStr << L"ExpressionStatement\n";
				PrintAstNodeDebug(exprStmt->GetExpression(), indent + 1);
				break;
			}
			case NodeType::NT_BINARY_OPERATION: {
				const auto* binOp = static_cast<const Node_binaryOperation*>(node);
				std::wcout << indentStr << L"BinaryOperation: " << TokenTypeToString(binOp->GetOperator()) << L"\n";
				std::wcout << indentStr << L"  Left:\n";
				PrintAstNodeDebug(binOp->GetLeft(), indent + 2);
				std::wcout << indentStr << L"  Right:\n";
				PrintAstNodeDebug(binOp->GetRight(), indent + 2);
				break;
			}
			case NodeType::NT_UNARY_OPERATION: {
				const auto* unaryOp = static_cast<const Node_unaryOperation*>(node);
				std::wcout << indentStr << L"UnaryOperation: " << TokenTypeToString(unaryOp->GetOperator()) << L"\n";
				PrintAstNodeDebug(unaryOp->GetOperand(), indent + 1);
				break;
			}
			case NodeType::NT_VARIABLE: {
				const auto* var = static_cast<const Node_variable*>(node);
				std::wcout << indentStr << L"Variable: " << var->GetName() << L"\n";
				break;
			}
			case NodeType::NT_LITERAL: {
				const auto* literal = static_cast<const Node_literal*>(node);
				std::wcout << indentStr << L"Literal: " << literal->GetValue() << L"\n";
				break;
			}
			case NodeType::NT_GROUPING: {
				const auto* group = static_cast<const Node_grouping*>(node);
				std::wcout << indentStr << L"Grouping:\n";
				PrintAstNodeDebug(group->GetExpression(), indent + 1);
				break;
			}
			case NodeType::NT_ASSIGNMENT: {
				const auto* assign = static_cast<const Node_assignment*>(node);
				std::wcout << indentStr << L"Assignment: " << assign->GetName() << L" =\n";
				PrintAstNodeDebug(assign->GetValue(), indent + 1);
				break;
			}
			case NodeType::NT_CALL: {
				const auto* call = static_cast<const Node_call*>(node);
				std::wcout << indentStr << L"Call: " << call->GetCallee() << L"(\n";
				const auto& args = call->GetArguments();
				for (const auto& arg : args) {
					PrintAstNodeDebug(arg.get(), indent + 1);
				}
				std::wcout << indentStr << L")\n";
				break;
			}
			default:
				std::wcout << indentStr << L"Unknown node type: " << static_cast<int>(node->GetType()) << L"\n";
		}
	}
}
