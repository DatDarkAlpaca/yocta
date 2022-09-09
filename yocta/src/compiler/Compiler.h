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

		void expression();

		void eat(Token::Type type, const char* message);

		void finish();

		void grouping();

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

		Rule* getParserRule(Token::Type type);

	private:
		void handleErrorAtCurrentToken(const char* message);

		void handleErrorToken(Token* token, const char* message);

	public:
		Lexer lexer;
		Parser parser;
		Chunk* currentChunk = nullptr;
		YoctaObject* objects = nullptr;

	private:
		std::unordered_map<Token::Type, Rule> parseRules;
	};
}