#pragma once
#include <unordered_map>
#include <functional>

#include "YoctaObject.h"
#include "Precedence.h"
#include "LocalVar.h"
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

		void declaration();

		void variableDeclaration();

		void statement();

		void expression();

		void eat(TokenType type, const char* message);

		void finish();

		void grouping();

	private:
		void startScope();

		void scopeBlock();
		
		void endScope();

	private:
		void statementExpression();

		void statementPrint();

		void statementIf();

	private:
		uint8_t parseVariable(const char* message);

		void defineVariable(uint8_t globalVariable);

		void declareVariable();

		void markInitialized();

	private:
		void synchronize();

	private:
		void emitByte(uint8_t byte);

		void emitConstant(Value value);

		int emitJump(uint8_t instruction);

	private:
		void patchJump(int offset);

	private:
		void numeric(bool canAssign);
		
		void unary(bool canAssign);

		void binary(bool canAssign);

		void literalType(bool canAssign);

		void string(bool canAssign);

		void variable(bool canAssign);

		void andRule(bool canAssign)
		{
			int endJump = emitJump((uint8_t)OPCode::OP_JUMP_IF_FALSE);

			emitByte((uint8_t)OPCode::OP_POP_BACK);
			parsePrecedence(Precedence::P_AND);

			patchJump(endJump);
		}

		void orRule(bool canAssign)
		{
			int elseJump = emitJump((uint8_t)OPCode::OP_JUMP_IF_FALSE);
			int endJump = emitJump((uint8_t)OPCode::OP_JUMP);

			patchJump(elseJump);
			emitByte((uint8_t)OPCode::OP_POP_BACK);

			parsePrecedence(Precedence::P_OR);
			patchJump(endJump);
		}

		void namedVariable(Token name, bool canAssign);

		void parsePrecedence(const Precedence& precendece);

	private:
		uint8_t identifierConstant(Token* name);

		int resolveLocal(Token name);

		void addLocal(Token name);

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

		bool checkToken(TokenType type);

	public:
		Lexer lexer;
		Parser parser;
		LocalStack localStack;
		Chunk* currentChunk = nullptr;
		YoctaObject* objects = nullptr;

	private:
		std::unordered_map<TokenType, Rule> parseRules;
	};
}