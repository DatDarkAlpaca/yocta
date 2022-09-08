#include "Chunk.h"

void yo::Chunk::push_back(uint8_t opcode, int lineNumber)
{
	data.push_back(opcode);
	lines.push_back(lineNumber);
}

void yo::Chunk::push_constant(yocta_value constant, int lineNumber)
{
	constantPool.push_back(constant);
	size_t index = constantPool.size() - 1;

	data.push_back((uint8_t)index);
	lines.push_back(lineNumber);
}

void yo::Chunk::clear()
{
	data.clear();
	constantPool.clear();
}