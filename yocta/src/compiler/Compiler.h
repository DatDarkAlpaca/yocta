#pragma once
#include <unordered_map>
#include <functional>

#include "Precedence.h"
#include "Parser.h"
#include "Lexer.h"
#include "Chunk.h"
#include "Rule.h"

namespace yo
{
	class Compiler
	{
	public:
		Compiler();

	public:
		bool compile(const char* source, Chunk* chunk);

	private:
		void advance();

		void expression();

		void eat(Token::Type type, const char* message);

		void finish();

		void grouping();

	private:
		void emitByte(uint8_t byte);

		void emitConstant(yocta_value value);

	private:
		void numeric();
		
		void unary();

		void binary();

		void literalType()
		{
			switch (parser.previous.type)
			{
				case Token::Type::T_NONE:
					emitByte((uint8_t)OPCode::OP_NONE);
					break;
				case Token::Type::T_TRUE:
					emitByte((uint8_t)OPCode::OP_TRUE);
					break;
				case Token::Type::T_FALSE:
					emitByte((uint8_t)OPCode::OP_FALSE);
					break;
			}
		}

		void parsePrecedence(const Precedence& precendece);

	private:
		void intializeParserRules();

		Rule* getParserRule(Token::Type type);

	private:
		void handleErrorAtCurrentToken(const char* message);

		void handleErrorToken(Token* token, const char* message);

	public:
		Lexer lexer;
		Parser parser;
		Chunk* currentChunk = nullptr;

	private:
		std::unordered_map<Token::Type, Rule> parseRules;
	};
}