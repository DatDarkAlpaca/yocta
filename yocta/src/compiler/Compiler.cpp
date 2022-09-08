#include "Debug.h"
#include "Compiler.h"
#include "Disassembler.h"

yo::Compiler::Compiler()
{
	intializeParserRules();
}

// TODO: return a chunk rather than receiving a chunk pointer.
bool yo::Compiler::compile(const char* source, Chunk* chunk)
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

void yo::Compiler::advance()
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

void yo::Compiler::expression()
{
	parsePrecedence(Precedence::P_ASSIGNMENT);
}

void yo::Compiler::eat(Token::Type type, const char* message)
{
	if (parser.current.type == type)
	{
		advance();
		return;
	}

	handleErrorAtCurrentToken(message);
}

void yo::Compiler::finish()
{
	emitByte((uint8_t)OPCode::OP_RETURN);

	#ifdef DEBUG_PRINT_SOURCE
	if (!parser.errorFound)
		Disassembler::disassemble(*currentChunk, "Compiler");
	#endif
}

void yo::Compiler::grouping()
{
	expression();
	eat(Token::Type::T_RIGHT_PARENTHESIS, "Expected ')' after expression.");
}

void yo::Compiler::emitByte(uint8_t byte)
{
	currentChunk->push_back(byte, parser.previous.line);
}

void yo::Compiler::emitConstant(yocta_value value)
{
	emitByte((uint8_t)OPCode::OP_CONSTANT);
	currentChunk->push_constant(value, parser.previous.line);
}

void yo::Compiler::numeric()
{
	double value = std::strtod(parser.previous.start, NULL);

	Value v{ ValueType::VT_NUMERIC, value };
	emitConstant(std::get<double>(v.variantValue));
}

void yo::Compiler::unary()
{
	Token::Type type = parser.previous.type;

	parsePrecedence(Precedence::P_UNARY);

	switch (type)
	{
	case Token::Type::T_MINUS:
		emitByte((uint8_t)OPCode::OP_NEGATE);
		break;
	case Token::Type::T_EXCLAMATION:
		emitByte((uint8_t)OPCode::OP_NOT);
		break;
	}
}

void yo::Compiler::binary()
{
	Token::Type type = parser.previous.type;

	Rule* rule = getParserRule(type);

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

void yo::Compiler::parsePrecedence(const Precedence& precendece)
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

	while (precendece <= getParserRule(parser.current.type)->precedence)
	{
		advance();

		std::function<void()> infix = getParserRule(parser.previous.type)->infix;
		infix();
	}
}

void yo::Compiler::intializeParserRules()
{
	parseRules.insert({
		Token::Type::T_LEFT_PARENTHESIS,
		Rule(std::bind(&Compiler::grouping, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		Token::Type::T_RIGHT_PARENTHESIS,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_LEFT_BRACES,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_RIGHT_BRACES,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_COMMA,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_DOT,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_MINUS,
		Rule(std::bind(&Compiler::unary, this), std::bind(&Compiler::binary, this), Precedence::P_TERM)
		});

	parseRules.insert({
		Token::Type::T_PLUS,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_TERM)
		});

	parseRules.insert({
		Token::Type::T_SLASH,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_FACTOR)
		});

	parseRules.insert({
		Token::Type::T_ASTERISTIC,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_FACTOR)
		});

	parseRules.insert({
		Token::Type::T_SEMICOLON,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_EXCLAMATION,
		Rule(std::bind(&Compiler::unary, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		Token::Type::T_EXCLAMATION_EQUAL,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_EQUAL,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_EQUAL_EQUAL,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_GREATER,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_GREATER_EQUAL,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_LESS,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_LESS_EQUAL,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_IDENTIFIER,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_STRING,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_NUMERIC,
		Rule(std::bind(&Compiler::numeric, this), nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_AND,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_OR,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_IF,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_ELSE,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_FALSE,
		Rule(std::bind(&Compiler::literalType, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		Token::Type::T_TRUE,
		Rule(std::bind(&Compiler::literalType, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		Token::Type::T_FOR,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_WHILE,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_NONE,
		Rule(std::bind(&Compiler::literalType, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		Token::Type::T_PRINT,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_VAR,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_FUN,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_RETURN,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_CLASS,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_SUPER,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_THIS,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_ERROR,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		Token::Type::T_EOF,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});
}

yo::Rule* yo::Compiler::getParserRule(Token::Type type)
{
	return &parseRules[type];
}

void yo::Compiler::handleErrorAtCurrentToken(const char* message)
{
	handleErrorToken(&parser.current, message);
}

void yo::Compiler::handleErrorToken(Token* token, const char* message)
{
	if (parser.panicMode)
		return;

	parser.panicMode = true;

	fprintf(stderr, "<Line %d> Error ", token->line);

	if (token->type == Token::Type::T_EOF)
		fprintf(stderr, "at the end of the file");

	else if (token->type == Token::Type::T_ERROR) {}

	else
		fprintf(stderr, "at '%.*s'", token->length, token->start);

	fprintf(stderr, ": %s\n", message);
	parser.errorFound = true;
}
