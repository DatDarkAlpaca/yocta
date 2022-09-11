#include "Disassembler.h"
#include "OperationCodes.h"

void yo::Disassembler::disassemble(const Chunk& array, const char* instructionSetName)
{
	printf("-=-= Disassembly : %s =-=-\n", instructionSetName);

	for (unsigned int offset = 0; offset < array.data.size();)
		offset = disassembleInstruction(array, offset);

	printf("\n");
}

unsigned int yo::Disassembler::disassembleInstruction(const Chunk& chunk, int offset)
{
	printf("%04d\t", offset);

	uint8_t instruction = chunk.data[offset];

	printf("%04d\t", chunk.lines[offset]);

	switch (instruction)
	{
	case (uint8_t)OPCode::OP_RETURN:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_CONSTANT:
		return constantInstruction(instruction, chunk, offset);

	case (uint8_t)OPCode::OP_NEGATE:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_ADD:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_SUB:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_MULT:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_DIV:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_NONE:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_TRUE:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_FALSE:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_NOT:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_EQUAL:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_LESS:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_GREATER:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_PRINT:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_POP_BACK:
		return simpleInstruction(instruction, offset);

	case (uint8_t)OPCode::OP_GET_GLOBAL:
		return constantInstruction(instruction, chunk, offset);

	case (uint8_t)OPCode::OP_SET_GLOBAL:
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
	uint8_t constant = chunk.data[++offset];

	Value value = chunk.constantPool[constant];
	if (value.variantValue.index() == 2)
	{
		printf("%s\t[Index]: %d | [Value]: STRING\n", translateCode((OPCode)code), constant);
	}
	else
		printf("%s\t[Index]: %d | [Value]: %g\n", translateCode((OPCode)code), constant, value);

	return offset + 1;
}