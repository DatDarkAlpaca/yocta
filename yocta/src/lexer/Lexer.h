#pragma once
#include "StringHelper.h"
#include "Token.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace yo
{
	class Lexer
	{
	public:
		explicit Lexer(const char* source);

		Lexer() = default;

	public:
		void open(const char* source);

	public:
		Token nextToken();

	private:
		Token createToken(const std::string& data, TokenType type) const { return { data, type, m_Line }; }

		Token createErrorToken(const char* details) const { return { details, TokenType::T_ERROR, m_Line }; }

	private:
		Token handleNumeric();

		Token handleIdentifier();

		Token handleSymbol(char symbol);

		Token handleString();

	private:
		bool validSymbol(char symbol) const;

		bool validIdentifier(char symbol) const;

		inline bool matchesNext(char expected);

	private:
		TokenType getIdentifierType(const std::string& identifier) const;

	private:
		inline char peek() const { return *m_Source; }

		char nextCharacter() { return *m_Source++; }

	public:
		const char* m_Source = nullptr;
		unsigned int m_Line = 1;

	private:
		inline static std::vector<char> m_ValidSymbols = {
			'(', ')', '[', ']', '{', '}',
			';', '.', ',',
			'+', '-', '*', '/',
			'!', '=',
			'>', '<',
			'|', '&'
		};

		inline static std::unordered_map<std::string, TokenType> identifierTable = {
			{ "and", TokenType::T_AND },
			{ "or", TokenType::T_OR },
			{ "none", TokenType::T_NONE },

			{ "var", TokenType::T_VAR },

			{ "func", TokenType::T_FUNC },
			{ "return", TokenType::T_RETURN },

			{ "if", TokenType::T_IF },
			{ "else", TokenType::T_ELSE },
			{ "this", TokenType::T_THIS },

			{ "true", TokenType::T_TRUE },
			{ "false", TokenType::T_FALSE },

			{ "while", TokenType::T_WHILE },
			{ "for", TokenType::T_FOR },

			{ "class", TokenType::T_CLASS },
			{ "super", TokenType::T_SUPER },

			{ "print", TokenType::T_PRINT },
		};
	};
}