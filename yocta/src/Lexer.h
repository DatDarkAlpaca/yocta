#pragma once
#include <string>

namespace yo
{
	/*
	*	String helpers:
	*/
	inline bool endOfStream(const char* stream)
	{
		return *stream == '\0';
	}

	inline bool matches(const char* data, char expected)
	{
		if (endOfStream(data))
			return false;

		if (*data != expected)
			return false;

		data++;
		return true;
	}

	class Token
	{
	public:
		enum class Type
		{
			// Literals:
			T_IDENTIFIER,
			T_STRING,
			T_NUMERIC,
			T_ERROR, T_EOF,

			// Single Character:
			T_LEFT_PARENTHESIS, T_RIGHT_PARENTHESIS,
			T_LEFT_BRACKETS, T_RIGHT_BRACKETS,
			T_LEFT_BRACES, T_RIGHT_BRACES,
			T_COMMA, T_DOT, T_MINUS, T_PLUS,
			T_SEMICOLON, T_SLASH, T_ASTERISTIC,

			// Multi-character:
			T_EXCLAMATION, T_EXCLAMATION_EQUAL,
			T_EQUAL, T_EQUAL_EQUAL,
			T_GREATER, T_GREATER_EQUAL,
			T_LESS, T_LESS_EQUAL,

			// Keywords:
			T_AND, T_OR, T_NONE, T_RETURN,
			T_IF, T_ELSE, T_TRUE, T_FALSE,
			T_WHILE, T_FOR,
			T_VAR, T_FUN, T_CLASS, T_SUPER, T_THIS,

			// Function:
			T_PRINT
		};

	public:
		Token(Type type, const char* start, int length, int line)
			: type(type), start(start), length(length), line(line)
		{

		}

		Token() = default;

	public:
		Type type;
		const char* start;
		int length, line;
	};

	class Lexer
	{
	public:
		Lexer() = default;

		Lexer(const char* source)
		{
			start = source;
			current = start;
			line = 1;
		}

	public:
		void open(const char* source)
		{
			start = source;
			current = start;
			line = 1;
		}

	public:
		inline Token scanToken()
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
					return createToken(matches(current, '=') 
						? Token::Type::T_EXCLAMATION_EQUAL : Token::Type::T_EXCLAMATION);

				case '=':
					return createToken(matches(current, '=')
						? Token::Type::T_EQUAL_EQUAL : Token::Type::T_EQUAL);

				case '<':
					return createToken(matches(current, '=')
						? Token::Type::T_LESS_EQUAL : Token::Type::T_LESS);

				case '>':
					return createToken(matches(current, '=')
						? Token::Type::T_GREATER_EQUAL : Token::Type::T_GREATER);

				case '"':
					return handleString();
			}

			return createErrorToken("Unexpected token");
		}

	private:
		Token createToken(Token::Type type) const
		{
			return { type, start, (int)(current - start), line };
		}

		Token createErrorToken(const char* details) const
		{
			return { Token::Type::T_ERROR, details, (int)(std::strlen(details)), line };
		}

		Token handleString()
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

		Token handleNumeric()
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

		Token handleIdentifier()
		{
			while (std::isalpha(*current) || std::isdigit(*current))
				advance();

			return createToken(getIdentifierType());
		}

		Token::Type getIdentifierType() const
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
				case 't': {
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
				case 'f': {
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
		}

	private:
		char advance()
		{
			++current;
			return current[-1];
		}

		char peekNext()
		{
			if (endOfStream(current))
				return '\0';
			return current[-1];
		}

		void ignoreWhitespace()
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

		Token::Type checkWordEnd(int begin, int length, const char* rest, Token::Type type) const
		{
			if (current - start == begin + length && std::memcmp(start + begin, rest, length) == 0)
				return type;

			return Token::Type::T_IDENTIFIER;
		}

	public:
		const char* start = nullptr, *current = nullptr;
		int line;
	};
}