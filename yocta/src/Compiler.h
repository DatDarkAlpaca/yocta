#pragma once
#include "Lexer.h"

namespace yo
{
	struct Parser
	{
		Token current, previous;
		bool errorFound = false, panicMode;
	};

	class Compiler
	{
	public:
		bool compile(const char* source, Chunk* chunk)
		{
			lexer.open(source);
			currentChunk = chunk;

			parser.errorFound = false;
			parser.panicMode = false;

			advance();

			// expr();

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
			emitByte((uint8_t)Token::Type::T_RETURN);
		}

	private:
		void emitByte(uint8_t byte)
		{
			currentChunk->push_back(byte, parser.previous.line);
		}

		void emitBytes(uint8_t first, uint8_t second)
		{
			emitByte(first);
			emitByte(second);
		}

		void emitConstant(yocta_value value)
		{
			emitBytes((uint8_t)OPCode::OP_CONSTANT, createConstant(value));
		}

	private:
		uint8_t createConstant(yocta_value value)
		{
			currentChunk->push_back(value, parser.previous.line);
		}

	private:
		void numeric()
		{
			double value = std::strtod(parser.previous.start, NULL);
			emitConstant(value);
		}

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
	};
}