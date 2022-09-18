#include <iostream>
#include <sstream>
#include <fstream>

#include "VirtualMachine.h"
#include "InputStream.h"
#include "Compiler.h"
#include "Parser.h"

#include "Debug.h"

#include "compilerDisassembler.h"
#include "parserDisassembler.h"

void execute(std::string_view source, yo::VirtualMachine& virtualMachine)
{
	using namespace yo;

	InputStream stream(source);

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

	virtualMachine.execute(set);
}

void inlineInterpreter()
{
	using namespace yo;

	VirtualMachine virtualMachine;

	while (true)
	{
		printf("> ");

		std::string input;
		std::getline(std::cin, input);

		if (input == "exit" || input == "quit")
			break;

		try {
			execute(input, virtualMachine);
		}
		catch (const Error& e) {
			printf("<Line: %d [CI: %d]> %s\n", (int)e.getLineNumber(), (int)e.getCharIndex(), e.what());
		}
	}
}

std::string_view readFile(std::string_view filepath)
{
	std::ifstream file(filepath.data());

	if (!file.good())
	{
		fprintf(stderr, "An error has occurred while opening the source file.");
		exit(1);
	}

	std::stringstream stringBuffer;
	stringBuffer << file.rdbuf();

	return stringBuffer.str();
}

void runFile(const char* filepath)
{
	using namespace yo;

	VirtualMachine virtualMachine;

	std::string_view source = readFile(filepath);
	execute(source, virtualMachine);
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