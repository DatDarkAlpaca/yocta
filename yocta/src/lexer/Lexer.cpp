#include "Lexer.h"
#include <string>

yo::Lexer::Lexer(const char* source)
	: m_Source(source)
{
}

void yo::Lexer::open(const char* source)
{
	m_Source = source;
}

yo::Token yo::Lexer::nextToken()
{
	while (std::isspace(peek()))
		nextCharacter();

	if (peek() == '\0')
		return createToken("\0", TokenType::T_EOF);

	if (std::isalpha(peek()))
		return handleIdentifier();

	if (std::isdigit(peek()))
		return handleNumeric();

	if (peek() == '"')
		return handleString();

	if (validSymbol(peek()))
		return handleSymbol(peek());

	return createToken({}, TokenType::T_NONE);
}

yo::Token yo::Lexer::handleNumeric()
{
	const char* start = m_Source;

	nextCharacter();
	while (std::isdigit(peek()))
		nextCharacter();

	std::string tokenData;
	tokenData.assign(start, m_Source - start);

	return createToken(tokenData, TokenType::T_NUMERIC);
}

yo::Token yo::Lexer::handleIdentifier()
{
	const char* start = m_Source;

	nextCharacter();

	while (validIdentifier(peek()))
		nextCharacter();

	std::string tokenData;
	tokenData.assign(start, m_Source - start);

	return createToken(tokenData, getIdentifierType(tokenData));
}

yo::Token yo::Lexer::handleSymbol(char symbol)
{
	nextCharacter();

	switch (symbol)
	{
		case '(': return createToken("(", TokenType::T_LEFT_PARENTHESIS);
		case ')': return createToken(")", TokenType::T_RIGHT_PARENTHESIS);
		case '[': return createToken("[", TokenType::T_LEFT_BRACKETS);
		case ']': return createToken("]", TokenType::T_RIGHT_BRACKETS);
		case '{': return createToken("{", TokenType::T_LEFT_BRACES);
		case '}': return createToken("}", TokenType::T_RIGHT_BRACES);

		case ';': return createToken(";", TokenType::T_SEMICOLON);
		case '.': return createToken(".", TokenType::T_DOT);
		case ',': return createToken(",", TokenType::T_COMMA);

		case '+': return createToken("+", TokenType::T_PLUS);
		case '-': return createToken("-", TokenType::T_MINUS);
		case '*': return createToken("*", TokenType::T_ASTERISTIC);
		case '/': return createToken("/", TokenType::T_SLASH);

		case '&': return createToken("&", TokenType::T_AMPERSTAND);
		case '|': return createToken("|", TokenType::T_PIPE);

		case '!':
		{
			TokenType type = matchesNext('=') ? TokenType::T_EXCLAMATION_EQUAL : TokenType::T_EXCLAMATION;
			const char* symbol = matchesNext('=') ? "!=" : "!";
			nextCharacter();
			return createToken(symbol, type);
		}

		case '=':
		{
			TokenType type = matchesNext('=') ? TokenType::T_EQUAL_EQUAL : TokenType::T_EQUAL;
			const char* symbol = matchesNext('=') ? "==" : "=";
			nextCharacter();
			return createToken(symbol, type);
		}

		case '>':
		{
			TokenType type = matchesNext('=') ? TokenType::T_GREATER_EQUAL : TokenType::T_GREATER;
			const char* symbol = matchesNext('=') ? ">=" : ">";
			nextCharacter();
			return createToken(symbol, type);
		}
		case '<':
		{
			TokenType type = matchesNext('=') ? TokenType::T_LESS_EQUAL : TokenType::T_LESS;
			const char* symbol = matchesNext('=') ? "<=" : "<";
			nextCharacter();
			return createToken(symbol, type);
		}
	}
}

yo::Token yo::Lexer::handleString()
{
	nextCharacter();

	const char* start = m_Source;
	while (peek() != '"' && peek() != '\0')
	{
		if (peek() == '\n')
			m_Line++;

		nextCharacter();
	}

	std::string tokenString;
	tokenString.assign(start, m_Source - start);

	nextCharacter();

	return { tokenString, TokenType::T_STRING, m_Line };
}

bool yo::Lexer::validSymbol(char symbol) const
{
	// TODO: optimize using binary search and a sorted list of symbols.
	return std::find(m_ValidSymbols.begin(), m_ValidSymbols.end(), symbol) != m_ValidSymbols.end();
}

bool yo::Lexer::validIdentifier(char symbol) const
{
	return std::isalnum(symbol) || symbol == '_';
}

bool yo::Lexer::matchesNext(char expected)
{
	if (*m_Source == '\0')
		return false;

	return peek() == expected;
}

yo::TokenType yo::Lexer::getIdentifierType(const std::string& identifier) const
{
	if (identifierTable.find(identifier) == identifierTable.end())
		return TokenType::T_IDENTIFIER;

	return identifierTable[identifier];
}
