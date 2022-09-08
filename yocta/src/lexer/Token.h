#pragma once

namespace yo
{
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
		const char* start;
		int length, line;
		Type type;
	};
}