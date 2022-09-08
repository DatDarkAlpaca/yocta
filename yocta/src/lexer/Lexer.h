#pragma once
#include "StringHelper.h"
#include "Token.h"

namespace yo
{
	class Lexer
	{
	public:
		Lexer() = default;

		Lexer(const char* source);

	public:
		void open(const char* source);

	public:
		Token scanToken();

	private:
		Token createToken(Token::Type type) const;

		Token createErrorToken(const char* details) const;

	private:
		Token handleString();

		Token handleNumeric();

		Token handleIdentifier();

	private:
		Token::Type getIdentifierType() const;

	private:
		char advance();

		char peekNext();

	private:
		void ignoreWhitespace();

		Token::Type checkWordEnd(int begin, int length, const char* rest, Token::Type type) const;

	public:
		const char* start = nullptr, *current = nullptr;
		int line = 1;
	};
}