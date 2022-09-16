#include <iostream>
#include <sstream>

#include "InputStream.h"
#include "Compiler.h"
#include "Parser.h"

#include "Debug.h"

#include "compilerDisassembler.h"
#include "parserDisassembler.h"

int main()
{
	using namespace yo;

	std::string_view input = "(1 - 5 * (2 + 3));\ntrue;";
	InputStream stream(input);

	Parser parser(stream);
	auto expressions = parser.parse();

#ifdef DEBUG_PARSER_TRACE
	disassembleParser(expressions);
	printf("\n");
#endif

	Compiler compiler(expressions);
	InstructionSet set = compiler.compile();

	disassembleInstructionSet(set);
}