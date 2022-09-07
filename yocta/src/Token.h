#pragma once
#include <string>

namespace yo
{
	struct Token
	{
	public:
		enum class Type
		{
			TY_None = 0,
			TY_IDENTIFIER,
			TY_INT,
			TY_FLOAT,
			TY_DOUBLE,
			TY_TRUE,
			TY_FALSE,
			TY_PLUS_SIGN,
			TY_MINUS_SIGN,
			TY_MULT_SIGN,
			TY_DIV_SIGN,
			TY_LEFT_PARENTH,
			TY_RIGHT_PARENTH,
			TY_Count
		};

	public:
		Token(Type type, std::string text, unsigned int startPosition)
			: type(type), text(text), startPosition(startPosition) { }

	public:
		Type type;
		std::string text;
		unsigned int startPosition;
	};
}