#include "Disassembler.h"

void yo::Disassembler::operator()(const InstructionArray& array, const char* instructionSetName)
{
	printf("-=-= %s =-=-\n", instructionSetName);

	for (unsigned int offset = 0; offset < array.count;)
	{
		offset = disassembleInstruction(array, offset);
	}
}

unsigned int yo::Disassembler::disassembleInstruction(const InstructionArray& array, int offset)
{
	printf("%04d\t", offset);

	uint8_t instruction = array.data[offset];

	switch (instruction)
	{
	case (uint8_t)OPCode::OP_RETURN:
		return simpleInstruction("OP_RETURN", offset);

	default:
		printf("Unknown opcode %d\n", instruction);
		return offset + 1;
	}
}

unsigned int yo::Disassembler::simpleInstruction(const char* name, int offset)
{
	printf("%s\n", name);
	return offset + 1;
}
