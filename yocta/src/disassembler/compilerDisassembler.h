#pragma once

namespace yo
{
	namespace
	{
		int simpleInstruction(uint8_t code, int offset)
		{
			printf("%s\n", translateCode((OPCode)code).c_str());
			return offset + 1;
		}

		int constantInstruction(uint8_t code, const InstructionSet& set, int offset)
		{
			uint8_t constant = set.instructions[++offset];
			YoctaValue value = set.constants[constant];
			std::string textValue = translateYoctaValue(value);

			printf("%s\t\t%d\t[%s]\n", translateCode((OPCode)code).c_str(), constant, textValue.c_str());

			return offset + 1;
		}
	}

	int disassembleInstruction(const InstructionSet& set, int offset)
	{
		printf("%04d\t", offset);

		uint8_t instruction = set.instructions[offset];
		
		switch (instruction)
		{
			case OPCode::OP_PUSH_LITERAL:
				return constantInstruction(OPCode::OP_PUSH_LITERAL, set, offset);

			case OPCode::OP_ADD:
				return simpleInstruction(OPCode::OP_ADD, offset);

			case OPCode::OP_SUB:
				return simpleInstruction(OPCode::OP_SUB, offset);

			case OPCode::OP_MULT:
				return simpleInstruction(OPCode::OP_MULT, offset);

			case OPCode::OP_DIV:
				return simpleInstruction(OPCode::OP_DIV, offset);

			case OPCode::OP_MOD:
				return simpleInstruction(OPCode::OP_MOD, offset);

			default:
				printf("Unknown opcode [%s]\n", translateCode((OPCode)instruction).c_str());
				return offset + 1;
		}
	}

	inline void disassembleInstructionSet(const InstructionSet& set)
	{
		using namespace yo;
		
		printf("-=-= Compiler : Disassembler =-=-\n");

		printf("- Constants: ");
		for (const auto& constant : set.constants)
			printf("[%s]", translateYoctaValue(constant).c_str());

		printf("\n");
		for (unsigned int offset = 0; offset < set.instructions.size();)
			offset = disassembleInstruction(set, offset);
	}
}