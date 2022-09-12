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

		void declaration();

		void variableDeclaration();

		void statement();

		void expression();

		void eat(TokenType type, const char* message);

		void finish();

		void grouping();

	private:
		void statementExpression();

		void statementPrint();

	private:
		uint8_t parseVariable(const char* message);

		void defineVariable(uint8_t globalVariable);

	private:
		void synchronize();

	private:
		void emitByte(uint8_t byte);

		void emitConstant(Value value);

	private:
		void numeric(bool canAssign);
		
		void unary(bool canAssign);

		void binary(bool canAssign);

		void literalType(bool canAssign);

		void string(bool canAssign);

		void variable(bool canAssign);

		void namedVariable(Token name, bool canAssign);

		void parsePrecedence(const Precedence& precendece);

	private:
		uint8_t identifierConstant(Token* name);

	private:
		std::string prepareStringObject() const;

	private:
		void intializeParserRules();

		Rule* getParserRule(TokenType type);

	private:
		void handleErrorAtCurrentToken(const std::string& message);

		void handleErrorToken(Token* token, const std::string& message);

	private:
		bool matchToken(TokenType type);

	public:
		Lexer lexer;
		Parser parser;
		Chunk* currentChunk = nullptr;
		YoctaObject* objects = nullptr;

	private:
		std::unordered_map<TokenType, Rule> parseRules;
	};
}