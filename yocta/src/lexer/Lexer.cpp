#include "Lexer.h"
#include <string>

yo::Lexer::Lexer(const char* source)
{
	start = source;
	current = start;
	line = 1;
}

void yo::Lexer::open(const char* source)
{
	start = source;
	current = start;
	line = 1;
}

yo::Token yo::Lexer::scanToken()
{
	ignoreWhitespace();

	start = current;

	if (endOfStream(current))
		return createToken(Token::Type::T_EOF);

	char character = advance();

	if (std::isdigit(character))
		return handleNumeric();

	if (std::isalpha(character))
		return handleIdentifier();

	switch (character)
	{
		case '(': return createToken(Token::Type::T_LEFT_PARENTHESIS);
		case ')': return createToken(Token::Type::T_RIGHT_PARENTHESIS);
		case '[': return createToken(Token::Type::T_LEFT_BRACKETS);
		case ']': return createToken(Token::Type::T_RIGHT_BRACKETS);
		case '{': return createToken(Token::Type::T_LEFT_BRACES);
		case '}': return createToken(Token::Type::T_RIGHT_BRACES);

		case ';': return createToken(Token::Type::T_SEMICOLON);
		case '.': return createToken(Token::Type::T_DOT);
		case ',': return createToken(Token::Type::T_COMMA);

		case '+': return createToken(Token::Type::T_PLUS);
		case '-': return createToken(Token::Type::T_MINUS);
		case '*': return createToken(Token::Type::T_ASTERISTIC);
		case '/': return createToken(Token::Type::T_SLASH);

		case '!':
		{
			auto token = createToken(matches(current, '=')
				? Token::Type::T_EXCLAMATION_EQUAL : Token::Type::T_EXCLAMATION);
			if (token.type == Token::Type::T_EXCLAMATION_EQUAL)
				current++;
			return token;
		}

		case '=':
		{
			auto token = createToken(matches(current, '=')
				? Token::Type::T_EQUAL_EQUAL : Token::Type::T_EQUAL);
			if (token.type == Token::Type::T_EQUAL_EQUAL)
				current++;
			return token;
		}
		
		case '<':
		{
			auto token = createToken(matches(current, '=')
				? Token::Type::T_LESS_EQUAL : Token::Type::T_LESS);
			if(token.type == Token::Type::T_LESS_EQUAL)
				current++;
			return token;
		}
			
		case '>':
		{
			auto token = createToken(matches(current, '=')
				? Token::Type::T_GREATER_EQUAL : Token::Type::T_GREATER);
			if (token.type == Token::Type::T_GREATER_EQUAL)
				current++;
			return token;
		}

		case '"':
			return handleString();
	}

	return createErrorToken("Unexpected token");
}

yo::Token yo::Lexer::createToken(Token::Type type) const
{
	return { type, start, (int)(current - start), line };
}

yo::Token yo::Lexer::createErrorToken(const char* details) const
{
	return { Token::Type::T_ERROR, details, (int)(std::strlen(details)), line };
}

yo::Token yo::Lexer::handleString()
{
	while (*current != '"' && !endOfStream(current))
	{
		if (*current == '\n')
			line++;

		advance();
	}

	if (endOfStream(current))
		return createErrorToken("Unterminated character literal.");

	advance();
	return createToken(Token::Type::T_STRING);
}

yo::Token yo::Lexer::handleNumeric()
{
	while (std::isdigit(*current))
		advance();

	if (*current == '.' && std::isdigit(peekNext()))
	{
		advance();

		while (std::isdigit(*current))
			advance();
	}

	return createToken(Token::Type::T_NUMERIC);
}

yo::Token yo::Lexer::handleIdentifier()
{
	while (std::isalpha(*current) || std::isdigit(*current))
		advance();

	return createToken(getIdentifierType());
}

yo::Token::Type yo::Lexer::getIdentifierType() const
{
	switch (start[0])
	{
		// and:
		case 'a': return checkWordEnd(1, 2, "nd", Token::Type::T_AND);

		// or:
		case 'o': return checkWordEnd(1, 1, "or", Token::Type::T_OR);

		// none:
		case 'n': return checkWordEnd(1, 3, "one", Token::Type::T_NONE);

		// return:
		case 'r': return checkWordEnd(1, 5, "eturn", Token::Type::T_NONE);

		// if:
		case 'i': return checkWordEnd(1, 1, "f", Token::Type::T_IF);

		// else:
		case 'e': return checkWordEnd(1, 3, "lse", Token::Type::T_ELSE);

		// this | true:
		case 't': 
		{
			if (current - start > 1)
			{
				switch (start[1])
				{
				case 'h': return checkWordEnd(2, 2, "is", Token::Type::T_THIS);
				case 'r': return checkWordEnd(2, 2, "ue", Token::Type::T_TRUE);
				}
			}
			break;
		}

		// while:
		case 'w': return checkWordEnd(1, 4, "hile", Token::Type::T_WHILE);

		// false | for | fun:
		case 'f': 
		{
			if (current - start > 1)
			{
				switch (start[1])
				{
					case 'a': return checkWordEnd(2, 3, "lse", Token::Type::T_FALSE);
					case 'o': return checkWordEnd(2, 1, "r", Token::Type::T_FOR);
					case 'u': return checkWordEnd(2, 1, "n", Token::Type::T_FUN);
				}
			}
		break;
		}

		// var:
		case 'v': return checkWordEnd(1, 2, "ar", Token::Type::T_VAR);

		// class:
		case 'c': return checkWordEnd(1, 4, "lass", Token::Type::T_CLASS);

		// super:
		case 's': return checkWordEnd(1, 4, "uper", Token::Type::T_SUPER);

		// print:
		case 'p': return checkWordEnd(1, 4, "rint", Token::Type::T_PRINT);
	}

	return {};
}

char yo::Lexer::advance()
{
	++current;
	return current[-1];
}

char yo::Lexer::peekNext()
{
	if (endOfStream(current))
		return '\0';

	return current[-1];
}

void yo::Lexer::ignoreWhitespace()
{
	while (true)
	{
		char peekChar = *current;

		if (peekChar == '\n')
		{
			line++;
			advance();
			break;
		}

		// End-of-line comments:
		if (peekNext() == '\\')
		{
			while (*current != '\n' && endOfStream(current))
				advance();
		}

		if (std::isspace(peekChar))
			advance();

		else
			return;
	}
}

yo::Token::Type yo::Lexer::checkWordEnd(int begin, int length, const char* rest, yo::Token::Type type) const
{
	if (current - start == begin + length && std::memcmp(start + begin, rest, length) == 0)
		return type;

	return Token::Type::T_IDENTIFIER;
}
