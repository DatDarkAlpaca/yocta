#pragma once
#include <string>

namespace yo
{
	enum class TokenType
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
		T_PIPE, T_AMPERSTAND,

		// Multi-character:
		T_EXCLAMATION, T_EXCLAMATION_EQUAL,
		T_EQUAL, T_EQUAL_EQUAL,
		T_GREATER, T_GREATER_EQUAL,
		T_LESS, T_LESS_EQUAL,

		// Keywords:
		T_AND, T_OR, T_NONE, T_RETURN,
		T_IF, T_ELSE, T_TRUE, T_FALSE,
		T_WHILE, T_FOR,
		T_VAR, T_FUNC, T_CLASS, T_SUPER, T_THIS,

		// Function:
		T_PRINT
	};

	class Token
	{
	public:
		Token(std::string data, TokenType type, unsigned int line)
			: type(type), data(data), line(line) { }

		Token() = default;

	public:
		TokenType type = TokenType::T_NONE;
		std::string data;
		unsigned int line = 0;

	public:
		friend bool operator==(const Token& lhs, const Token& rhs);
	};

	inline bool operator==(const Token& lhs, const Token& rhs)
	{
		if (lhs.type != rhs.type)
			return false;

		if (lhs.line != rhs.line)
			return false;

		return lhs.data == rhs.data;
	}
}