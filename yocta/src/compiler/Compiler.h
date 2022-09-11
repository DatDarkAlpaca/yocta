#pragma once
#include <unordered_map>
#include <functional>

#include "Precedence.h"
#include "Parser.h"
#include "Lexer.h"
#include "Chunk.h"
#include "Rule.h"
#include "YoctaObject.h"

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

		void declaration()
		{
			statement();
		}

		void statement()
		{
			if (matchToken(TokenType::T_PRINT))
				statementPrint();
			else
				statementExpression();
		}

		void expression();

		void eat(TokenType type, const char* message);

		void finish();

		void grouping();

	private:
		void statementExpression()
		{
			expression();

			eat(TokenType::T_SEMICOLON, "Expected ';' after expression.");

			emitByte((uint8_t)OPCode::OP_POP_BACK);
		}

		void statementPrint()
		{
			expression();

			eat(TokenType::T_SEMICOLON, "Expected ';' after expression.");

			emitByte((uint8_t)OPCode::OP_PRINT);
		}

	private:
		void synchronize()
		{
			parser.panicMode = false;

			while (parser.current.type != TokenType::T_EOF)
			{
				if (parser.previous.type == TokenType::T_SEMICOLON)
					return;
			}
		}

	private:
		void emitByte(uint8_t byte);

		void emitConstant(Value value);

	private:
		void numeric();
		
		void unary();

		void binary();

		void literalType();

		void string();

		void parsePrecedence(const Precedence& precendece);

	private:
		std::string prepareStringObject() const;

		StringObject* allocateStringObject(const std::string& str);

	private:
		void intializeParserRules();

		Rule* getParserRule(TokenType type);

	private:
		void handleErrorAtCurrentToken(const std::string& message);

		void handleErrorToken(Token* token, const std::string& message);

	private:
		bool matchToken(TokenType type)
		{
			if (parser.current.type != type)
				return false;

			advance();

			return true;
		}

	public:
		Lexer lexer;
		Parser parser;
		Chunk* currentChunk = nullptr;
		YoctaObject* objects = nullptr;

	private:
		std::unordered_map<TokenType, Rule> parseRules;
	};
}