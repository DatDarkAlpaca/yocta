#include <cstdio>
#include <fstream>
#include <sstream>

#include "VirtualMachine.h"

void inlineInterpreter()
{
	yo::VirtualMachine vm;

	while (true)
	{
		printf("> ");

		char line[1024];
		if (!fgets(line, sizeof(line), stdin)) 
		{
			printf("\n");
			return;
		}

		vm.interpret(line);
	}
}

static std::string readFile(const char* filepath)
{
	std::ifstream file(filepath);

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
	yo::VirtualMachine vm;

	std::string src = readFile(filepath);

	vm.interpret(src.c_str());
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
	
	return 0;
}