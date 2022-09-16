#pragma once
#include "InputStream.h"
#include "Token.h"
#include "Lexer.h"

namespace yo
{
	inline void disassembleLexOperation(Token token, size_t index)
	{
		using namespace yo;
		auto value = token.getValue();

		if (value.isEOF())
			return;

		printf("ID: %d\t", (int)index);

		printf("%s\t", translateYoctaValue(value).c_str());

		printf("Line: %d | ", (int)token.getLineNumber());
		printf("CI: %d\n", (int)token.getCharIndex());
	}

	inline void debugLexerStream(InputStream& stream)
	{
		printf("-=> Stream Tokens : Lexer Results <=-\n");

		size_t index = 0;
		for (Token token = lex(stream); !token.getValue().isEOF(); token = lex(stream))
		{
			disassembleLexOperation(token, index);
			index++;
		}
	}
}