#pragma once
#include "Lexer.h"
#include <unordered_map>
#include <functional>

namespace yo
{
	struct Parser
	{
		Token current = {}, previous = {};
		bool errorFound = false, panicMode = false;
	};

	enum class Precedence
	{
		P_NONE,
		P_ASSIGNMENT,
		P_OR,
		P_AND,
		P_EQUAL,
		P_COMPARE,
		P_TERM,
		P_FACTOR,
		P_UNARY,
		P_CALL,
		P_PRIMARY
	};

	struct ParseRule
	{
	public:
		ParseRule(std::function<void()> prefix, std::function<void()> infix, Precedence precedence)
			: prefix(prefix), infix(infix), precedence(precedence)
		{

		}	

		ParseRule() = default;

	public:
		std::function<void()> prefix;
		std::function<void()> infix;
		Precedence precedence;
	};

	class Compiler
	{
	public:
		Compiler()
		{
			intializeParserRules();
		}

	public:
		bool compile(const char* source, Chunk* chunk)
		{
			lexer.open(source);
			currentChunk = chunk;

			parser.errorFound = false;
			parser.panicMode = false;

			advance();

			expression();

			eat(Token::Type::T_EOF, "Expected end of expression.");

			finish();

			return !parser.errorFound;
		}

	private:
		void advance()
		{
			parser.previous = parser.current;

			while (true)
			{
				parser.current = lexer.scanToken();

				if (parser.current.type != Token::Type::T_ERROR)
					break;

				handleErrorAtCurrentToken(parser.current.start);
			}
		}

		void expression()
		{
			parsePrecedence(Precedence::P_ASSIGNMENT);
		}

		void eat(Token::Type type, const char* message)
		{
			if (parser.current.type == type)
			{
				advance();
				return;
			}

			handleErrorAtCurrentToken(message);
		}

		void finish()
		{
			emitByte((uint8_t)OPCode::OP_RETURN);

#ifdef DEBUG_PRINT_SOURCE
			if (!parser.errorFound)
				Disassembler::disassemble(*currentChunk, "Compiler");
#endif
		}

		void grouping()
		{
			expression();
			eat(Token::Type::T_RIGHT_PARENTHESIS, "Expected ')' after expression.");
		}

	private:
		void emitByte(uint8_t byte)
		{
			currentChunk->push_back(byte, parser.previous.line);
		}

		void emitConstant(yocta_value value)
		{
			emitByte((uint8_t)OPCode::OP_CONSTANT);
			currentChunk->push_constant(value, parser.previous.line);
		}

	private:
		void numeric()
		{
			double value = std::strtod(parser.previous.start, NULL);
			emitConstant(value);
		}
		
		void unary()
		{
			Token::Type type = parser.previous.type;

			parsePrecedence(Precedence::P_UNARY);

			switch (type)
			{
			case Token::Type::T_MINUS:
				emitByte((uint8_t)OPCode::OP_NEGATE);
				break;
			}
		}

		void binary()
		{
			Token::Type type = parser.previous.type;

			ParseRule* rule = getParserRule(type);

			parsePrecedence((Precedence)((int)rule->precedence + 1));

			switch (type)
			{
				case Token::Type::T_PLUS:
					emitByte((uint8_t)OPCode::OP_ADD);
					break;
				case Token::Type::T_MINUS:
					emitByte((uint8_t)OPCode::OP_SUB);
					break;
				case Token::Type::T_ASTERISTIC:
					emitByte((uint8_t)OPCode::OP_MULT);
					break;
				case Token::Type::T_SLASH:
					emitByte((uint8_t)OPCode::OP_DIV);
					break;
			}
		}

		void parsePrecedence(const Precedence& precendece)
		{
			advance();
			Token::Type type = parser.previous.type;
			std::function<void()> prefix = getParserRule(type)->prefix;

			if (!prefix)
			{
				handleErrorAtCurrentToken("Expected expression");
				return;
			}

			prefix();

			while(precendece <= getParserRule(parser.current.type)->precedence)
			{
				advance();

				std::function<void()> infix = getParserRule(parser.previous.type)->infix;
				infix();
			}
		}

	private:
		/* Todo: optimize */
		void intializeParserRules()
		{
			parserRules.insert({
				Token::Type::T_LEFT_PARENTHESIS,
				ParseRule(std::bind(&Compiler::grouping, this), nullptr, Precedence::P_NONE) 
			});

			parserRules.insert({
				Token::Type::T_RIGHT_PARENTHESIS,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_LEFT_BRACES,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_RIGHT_BRACES,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_COMMA,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_DOT,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_MINUS,
				ParseRule(std::bind(&Compiler::unary, this), std::bind(&Compiler::binary, this), Precedence::P_TERM)
			});

			parserRules.insert({
				Token::Type::T_PLUS,
				ParseRule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_TERM)
			});

			parserRules.insert({
				Token::Type::T_SLASH,
				ParseRule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_FACTOR)
			});

			parserRules.insert({
				Token::Type::T_ASTERISTIC,
				ParseRule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_FACTOR)
			});

			parserRules.insert({
				Token::Type::T_SEMICOLON,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_EXCLAMATION,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_EXCLAMATION_EQUAL,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_EQUAL,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_EQUAL_EQUAL,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_GREATER,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_GREATER_EQUAL,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_LESS,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({	
				Token::Type::T_LESS_EQUAL,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_IDENTIFIER,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_STRING,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_NUMERIC,
				ParseRule(std::bind(&Compiler::numeric, this), nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_AND,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_OR,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_IF,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_ELSE,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_FALSE,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_TRUE,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({	
				Token::Type::T_FOR,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_WHILE,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_NONE,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_PRINT,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_VAR,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_FUN,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_RETURN,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_CLASS,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_SUPER,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_THIS,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_ERROR,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});

			parserRules.insert({
				Token::Type::T_EOF,
				ParseRule(nullptr, nullptr, Precedence::P_NONE)
			});
		}

		ParseRule* getParserRule(Token::Type type) { return &parserRules[type]; }

	private:
		void handleErrorAtCurrentToken(const char* message)
		{
			handleErrorToken(&parser.current, message);
		}

		void handleErrorToken(Token* token, const char* message)
		{
			if (parser.panicMode)
				return;

			parser.panicMode = true;

			fprintf(stderr, "<Line %d> Error ", token->line);

			if(token->type == Token::Type::T_EOF)
				fprintf(stderr, "at the end of the file");

			else if (token->type == Token::Type::T_ERROR) { }

			else
				fprintf(stderr, "at '%.*s'", token->length, token->start);

			fprintf(stderr, ": %s\n", message);
			parser.errorFound = true;
		}

	public: // TODO: ponder whether this should be public.
		Lexer lexer;
		Parser parser;
		Chunk* currentChunk = nullptr;

	private:
		std::unordered_map<Token::Type, ParseRule> parserRules;
	};
}