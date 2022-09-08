#include <cstdio>

#include "Chunk.h"
#include "Disassembler.h"

#include "OperationCodes.h"

int main()
{
	using namespace yo;

	Chunk chunk;
	chunk.push_back((uint8_t)OPCode::OP_CONSTANT, 123);
	chunk.push_back(10.0, 123);
	chunk.push_back((uint8_t)OPCode::OP_RETURN, 123);

	Disassembler disassembler;
	disassembler(chunk, "Disassembly");

	return 0;
}