#include <iostream>
#include <sstream>

#include "VirtualMachine.h"
#include "InputStream.h"
#include "Compiler.h"
#include "Parser.h"

#include "Debug.h"

#include "compilerDisassembler.h"
#include "parserDisassembler.h"


void inlineInterpreter()
{
	using namespace yo;

	VirtualMachine virtualMachine;

	while (true)
	{
		printf("> ");

		std::string input;
		std::getline(std::cin, input);

		try
		{
			InputStream stream(input);

			Parser parser(stream);
			auto expressions = parser.parse();

#ifdef DEBUG_PARSER_TRACE
			disassembleParser(expressions);
			printf("\n");
#endif

			Compiler compiler(expressions);
			InstructionSet set = compiler.compile();

#ifdef DEBUG_COMPILER_TRACE
			disassembleInstructionSet(set);
#endif

			VirtualMachine virtualMachine;
			virtualMachine.execute(set);
		}
		catch (const Error& e) {
			printf("<Line: %d [CI: %d]> %s\n", (int)e.getLineNumber(), (int)e.getCharIndex(), e.what());
		}
	}
}

void runFile(const char* filepath)
{

}


int main(int argc, char** argv)
{
	if (argc == 1)
		inlineInterpreter();
	else if (argc == 2)
		runFile(argv[1]);
	else
	{
		fprintf(stderr, "Usage: yocta <filepath>\n");
		return 1;
	}
}