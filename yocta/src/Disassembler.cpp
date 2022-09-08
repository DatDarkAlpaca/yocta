#include "Disassembler.h"
#include "OperationCodes.h"

void yo::Disassembler::operator()(const Chunk& array, const char* instructionSetName)
{
	printf("-=-= Operations : %s =-=-\n", instructionSetName);

	for (unsigned int offset = 0; offset < array.data.size();)
	{
		offset = disassembleInstruction(array, offset);
	}
}

unsigned int yo::Disassembler::disassembleInstruction(const Chunk& chunk, int offset)
{
	printf("%04d\t", offset);

	uint8_t instruction = chunk.data[offset];

	switch (instruction)
	{
	case (uint8_t)OPCode::OP_RETURN:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_CONSTANT:
		return constantInstruction(instruction, chunk, offset);

	default:
		printf("Unknown opcode [%s]\n", translateCode((OPCode)instruction));
		return offset + 1;
	}
}

unsigned int yo::Disassembler::simpleInstruction(uint8_t code, int offset)
{
	printf("%s\n", translateCode((OPCode)code));
	return offset + 1;
}

unsigned int yo::Disassembler::constantInstruction(uint8_t code, const Chunk& chunk, int offset)
{
	uint8_t constant = chunk.data[offset + 1];
	printf("%s\t[Index]: %d | [Value]: %g\n", translateCode((OPCode)code), constant, chunk.constantPool[constant]);

	return offset + 2;
}