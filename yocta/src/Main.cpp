#include <cstdio>

#include "VirtualMachine.h"
#include "OperationCodes.h"
#include "Disassembler.h"
#include "Chunk.h"

int main()
{
	using namespace yo;

	Chunk chunk;
	chunk.push_back((uint8_t)OPCode::OP_CONSTANT, 123);
	chunk.push_back(10.0, 123);

	chunk.push_back((uint8_t)OPCode::OP_CONSTANT, 124);
	chunk.push_back(20.0, 124);

	chunk.push_back((uint8_t)OPCode::OP_NEGATE, 125);

	chunk.push_back((uint8_t)OPCode::OP_ADD, 125);

	chunk.push_back((uint8_t)OPCode::OP_RETURN, 126);

	VirtualMachine vm;
	vm.run(chunk);

	return 0;
}