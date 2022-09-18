#include "lexerDisassembler.h"
#include "Parser.h"
#include "Debug.h"

yo::Parser::Parser(InputStream& stream)
	: m_Stream(stream)
{
}

std::vector<yo::Expression> yo::Parser::parse()
{
#ifdef DEBUG_LEXER_TRACE
	printf("-=> Stream Tokens : Lexer Results <=-\n");
#endif

	advance();

	while (!m_CurrentToken.getValue().isEOF())
		declaration();

	finishParser();

	return m_Results;
}

void yo::Parser::declaration()
{
	if (matchesToken(ReservedToken::T_VAR))
		variableDeclaration();
	else
		statement();
}

void yo::Parser::variableDeclaration()
{
	consumeIdentifier("Expected variable name");
	std::string variableName = m_PreviousToken.getValue().getIdentifier().name;

	if (matchesToken(ReservedToken::T_ASSIGN))
		expression();
	else
		m_Results.push_back({ ExpressionType::EXPR_CONSTANT, { NULL_TYPE()} });

	consumeToken(ReservedToken::T_SEMICOLON, "Expected ';'");

	m_Results.push_back({ ExpressionType::EXPR_DEF_GLOBAL_VAR, variableName });
}

void yo::Parser::statement()
{
	if (matchesToken(ReservedToken::T_PRINT))
		printStatement();
	else
		expressionStatement();
}

void yo::Parser::printStatement()
{
	consumeToken(ReservedToken::T_OPEN_PARENTHESIS, "Expected '('");

	expression();

	consumeToken(ReservedToken::T_CLOSE_PARENTHESIS, "Expected ')'");
	
	consumeToken(ReservedToken::T_SEMICOLON, "Expected ';'");

	m_Results.push_back({ ExpressionType::EXPR_OPERATION, ReservedToken::T_PRINT });
}

void yo::Parser::expressionStatement()
{
	expression();

	consumeToken(ReservedToken::T_SEMICOLON, "Expected ';'");

	m_Results.push_back({ ExpressionType::EXPR_POP });
}

void yo::Parser::expression()
{
	parsePrecedence(Precedence::P_ASSIGNMENT);
}

void yo::Parser::finishParser()
{
	m_Results.push_back({ ExpressionType::EXPR_OPERATION, ReservedToken::T_NONE });
}

void yo::Parser::parsePrecedence(Precedence precedence)
{
	advance();
	rule_type prefix = getRule(m_PreviousToken.getValue()).prefix;

	if (!prefix)
	{
		throw new ParserError("Expected expression", m_CurrentToken.getLineNumber(), m_CurrentToken.getCharIndex());
		return;
	}

	bool canAssign = precedence <= Precedence::P_ASSIGNMENT;
	prefix(canAssign);

	while (precedence <= getRule(m_CurrentToken.getValue()).precedence)
	{
		advance();
		rule_type infix = getRule(m_PreviousToken.getValue()).infix;
		infix(canAssign);

		if (canAssign && matchesToken(ReservedToken::T_ASSIGN))
			throw "Invalid assignment value.";
	}
}

bool yo::Parser::matchesToken(ReservedToken value)
{
	if (!m_CurrentToken.getValue().isReservedToken())
		return false;

	if (m_CurrentToken.getValue().getReservedToken() != value)
		return false;

	advance();
	return true;
}

void yo::Parser::handleGrouping()
{
	expression();
	consumeToken({ ReservedToken::T_CLOSE_PARENTHESIS }, "Missing ')'");
}

void yo::Parser::handleVariable(bool canAssign)
{
	std::string arg = m_PreviousToken.getValue().getIdentifier().name;

	if (canAssign && matchesToken(ReservedToken::T_ASSIGN))
	{
		expression();
		m_Results.push_back({ ExpressionType::EXPR_SET_GLOBAL_VAR, arg });
	}
	else
		m_Results.push_back({ ExpressionType::EXPR_GET_GLOBAL_VAR, arg });
}

void yo::Parser::handleUnary(bool canAssign)
{
	YoctaValue unaryOperator = m_PreviousToken.getValue();
	if (!unaryOperator.isReservedToken())
		throw new ParserError("Invalid unary operator", m_CurrentToken.getLineNumber(), m_PreviousToken.getCharIndex());

	parsePrecedence(Precedence::P_UNARY);

	m_Results.push_back({ ExpressionType::EXPR_OPERATION, unaryOperator.getReservedToken() });
}

void yo::Parser::handleBinary(bool canAssign)
{
	YoctaValue binaryOperator = m_PreviousToken.getValue();
	if (!binaryOperator.isReservedToken())
		throw new ParserError("Invalid unary operator", m_CurrentToken.getLineNumber(), m_PreviousToken.getCharIndex());

	Rule rule = getRule(binaryOperator);

	parsePrecedence((Precedence)((int)rule.precedence + 1));

	m_Results.push_back({ ExpressionType::EXPR_OPERATION, binaryOperator.getReservedToken() });
}

void yo::Parser::handleLiteral(bool canAssign)
{
	switch (m_PreviousToken.getValue().getReservedToken())
	{
		case ReservedToken::T_TRUE:
			m_Results.push_back({ ExpressionType::EXPR_CONSTANT, { true } });
			break;

		case ReservedToken::T_FALSE:
			m_Results.push_back({ ExpressionType::EXPR_CONSTANT, { false } });
			break;

		case ReservedToken::T_NONE:
			m_Results.push_back({ ExpressionType::EXPR_CONSTANT, { false } });
			break;
	}
}

void yo::Parser::handleNumber(bool canAssign)
{
	m_Results.push_back({ ExpressionType::EXPR_CONSTANT, m_PreviousToken.getValue().getNumber() });
}

void yo::Parser::handleString(bool canAssign)
{
	m_Results.push_back({ ExpressionType::EXPR_CONSTANT, m_PreviousToken.getValue().getString() });
}

void yo::Parser::advance()
{
	m_PreviousToken = m_CurrentToken;
	m_CurrentToken = lex(m_Stream);

#ifdef DEBUG_LEXER_TRACE
	disassembleLexOperation(m_CurrentToken, 0);
#endif
}

void yo::Parser::consumeToken(ReservedToken token, std::string errorMessage)
{
	if (!m_CurrentToken.getValue().isReservedToken())
		throw "wtf";

	if (m_CurrentToken.getValue().getReservedToken() == token)
		return advance();

	throw new ParserError(errorMessage, m_CurrentToken.getLineNumber(), m_CurrentToken.getCharIndex());
}

void yo::Parser::consumeIdentifier(std::string errorMessage)
{
	if (!m_CurrentToken.getValue().isIdentifier())
		throw "Invalid identifier";

	advance();
}

yo::Rule yo::Parser::getRule(YoctaValue value)
{
	if (value.isReservedToken())
		return reservedRules.at(value.getReservedToken());

	else if (value.isIdentifier())
		return { std::bind(&Parser::handleVariable, this, true), nullptr, Precedence::P_NONE };

	else if (value.isString())
		return { std::bind(&Parser::handleString, this, true), nullptr, Precedence::P_NONE };

	else if (value.isNumber())
		return { std::bind(&Parser::handleNumber, this, true), nullptr, Precedence::P_NONE };

	else if (value.isEOF())
		return { };

	return {};
}
