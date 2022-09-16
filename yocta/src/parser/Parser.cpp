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

	expression();

	return m_Results;
}

void yo::Parser::expression()
{
	parsePrecedence(Precedence::P_ASSIGNMENT);
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

	prefix();

	while (precedence <= getRule(m_CurrentToken.getValue()).precedence)
	{
		advance();
		rule_type infix = getRule(m_PreviousToken.getValue()).infix;
		infix();
	}
}

bool yo::Parser::matchesToken(YoctaValue value)
{
	if (m_CurrentToken.getValue() != value)
		return false;

	advance();
	return true;
}

void yo::Parser::handleGrouping()
{
	expression();
	consumeToken({ ReservedToken::T_CLOSE_PARENTHESIS }, "Missing ')'");
}

void yo::Parser::handleUnary()
{
	YoctaValue unaryOperator = m_PreviousToken.getValue();
	if (!unaryOperator.isReservedToken())
		throw new ParserError("Invalid unary operator", m_CurrentToken.getLineNumber(), m_PreviousToken.getCharIndex());

	parsePrecedence(Precedence::P_UNARY);

	switch (unaryOperator.getReservedToken())
	{
	case ReservedToken::T_SUB:
		m_Results.push_back({ ExpressionType::EXPR_OPERATION, Identifier("-")});
		break;
	}
}

void yo::Parser::handleBinary()
{
	YoctaValue binaryOperator = m_PreviousToken.getValue();
	if (!binaryOperator.isReservedToken())
		throw new ParserError("Invalid unary operator", m_CurrentToken.getLineNumber(), m_PreviousToken.getCharIndex());

	Rule rule = getRule(binaryOperator);

	parsePrecedence((Precedence)((int)rule.precedence + 1));

	switch (binaryOperator.getReservedToken())
	{
		case ReservedToken::T_ADD:
			m_Results.push_back({ ExpressionType::EXPR_OPERATION, ReservedToken::T_ADD });
			break;
		case ReservedToken::T_SUB:
			m_Results.push_back({ ExpressionType::EXPR_OPERATION, ReservedToken::T_SUB });
			break;
		case ReservedToken::T_MULT:
			m_Results.push_back({ ExpressionType::EXPR_OPERATION, ReservedToken::T_MULT });
			break;
		case ReservedToken::T_DIV:
			m_Results.push_back({ ExpressionType::EXPR_OPERATION, ReservedToken::T_DIV });
			break;
		case ReservedToken::T_MOD:
			m_Results.push_back({ ExpressionType::EXPR_OPERATION,  ReservedToken::T_MOD });
			break;
	}
}

void yo::Parser::handleLiteral()
{
	switch (m_PreviousToken.getValue().getReservedToken())
	{
		case ReservedToken::T_TRUE:
			m_Results.push_back({ ExpressionType::EXPR_CONSTANT, true });
			break;

		case ReservedToken::T_FALSE:
			m_Results.push_back({ ExpressionType::EXPR_CONSTANT, false });
			break;

		case ReservedToken::T_NONE:
			m_Results.push_back({ ExpressionType::EXPR_CONSTANT, false });
			break;
	}
}

void yo::Parser::handleNumber()
{
	m_Results.push_back({ ExpressionType::EXPR_CONSTANT, m_PreviousToken.getValue().getNumber() });
}

void yo::Parser::handleString()
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

void yo::Parser::consumeToken(YoctaValue value, std::string errorMessage)
{
	if (m_CurrentToken.getValue() == value)
		return advance();

	throw new ParserError(errorMessage, m_CurrentToken.getLineNumber(), m_CurrentToken.getCharIndex());
}

yo::Rule yo::Parser::getRule(YoctaValue value)
{
	if (value.isReservedToken())
		return reservedRules.at(value.getReservedToken());

	else if (value.isIdentifier())
		return {};

	else if (value.isString())
		return { std::bind(&Parser::handleString, this), nullptr, Precedence::P_NONE };

	else if (value.isNumber())
		return { std::bind(&Parser::handleNumber, this), nullptr, Precedence::P_NONE };

	else if (value.isEOF())
		return { nullptr, nullptr, Precedence::P_NONE };

	return {};
}
