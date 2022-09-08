#include <cstdio>

#include "Chunk.h"
#include "Disassembler.h"

#include "OperationCodes.h"

int main()
{
	using namespace yo;

	Chunk chunk;
	chunk.push_back((uint8_t)OPCode::OP_CONSTANT);
	chunk.push_back(10.0);
	chunk.push_back((uint8_t)OPCode::OP_RETURN);

	Disassembler disassembler;
	disassembler(chunk, "Disassembly");

	return 0;
}