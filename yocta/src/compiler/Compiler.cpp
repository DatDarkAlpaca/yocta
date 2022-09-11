#include "Debug.h"
#include "Compiler.h"
#include "Disassembler.h"

yo::Compiler::Compiler()
{
	intializeParserRules();
}

bool yo::Compiler::compile(const char* source, Chunk* chunk)
{
	lexer.open(source);
	currentChunk = chunk;

	parser.errorFound = false;
	parser.panicMode = false;

	advance();

	while (!matchToken(TokenType::T_EOF))
		declaration();

	finish();

	return !parser.errorFound;
}

void yo::Compiler::advance()
{
	parser.previous = parser.current;

	while (true)
	{
		parser.current = lexer.nextToken();

		if (parser.current.type != TokenType::T_ERROR)
			break;

		handleErrorAtCurrentToken(parser.current.data);
	}
}

void yo::Compiler::expression()
{
	parsePrecedence(Precedence::P_ASSIGNMENT);
}

void yo::Compiler::eat(TokenType type, const char* message)
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
	eat(TokenType::T_RIGHT_PARENTHESIS, "Expected ')' after expression.");
}

void yo::Compiler::emitByte(uint8_t byte)
{
	currentChunk->push_back(byte, parser.previous.line);
}

void yo::Compiler::emitConstant(Value value)
{
	emitByte((uint8_t)OPCode::OP_CONSTANT);
	currentChunk->push_constant(value, parser.previous.line);
}

void yo::Compiler::numeric()
{
	double value = std::strtod(parser.previous.data.c_str(), NULL);
	emitConstant({ ValueType::VT_NUMERIC, value });
}

void yo::Compiler::unary()
{
	TokenType type = parser.previous.type;

	parsePrecedence(Precedence::P_UNARY);

	switch (type)
	{
	case TokenType::T_MINUS:
		emitByte((uint8_t)OPCode::OP_NEGATE);
		break;
	case TokenType::T_EXCLAMATION:
		emitByte((uint8_t)OPCode::OP_NOT);
		break;
	}
}

void yo::Compiler::binary()
{
	TokenType type = parser.previous.type;

	Rule* rule = getParserRule(type);

	parsePrecedence((Precedence)((int)rule->precedence + 1));

	switch (type)
	{
	case TokenType::T_PLUS:
		emitByte((uint8_t)OPCode::OP_ADD);
		break;
	case TokenType::T_MINUS:
		emitByte((uint8_t)OPCode::OP_SUB);
		break;
	case TokenType::T_ASTERISTIC:
		emitByte((uint8_t)OPCode::OP_MULT);
		break;
	case TokenType::T_SLASH:
		emitByte((uint8_t)OPCode::OP_DIV);
		break;
		
	case TokenType::T_EQUAL_EQUAL:
		emitByte((uint8_t)OPCode::OP_EQUAL);
		break;
	case TokenType::T_EXCLAMATION_EQUAL:
		emitByte((uint8_t)OPCode::OP_EQUAL);
		emitByte((uint8_t)OPCode::OP_NOT);
		break;
	case TokenType::T_GREATER:
		emitByte((uint8_t)OPCode::OP_GREATER);
		break;
	case TokenType::T_GREATER_EQUAL:
		emitByte((uint8_t)OPCode::OP_LESS);
		emitByte((uint8_t)OPCode::OP_NOT);
		break;
	case TokenType::T_LESS:
		emitByte((uint8_t)OPCode::OP_LESS);
		break;
	case TokenType::T_LESS_EQUAL:
		emitByte((uint8_t)OPCode::OP_GREATER);
		emitByte((uint8_t)OPCode::OP_NOT);
		break;

	}
}

void yo::Compiler::literalType()
{
	switch (parser.previous.type)
	{
	case TokenType::T_NONE:
		emitByte((uint8_t)OPCode::OP_NONE);
		break;
	case TokenType::T_TRUE:
		emitByte((uint8_t)OPCode::OP_TRUE);
		break;
	case TokenType::T_FALSE:
		emitByte((uint8_t)OPCode::OP_FALSE);
		break;
	}
}

void yo::Compiler::string()
{
	std::string str = prepareStringObject();

	emitConstant({ ValueType::VT_OBJECT, (YoctaObject*)(allocateStringObject(str)) });
}

void yo::Compiler::parsePrecedence(const Precedence& precendece)
{
	advance();
	TokenType type = parser.previous.type;
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

std::string yo::Compiler::prepareStringObject() const
{
	std::string str = parser.previous.data;
	std::string a = parser.current.data;

	return str;
}

yo::StringObject* yo::Compiler::allocateStringObject(const std::string& str)
{
	return new StringObject(str);
}

void yo::Compiler::intializeParserRules()
{
	parseRules.insert({
		TokenType::T_LEFT_PARENTHESIS,
		Rule(std::bind(&Compiler::grouping, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_RIGHT_PARENTHESIS,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_LEFT_BRACES,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_RIGHT_BRACES,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_COMMA,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_DOT,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_MINUS,
		Rule(std::bind(&Compiler::unary, this), std::bind(&Compiler::binary, this), Precedence::P_TERM)
	});

	parseRules.insert({
		TokenType::T_PLUS,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_TERM)
	});

	parseRules.insert({
		TokenType::T_SLASH,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_FACTOR)
		});

	parseRules.insert({
		TokenType::T_ASTERISTIC,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_FACTOR)
	});

	parseRules.insert({
		TokenType::T_SEMICOLON,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_EXCLAMATION,
		Rule(std::bind(&Compiler::unary, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_EXCLAMATION_EQUAL,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_EQUAL)
	});

	parseRules.insert({
		TokenType::T_EQUAL,
		Rule(nullptr, nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_EQUAL_EQUAL,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_COMPARE)
	});

	parseRules.insert({
		TokenType::T_GREATER,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_COMPARE)
	});

	parseRules.insert({
		TokenType::T_GREATER_EQUAL,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_COMPARE)
	});

	parseRules.insert({
		TokenType::T_LESS,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_COMPARE)
	});

	parseRules.insert({
		TokenType::T_LESS_EQUAL,
		Rule(nullptr, std::bind(&Compiler::binary, this), Precedence::P_COMPARE)
	});

	parseRules.insert({
		TokenType::T_IDENTIFIER,
		Rule(nullptr, nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_STRING,
		Rule(std::bind(&Compiler::string, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_NUMERIC,
		Rule(std::bind(&Compiler::numeric, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_AND,
		Rule(nullptr, nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_OR,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_IF,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_ELSE,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_FALSE,
		Rule(std::bind(&Compiler::literalType, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_TRUE,
		Rule(std::bind(&Compiler::literalType, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_FOR,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_WHILE,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_NONE,
		Rule(std::bind(&Compiler::literalType, this), nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_PRINT,
		Rule(nullptr, nullptr, Precedence::P_NONE)
	});

	parseRules.insert({
		TokenType::T_VAR,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_FUNC,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_RETURN,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_CLASS,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_SUPER,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_THIS,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_ERROR,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});

	parseRules.insert({
		TokenType::T_EOF,
		Rule(nullptr, nullptr, Precedence::P_NONE)
		});
}

yo::Rule* yo::Compiler::getParserRule(TokenType type)
{
	return &parseRules[type];
}

void yo::Compiler::handleErrorAtCurrentToken(const std::string& message)
{
	handleErrorToken(&parser.current, message);
}

void yo::Compiler::handleErrorToken(Token* token, const std::string& message)
{
	if (parser.panicMode)
		return;

	parser.panicMode = true;

	fprintf(stderr, "<Line %d> Error ", token->line);

	if (token->type == TokenType::T_EOF)
		fprintf(stderr, "at the end of the file");

	else if (token->type == TokenType::T_ERROR) {}

	else
		fprintf(stderr, "at '%.*s'", token->data.length(), token->data.c_str());

	fprintf(stderr, ": %s\n", message.c_str());
	parser.errorFound = true;
}
